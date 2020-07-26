#include <Ether.h>
#include "ether_private.h"

typedef struct _Ether_Render_Output_Object  EtherRenderOutputObject;
typedef struct _Ether_Temp_Vertex           EtherTempVertex;

struct _Ether_Render_Output_Object
{
  EtherObject *original;           /**< points back at the object we came from */
  EtherOutputfacet **outfacets;   /**< array of output facets */
  int noutfacets;                   /**< number of output facets */
  int not_drawn;                    /**< set if object hasn't been drawn yet */
};

struct _Ether_Temp_Vertex
{
  EtherVector vertex;              /* transformed vertex */
  long int projected_x;             /* projected X */
  long int projected_y;             /* projected Y */
  EtherFactor intensity;           /* for Gouraud shading */
  enum
  { 
    TEMP_VERTEX_UNPROCESSED = 0, 
    TEMP_VERTEX_Z_DONE, 
    TEMP_VERTEX_XY_DONE, 
    TEMP_VERTEX_PROJECTED 
  } status;
};

static EtherStatistics Stats;

static EtherRenderOutputObject **_ether_render_objarray = NULL;   /**< array of object info (e.g. depth) */
static int _ether_render_max_objs = 0;                               /**< maximum number of entries in the array */
static int _ether_render_nobjs;                                      /**< number of entries actually in the array */
static int _ether_render_first;                                      /**< counts through objarray[] */

static EtherOutputfacet **_ether_render_outfacets = NULL;          /**< array of outfacets for sorting */
static int _ether_render_max_facets = 0;                             /**< maximum number of entries in outfacets[] */
static int _ether_render_nout_facets = 0;                            /**< number of entries currently in outfacets[] */

static EtherTempVertex *_ether_render_tempverts = NULL;
static int _ether_render_max_vertices = 0;                           /**< maximum number of entries in above four arrays */

static EtherRenderOutputObject *_ether_render_current_obj;        /**< current output object */
static EtherSurfaceMap *_ether_render_current_surfmap;             /**< pointer to current object's surface map */

static EtherVector *_ether_render_vert_array;                       /**< points to current rep's vertices */
static EtherVector *_ether_render_norm_array;                       /**< points to current rep's normals */

/* there should be functions to set these next two: */
static int _ether_render_object_sorting = 1;
static int _ether_render_facet_sorting = 1;

static long int _ether_render_horizontal_shift = 0;

/* Keep track of display capabilities; these are updated by ether_render_begin(), 
 * in case the information changed since the last frame
 */
static int _ether_render_display_clips       = 0;
static int _ether_render_display_can_gouraud = 0;

#define _ETHER_GOURAUD_SHADING(surf) (((surf)->type == ETHER_SURF_GOURAUD || (surf)->type == ETHER_SURF_SPECULAR) && _ether_render_display_can_gouraud && !_ether_render_wireframe)

/* offset into window of center of window */
static long int _ether_render_window_halfwidth;
static long int _ether_render_window_halfheight;   

/* these next four variables are only used to see if we can skip xy clipping */
static long int _ether_render_left_clip;
static long int _ether_render_right_clip;
static long int _ether_render_top_clip;
static long int _ether_render_bottom_clip;

static float _ether_render_aspect;

/* viewpoint in world coordinates */
static EtherVector _ether_render_viewpoint;

/* the direction we're looking in */
static EtherVector _ether_render_viewdir;   

/* current camera's near clipping plane distance */
static EtherScalar _ether_render_hither;

/* current camera's far clipping plane distance */
static EtherScalar _ether_render_yon;

/* current camera's world-to-view matrix */
static EtherMatrix _ether_render_wtv;

/* object-to-view matrix */
static EtherMatrix _ether_render_otv;

/* non-zero if we're orthoscopic */
static unsigned char _ether_render_ortho;  

/* divisor (instead of v[Z]) for orthoscopic viewing */
static EtherScalar _ether_render_orthodiv;

/* view-to-screen scale factors */
static long int _ether_render_screenscale_x;
static long int _ether_render_screenscale_y; 

static EtherLight        *_ether_render_lightlist = NULL;   /* point to (world) list of lights */
static int                 _ether_render_nlights = 0;        /* number of lights in the scene */                      
static EtherLocalLight  *_ether_render_locallights = NULL;
static int                 _ether_render_maxlights = 0;
static EtherFactor        _ether_render_ambient = ETHER_FLOAT_TO_FACTOR(0.5);

static EtherRenderStatus _ether_render_status;
#ifdef ETHER_NEW_PROC
static int _ether_render_direct = 0;
#endif

/* coefficients of clipping planes */
static EtherFactor _ether_render_aright;
static EtherFactor _ether_render_cright;
static EtherFactor _ether_render_btop;
static EtherFactor _ether_render_ctop;

static unsigned char _ether_render_layerbits[32] = { 0xFF }; /**< all layers visible initially */
static unsigned char _ether_render_bitmasks[]    = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

/* Screen Monitoring */
static int                _ether_render_monitor_x;
static int                _ether_render_monitor_y;
static int                _ether_render_monitoring = 0;
static EtherOutputfacet *_ether_render_monitor_facet;

/* Z clipper routines */

/* Uses outcodes; here are the bit values: */
#define _ETHER_RENDER_IS_ABOVE  0x01
#define _ETHER_RENDER_IS_BELOW  0x02
#define _ETHER_RENDER_IS_LEFT   0x04
#define _ETHER_RENDER_IS_RIGHT  0x08

/* stores output vertex in array for processing by display routines */
static EtherOutputvertex *_ether_render_first_point = NULL;
static EtherOutputvertex *_ether_render_last_point  = NULL;

/* zclip: does z clipping, computes perspective */
/* Produces clipped vertices, and */
/* returns 0 normally */
/* returns 1 if facet is entirely clipped in X or Y */
/* returns 2 if no XY clipping is needed */
static int   _ether_render_zclip               (EtherFacet *facet, EtherOutputfacet *of);
static int   _ether_render_no_zclip            (EtherFacet *facet, EtherOutputfacet *of);
static int   _ether_render_addvertex           (long x, long y, long z, EtherFactor intensity, unsigned char ocode, EtherOutputfacet *of, int ind);

static void  _ether_render_camera_update       (EtherCameraOld *camera);
static void  _ether_render_horizon_poly        (EtherOutputvertex *vertices, EtherColor color);
static void  _ether_render_draw_horizon        (long int x1, long int y1, long int x2, long int y2);
static void  _ether_render_process_object      (EtherObject *obj);
static void  _ether_render_draw                (EtherRenderOutputObject *obj);
static int   _ether_render_obscures            (EtherRenderOutputObject *a, EtherRenderOutputObject *b);
static void  _ether_render_monitor             (EtherOutputfacet *of);
static int   _ether_render_show_facet          (EtherOutputfacet *of);
static void  _ehter_render_show_bbox           (EtherRenderOutputObject *obj);
static void  _ether_render_draw_obj            (EtherRenderOutputObject *obj);

/* Backface Removal */
/* return non-zero if facet is backfacing */
static int   _ether_render_back_facing         (EtherFacet *facet);

/* Facet Processing */
/* parent will be passed as non-NULL if we're a detail facet */
static void  _ether_render_process_facet       (EtherFacet *facet, EtherOutputfacet *parent);

/* Facet Sorting */
/* This next routine should be re-coded to be non-recursive, for speed */
static void  _ether_render_bsp_process_facet   (EtherFacet *facet);
static int   _ether_render_facet_sort_nearest  (const void *a, const void *b);
static int   _ether_render_facet_sort_farthest (const void *a, const void *b);

/* Lighting Calculations */
static EtherFactor _ether_render_compute_lighting(EtherVector normal, EtherVector vertex);

/* Layer support */
#ifdef LAYER_SUPPORT
static void  _ether_render_layer_on            (int n);
static void  _ether_render_layer_off           (int n);
static void  _ether_render_layer_toggle        (int n);
static int   _ether_render_layer_is_on         (int n);
static void  _ether_render_layer_all_on        (void);
static void  _ether_render_layer_all_off       (void);
#endif

#ifdef ETHER_NEW_PROC
static int _ether_process_direct = 0;
#endif

/* DEBUGGING ONLY: */
int _ether_render_pause = 0;              /* if set, pause before rendering each object */
int _ether_render_show_bb = 0;            /* if set, show the bounding box around objects */
EtherColor _ether_render_bbox_color = 4; /* bounding boxes in red */
/* END OF DEBUGGING VARIABLES */

/* Highlighting and Wireframe */
static int _ether_render_highlight_facets = 0;   /* non-zero if current object is highlighted */
static EtherColor _ether_render_highlight_color = 255;
static EtherColor _ether_render_wireframe_color = 1;
static int _ether_render_wireframe = 0;

/* These flags are set to non-zero if current object needs clipping */
static unsigned char _ether_render_need_x_clipping;
static unsigned char _ether_render_need_y_clipping;
static unsigned char _ether_render_need_z_clipping;

/* Miscellaneous globals */
static EtherVector _ether_render_vp;        /* viewpoint in current object's coordinates */
static EtherVector _ether_render_viewdir;   /* the direction we're looking in */

int
ether_render_init(int maxvert, int maxf, int maxobjs, int maxlights, unsigned int mempoolsize)
{
  if(_ether_memory_init(mempoolsize))
    return -1;
  _ether_render_max_facets   = maxf;
  _ether_render_max_vertices = maxvert;
  _ether_render_max_objs     = maxobjs;
  _ether_render_maxlights    = maxlights;
  _ether_render_outfacets    = calloc(_ether_render_max_facets, sizeof(EtherOutputfacet *));
  _ether_render_tempverts    = calloc(_ether_render_max_vertices, sizeof(EtherTempVertex));
  _ether_render_objarray     = calloc(_ether_render_max_objs, sizeof(EtherRenderOutputObject *));
  _ether_render_locallights  = calloc(_ether_render_maxlights, sizeof(EtherLocalLight));
  if(_ether_render_outfacets    == NULL || 
      _ether_render_tempverts   == NULL || 
      _ether_render_objarray    == NULL || 
      _ether_render_locallights == NULL)
  {
    ether_render_quit();
    return -1;
  }
  return 0;
}

void 
ether_render_quit(void)
{
  if(_ether_render_locallights)
    free(_ether_render_locallights);
  if(_ether_render_objarray)
    free(_ether_render_objarray);
  if(_ether_render_tempverts)
    free(_ether_render_tempverts);
  if(_ether_render_outfacets)
    free(_ether_render_outfacets);
  _ether_memory_quit();
}

void
ether_render_begin(EtherCameraOld *camera, EtherLight *lights)
{
  int window_left, window_right, window_top, window_bottom;
  EtherMatrix cammat;
  if(camera == NULL)
    return;
  if(camera->need_updating)
    _ether_render_camera_update(camera);
  _ether_render_display_can_gouraud = ether_display_can_gouraud();
  _ether_render_display_clips = ether_display_can_xyclip();
  ether_display_window_get(&window_left, &window_top, &window_right, &window_bottom);

  /* compute offset of screen center from top left of viewport */
  _ether_render_window_halfwidth  = ((long int)(window_right  - window_left + 1) / 2) << _ETHER_SCREEN_FRACT_BITS;
  _ether_render_window_halfheight = ((long int)(window_bottom - window_top  + 1) / 2) << _ETHER_SCREEN_FRACT_BITS;
  _ether_render_aspect = camera->aspect;

  /* these _clip variables are only used to check if we need to xy_clip facets */
  _ether_render_right_clip  =  _ether_render_window_halfwidth - 1;
  _ether_render_left_clip   = -_ether_render_right_clip;
  _ether_render_bottom_clip =  _ether_render_window_halfheight - 1;
  _ether_render_top_clip    = -_ether_render_bottom_clip;

  /* should eventually nuke cammat altogether */
  if(camera->object == NULL)  /* no parent; we're at the origin, looking into +Z */
    ether_matrix_identity(cammat);
  else /* normal case; we inherit our parent's globalmat */
    ether_matrix_copy(cammat, camera->object->globalmat);

  /* set up a few globals that are constant for this scene */
  ether_vector_copy(_ether_render_viewpoint, cammat[_ETHER_TRANS]);
  _ether_render_viewdir[X] = cammat[X][Z];
  _ether_render_viewdir[Y] = cammat[Y][Z];
  _ether_render_viewdir[Z] = cammat[Z][Z];
  _ether_render_hither = camera->hither;
  if(_ether_render_hither < 1) 
    _ether_render_hither = 1;
  _ether_render_yon = camera->yon;
  if(_ether_render_yon < 1)
    _ether_render_yon = 1;
  ether_matrix_inverse(_ether_render_wtv, cammat);
  _ether_render_ortho    = camera->ortho;
  _ether_render_orthodiv = camera->orthodist;
  if(_ether_render_orthodiv == 0) 
    _ether_render_orthodiv = camera->hither;

  /* compute screen scale factors and set clipping plane coefficients */
  if(_ether_render_ortho && 0)
  {
    _ether_render_screenscale_x = (long int)ETHER_FLOAT_TO_SCALAR(_ether_render_window_halfwidth);
    _ether_render_screenscale_y = (long int)ETHER_FLOAT_TO_SCALAR(-camera->aspect * _ether_render_window_halfheight);

    /* cright = window_halfwidth * orthodiv / screenscale_x */
    _ether_render_cright = ETHER_FLOAT_TO_FACTOR(_ether_render_orthodiv);

    /* ctop = window_halfheight * orthodiv / screenscale_y */
    _ether_render_ctop = ETHER_FLOAT_TO_FACTOR(_ether_render_orthodiv / camera->aspect);
  }
  else
  {
    _ether_render_screenscale_x = (long int)ETHER_FLOAT_TO_SCALAR(camera->zoom * _ether_render_window_halfwidth);
    _ether_render_screenscale_y = (long int)ETHER_FLOAT_TO_SCALAR(-camera->zoom * camera->aspect * _ether_render_window_halfheight);

    /* copy clipping plane coefficients for object culling */
    _ether_render_aright = camera->aright;
    _ether_render_cright = camera->cright;
    _ether_render_btop   = camera->btop;  
    _ether_render_ctop   = camera->ctop;
  }
  _ether_memory_clear();          /* free up memory */
  _ether_render_nobjs          = 0;      /* no output objects yet */
  _ether_render_nout_facets    = 0;      /* no output facets yet */
  _ether_render_lightlist      = lights; /* store a copy of the list of lights */
  _ether_render_status.objects = 0;
  _ether_render_status.facets  = 0;
#ifdef ETHER_NEW_PROC
  if (_ether_render_direct)
    _ether_render_new_process_begin_scene();
#endif
}

void
ether_render_ambient_set(EtherFactor amb)
{
  _ether_render_ambient = amb;
}

void
ether_render_horizon(void)
{
  EtherVector p, q, a, b;

#if 1
  ether_display_clear(ether_world_sky_color_get());
  return;
#endif

  /* compute endpoints of "chord" of horizon circle, in world space */
  if(_ether_render_viewdir[X] == 0 && _ether_render_viewdir[Y] == 0 && _ether_render_viewdir[Z] == 0)
    return;
  p[X] = ether_math_factor_multiply(_ether_render_viewdir[X] - _ether_render_viewdir[Z], ETHER_FLOAT_TO_SCALAR(32000));
  p[Y] = 0;
  p[Z] = ether_math_factor_multiply(_ether_render_viewdir[Z] + _ether_render_viewdir[X], ETHER_FLOAT_TO_SCALAR(32000));
  q[X] = ether_math_factor_multiply(_ether_render_viewdir[X] + _ether_render_viewdir[Z], ETHER_FLOAT_TO_SCALAR(32000));
  q[Y] = 0;
  q[Z] = ether_math_factor_multiply(_ether_render_viewdir[Z] - _ether_render_viewdir[X], ETHER_FLOAT_TO_SCALAR(32000));
  if(_ether_render_viewdir[Y] < -ETHER_FLOAT_TO_FACTOR(0.85))
  {
    ether_display_clear(ether_world_ground_color_get());
    return;
  }
  if(_ether_render_viewdir[Y] > ETHER_FLOAT_TO_FACTOR(0.85))
  {
    ether_display_clear(ether_world_sky_color_get());
    return;
  }

  /* Transform to viewspace */
  _ether_transform(a, _ether_render_wtv, p);
  _ether_transform(b, _ether_render_wtv, q);

  /* project and scale to screen space */
  if(a[Z] < 1)
    a[Z] = 1;
  if(b[Z] < 1)
    b[Z] = 1;
  a[X] = ether_math_scalar_mult_div(a[X], (float) _ether_render_screenscale_x, a[Z]);
  a[Y] = ether_math_scalar_mult_div(a[Y], (float)-_ether_render_screenscale_y, a[Z]);
  b[X] = ether_math_scalar_mult_div(b[X], (float) _ether_render_screenscale_x, b[Z]);
  b[Y] = ether_math_scalar_mult_div(b[Y], (float)-_ether_render_screenscale_y, b[Z]);
  _ether_render_draw_horizon((long int)a[X], (long int)a[Y], (long int)b[X], (long int)b[Y]);
}


/* should merge the two loops in this next routine, and get rid of
   _ether_render_objarray[] altogether, and ... */
EtherRenderStatus *
ether_render_objlist(EtherObject *objects)
{
  register EtherObject *obj;
  register EtherRenderOutputObject *p;
  for(obj = objects; obj; obj = obj->next)
    _ether_render_process_object(obj);
#ifdef ETHER_NEW_PROC
  if (_ether_process_direct)
  {
    _ether_render_new_process_end_scene();
    return &_ether_render_status;
  }
#endif
  p = NULL;
  if(_ether_render_object_sorting)  /* no need to sort; just draw */
    for(_ether_render_first = 0; _ether_render_first < _ether_render_nobjs; ++_ether_render_first)
    {
      p = _ether_render_objarray[_ether_render_first];
      if(p->not_drawn)
        _ether_render_draw(_ether_render_objarray[_ether_render_first]);
    }
  else  /* need to sort */
    for (_ether_render_first = 0; _ether_render_first < _ether_render_nobjs; ++_ether_render_first)
      _ether_render_draw_obj(_ether_render_objarray[_ether_render_first]);
  return &_ether_render_status;
}

void
ether_render_horizontal_shift_set(int n)
{
  _ether_render_horizontal_shift = ((int) n) << _ETHER_SCREEN_FRACT_BITS;
}

void 
ether_render_draw_mode_set(int mode)
{
  _ether_render_wireframe = mode;
}

int
ether_render_draw_mode_get(void)
{
  return _ether_render_wireframe;
}

void
ether_render_wireframe_color_set(EtherColor color)
{
	_ether_render_wireframe_color = color;
}

EtherColor 
ether_render_wireframe_color_get(void)
{
  return _ether_render_wireframe_color;
}

void 
ether_render_highlight_color_set(EtherColor color)
{
	_ether_render_highlight_color = color;
}

EtherColor
ether_render_highlight_color_get(void)
{
  return _ether_render_highlight_color;
}

void 
ether_render_monitor_init(int x, int y)
{
  _ether_render_monitoring    = 1;
  _ether_render_monitor_x     = x; 
  _ether_render_monitor_y     = y;
  _ether_render_monitor_facet = NULL;
}

int
ether_render_monitor_read(EtherObject **obj, EtherFacet **facet, int *vertnum)
{
  _ether_render_monitoring = 0;
  if(_ether_render_monitor_facet == NULL)
  {
    if(obj)
      *obj = NULL;
    if(facet)
      *facet = NULL;
    if(vertnum)
      *vertnum = 0;
    return 0;
  }
  if(facet)
    *facet = _ether_render_monitor_facet->original;
  if(obj)
    *obj = ((EtherRenderOutputObject *)(_ether_render_monitor_facet->outobj))->original;
  if(vertnum)
    *vertnum = 0;   /* should determine which vertex it was */
  return 1;
}

void
ether_transform_vertex_to_screen(long int *x, long int *y, EtherObject *obj, EtherVector vertex)
{
  EtherMatrix otv;
  EtherVector v;
  if(ether_object_is_fixed(obj))
    ether_matrix_copy(otv, _ether_render_wtv);
  else
    ether_matrix_multiply(otv, _ether_render_wtv, obj->globalmat);
  _ether_transform(v, otv, vertex);
  if(_ether_render_ortho)
  {
    *x = (long)ether_math_scalar_mult_div(v[X], (EtherScalar)_ether_render_screenscale_x, _ether_render_orthodiv) + _ether_render_horizontal_shift;
    *y = (long)ether_math_scalar_mult_div(v[Y], (EtherScalar)_ether_render_screenscale_y, _ether_render_orthodiv);
  }
  else
  {
    *x = (long)ether_math_scalar_mult_div(v[X], (EtherScalar)_ether_render_screenscale_x, v[Z]) + _ether_render_horizontal_shift;
    *y = (long)ether_math_scalar_mult_div(v[Y], (EtherScalar)_ether_render_screenscale_y, v[Z]);
  }
  *x += _ether_render_window_halfwidth;  
  *y += _ether_render_window_halfheight;
}

static int 
_ether_render_zclip(EtherFacet *facet, EtherOutputfacet *of)
{
  EtherScalar x, y;
  int i;
  int thisone;
  int prevone = facet->points[facet->npoints - 1];
  unsigned char ocode    = 0;
  unsigned char ocode_or = 0;
  unsigned char ocode_and = (_ETHER_RENDER_IS_ABOVE|_ETHER_RENDER_IS_BELOW|_ETHER_RENDER_IS_LEFT|_ETHER_RENDER_IS_RIGHT);
  ++Stats.facets_need_z_clipping;
  for(i = 0; i < facet->npoints; ++i)
  {
    thisone = facet->points[i];
    if(_ether_render_tempverts[prevone].vertex[Z] >= _ether_render_hither && 
	_ether_render_tempverts[thisone].vertex[Z] >= _ether_render_hither)  /* edge on-screen */
    {
      if(_ether_render_tempverts[thisone].status < TEMP_VERTEX_PROJECTED)
      {
	if(_ether_render_ortho)
	{
	  _ether_render_tempverts[thisone].projected_x  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, (EtherScalar)_ether_render_orthodiv);
	  _ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	  _ether_render_tempverts[thisone].projected_y  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, (EtherScalar)_ether_render_orthodiv);
	}
	else
	{
	  _ether_render_tempverts[thisone].projected_x  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, _ether_render_tempverts[thisone].vertex[Z]);
	  _ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	  _ether_render_tempverts[thisone].projected_y  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, _ether_render_tempverts[thisone].vertex[Z]);
	}
	_ether_render_tempverts[thisone].status = TEMP_VERTEX_PROJECTED;
      }
      if(!_ether_render_display_clips && (_ether_render_need_x_clipping || _ether_render_need_y_clipping))
      {
	ocode = 0;
	if(_ether_render_tempverts[thisone].projected_x < _ether_render_left_clip)
	  ocode |= _ETHER_RENDER_IS_LEFT;
	else if(_ether_render_tempverts[thisone].projected_x > _ether_render_right_clip)
	  ocode |= _ETHER_RENDER_IS_RIGHT;
	if(_ether_render_tempverts[thisone].projected_y < _ether_render_bottom_clip)
	  ocode |= _ETHER_RENDER_IS_BELOW;
	else if(_ether_render_tempverts[thisone].projected_y > _ether_render_top_clip)
	  ocode |= _ETHER_RENDER_IS_ABOVE;
	ocode_or |= ocode;
	ocode_and &= ocode;
      }
      if (_ether_render_addvertex(_ether_render_tempverts[thisone].projected_x, 
	    _ether_render_tempverts[thisone].projected_y, 
	    (long)_ether_render_tempverts[thisone].vertex[Z], 
	    _ether_render_tempverts[thisone].intensity, ocode, of, i))
	return 1;
    }
    else if(_ether_render_tempverts[prevone].vertex[Z] < _ether_render_hither && _ether_render_tempverts[thisone].vertex[Z] < _ether_render_hither)  /* edge off-screen */
      ;
    else if(_ether_render_tempverts[prevone].vertex[Z] >= _ether_render_hither && _ether_render_tempverts[thisone].vertex[Z] < _ether_render_hither)  /* leaving */
    {
      EtherFactor intensity;
      EtherScalar numer = _ether_render_hither - _ether_render_tempverts[thisone].vertex[Z];
      EtherScalar denom = _ether_render_tempverts[prevone].vertex[Z] - _ether_render_tempverts[thisone].vertex[Z];
      x = _ether_render_tempverts[thisone].vertex[X] + 
	ether_math_scalar_mult_div(_ether_render_tempverts[prevone].vertex[X] - 
	    _ether_render_tempverts[thisone].vertex[X], numer, denom);
      y = _ether_render_tempverts[thisone].vertex[Y] + 
	ether_math_scalar_mult_div(_ether_render_tempverts[prevone].vertex[Y] - 
	    _ether_render_tempverts[thisone].vertex[Y], numer, denom);
      if (_ETHER_GOURAUD_SHADING(of->surface))
      {
	intensity = _ether_render_tempverts[thisone].intensity + 
	  ether_math_scalar_mult_div(_ether_render_tempverts[prevone].intensity - 
	      _ether_render_tempverts[thisone].intensity, numer, denom);
      }
      else
      { 
	intensity = of->intensity;  /* otherwise, use facet's intensity */
      }
      if(_ether_render_ortho)
      {
	x  = ether_math_scalar_mult_div(x, (EtherScalar)_ether_render_screenscale_x, _ether_render_orthodiv); 
	x += _ether_render_horizontal_shift;
	y  = ether_math_scalar_mult_div(y, (EtherScalar)_ether_render_screenscale_y, _ether_render_orthodiv); 
      }
      else
      {
	x  = ether_math_scalar_mult_div(x, (EtherScalar)_ether_render_screenscale_x, _ether_render_hither); 
	x += _ether_render_horizontal_shift;
	y  = ether_math_scalar_mult_div(y, (EtherScalar)_ether_render_screenscale_y, _ether_render_hither); 
      }
      if(!_ether_render_display_clips && (_ether_render_need_x_clipping || _ether_render_need_y_clipping))
      {
	ocode = 0;
	if(x < _ether_render_left_clip) 
	  ocode |= _ETHER_RENDER_IS_LEFT;
	else if(x > _ether_render_right_clip) 
	  ocode |= _ETHER_RENDER_IS_RIGHT;
	if(y < _ether_render_bottom_clip) 
	  ocode |= _ETHER_RENDER_IS_BELOW;
	else if(y > _ether_render_top_clip)
	  ocode |= _ETHER_RENDER_IS_ABOVE;
	ocode_or |= ocode;
	ocode_and &= ocode;
      }
      if(_ether_render_addvertex((long)x, (long)y, (long)(_ether_render_ortho ? _ether_render_orthodiv : _ether_render_hither), intensity, ocode, of, i))
	return 1;
    }
    else /* entering */
    {
      EtherFactor intensity;
      EtherScalar numer = _ether_render_hither - _ether_render_tempverts[prevone].vertex[Z];
      EtherScalar denom = _ether_render_tempverts[thisone].vertex[Z] - _ether_render_tempverts[prevone].vertex[Z];
      x = _ether_render_tempverts[prevone].vertex[X] + 
	ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X] - 
	    _ether_render_tempverts[prevone].vertex[X], numer, denom);
      y = _ether_render_tempverts[prevone].vertex[Y] + 
	ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y] - 
	    _ether_render_tempverts[prevone].vertex[Y], numer, denom);
      if(_ETHER_GOURAUD_SHADING(of->surface))
	intensity = _ether_render_tempverts[prevone].intensity + 
	  ether_math_scalar_mult_div(_ether_render_tempverts[thisone].intensity - 
	      _ether_render_tempverts[prevone].intensity, numer, denom);
      else
	intensity = of->intensity;  /* otherwise, use facet's intensity */
      if(_ether_render_ortho)
      {
	x = ether_math_scalar_mult_div(x, (EtherScalar)_ether_render_screenscale_x, _ether_render_orthodiv);
	x += _ether_render_horizontal_shift;
	y = ether_math_scalar_mult_div(y, (EtherScalar)_ether_render_screenscale_y, _ether_render_orthodiv);
      }
      else
      {
	x = ether_math_scalar_mult_div(x, (EtherScalar)_ether_render_screenscale_x, _ether_render_hither);
	x += _ether_render_horizontal_shift;
	y = ether_math_scalar_mult_div(y, (EtherScalar)_ether_render_screenscale_y, _ether_render_hither);
      }
      if(!_ether_render_display_clips && (_ether_render_need_x_clipping || _ether_render_need_y_clipping))
      {
	ocode = 0;
	if(x < _ether_render_left_clip)
	  ocode |= _ETHER_RENDER_IS_LEFT;
	else if(x > _ether_render_right_clip)
	  ocode |= _ETHER_RENDER_IS_RIGHT;
	if(y < _ether_render_bottom_clip)
	  ocode |= _ETHER_RENDER_IS_BELOW;
	else if(y > _ether_render_top_clip)
	  ocode |= _ETHER_RENDER_IS_ABOVE;
	ocode_or |= ocode;
	ocode_and &= ocode;
      }
      if(_ether_render_addvertex((long)x, (long)y, (long)(_ether_render_ortho ? _ether_render_orthodiv : _ether_render_hither), intensity, ocode, of, i))
	return 1;
      if(_ether_render_tempverts[thisone].status < TEMP_VERTEX_PROJECTED)
      {
	if (_ether_render_ortho)
	{
	  _ether_render_tempverts[thisone].projected_x = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, (EtherScalar)_ether_render_orthodiv);
	  _ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	  _ether_render_tempverts[thisone].projected_y = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, (EtherScalar)_ether_render_orthodiv);
	}
	else
	{
	  _ether_render_tempverts[thisone].projected_x = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, _ether_render_tempverts[thisone].vertex[Z]);
	  _ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	  _ether_render_tempverts[thisone].projected_y = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, _ether_render_tempverts[thisone].vertex[Z]);
	}
	_ether_render_tempverts[thisone].status = TEMP_VERTEX_PROJECTED;
      }
      if(!_ether_render_display_clips && (_ether_render_need_x_clipping || _ether_render_need_y_clipping))
      {
	ocode = 0;
	if(_ether_render_tempverts[thisone].projected_x < _ether_render_left_clip)
	  ocode |= _ETHER_RENDER_IS_LEFT;
	else if(_ether_render_tempverts[thisone].projected_x > _ether_render_right_clip)
	  ocode |= _ETHER_RENDER_IS_RIGHT;
	if(_ether_render_tempverts[thisone].projected_y < _ether_render_bottom_clip)
	  ocode |= _ETHER_RENDER_IS_BELOW;
	else if(_ether_render_tempverts[thisone].projected_y > _ether_render_top_clip)
	  ocode |= _ETHER_RENDER_IS_ABOVE;
	ocode_or |= ocode;
	ocode_and &= ocode;
      }
      if(_ether_render_addvertex(_ether_render_tempverts[thisone].projected_x, _ether_render_tempverts[thisone].projected_y, (long)_ether_render_tempverts[thisone].vertex[Z], _ether_render_tempverts[thisone].intensity, ocode, of, i))
	return 1;
    }
    prevone = thisone;
  }
  if (ocode_or == 0) return 2;  /* needs no xy clipping */
#ifdef ENTIRELY_CLIPPED
  if (ocode_and) return 1;      /* entirely clipped */
#endif
  return 0;
}

static int 
_ether_render_no_zclip(EtherFacet *facet, EtherOutputfacet *of)
{
  int i;
  unsigned char ocode = 0;
  unsigned char ocode_or = 0;
  unsigned char ocode_and = (_ETHER_RENDER_IS_ABOVE|_ETHER_RENDER_IS_BELOW|_ETHER_RENDER_IS_LEFT|_ETHER_RENDER_IS_RIGHT);
  for(i = 0; i < facet->npoints; ++i)
  {
    int thisone = facet->points[i];
    if(_ether_render_tempverts[thisone].status < TEMP_VERTEX_PROJECTED)
    {
      if(_ether_render_ortho)
      {
	_ether_render_tempverts[thisone].projected_x  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, (EtherScalar)_ether_render_orthodiv);
	_ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	_ether_render_tempverts[thisone].projected_y  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, (EtherScalar)_ether_render_orthodiv);
      }
      else
      {
	_ether_render_tempverts[thisone].projected_x  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[X], (EtherScalar)_ether_render_screenscale_x, (EtherScalar)_ether_render_tempverts[thisone].vertex[Z]);
	_ether_render_tempverts[thisone].projected_x += _ether_render_horizontal_shift;
	_ether_render_tempverts[thisone].projected_y  = (long)ether_math_scalar_mult_div(_ether_render_tempverts[thisone].vertex[Y], (EtherScalar)_ether_render_screenscale_y, (EtherScalar)_ether_render_tempverts[thisone].vertex[Z]);
      }
      _ether_render_tempverts[thisone].status = TEMP_VERTEX_PROJECTED;
    }
    if(!_ether_render_display_clips && (_ether_render_need_x_clipping || _ether_render_need_y_clipping))
    {
      ocode = 0;
      if(_ether_render_tempverts[thisone].projected_x < _ether_render_left_clip)
	ocode |= _ETHER_RENDER_IS_LEFT;
      else if(_ether_render_tempverts[thisone].projected_x > _ether_render_right_clip) 
	ocode |= _ETHER_RENDER_IS_RIGHT;
      if(_ether_render_tempverts[thisone].projected_y < _ether_render_bottom_clip)
	ocode |= _ETHER_RENDER_IS_BELOW;
      else if(_ether_render_tempverts[thisone].projected_y > _ether_render_top_clip) 
	ocode |= _ETHER_RENDER_IS_ABOVE;
      ocode_or |= ocode;
      ocode_and &= ocode;
    }
    if(_ether_render_addvertex(
	  _ether_render_tempverts[thisone].projected_x, 
	  _ether_render_tempverts[thisone].projected_y, 
	  (long)_ether_render_tempverts[thisone].vertex[Z], 
	  _ether_render_tempverts[thisone].intensity, ocode, of, i))
      return 1;
  }
  if (ocode_or == 0) return 2;  /* needs no xy clipping */
#ifdef ENTIRELY_CLIPPED
  if (ocode_and) return 1;      /* entirely clipped */
#endif
  return 0;
}

static int
_ether_render_addvertex(long x, long y, long z, EtherFactor intensity, unsigned char ocode, EtherOutputfacet *of, int ind)
{
  EtherOutputvertex *p = _ether_memory_alloc(sizeof(EtherOutputvertex));
  if(p == NULL)
    return -1;
  p->next = of->points;  /* last vertex always points to first */
  of->points->prev = p;  /* and first always points to last */
  if(_ether_render_last_point)
  {
    p->prev = _ether_render_last_point;  /* our previous is the current last */
    _ether_render_last_point->next = p;  /* last's next is us */
  }
  else
  {
    p->prev = of->points;  /* initially, points to first one */
    _ether_render_last_point = p;         /* and we're the last one */
  }
  _ether_render_last_point = p;   /* we're now the last one */
  /* convert to window coordinates */
  x += _ether_render_window_halfwidth;  
  y += _ether_render_window_halfheight;
  p->x = x;
  p->y = y; 
  p->z = z;
  p->intensity = intensity;
  if(ind == 0)
  {
    of->minbound[X] = of->maxbound[X] = x;
    of->minbound[Y] = of->maxbound[Y] = y;
    of->minbound[Z] = of->maxbound[Z] = z;
  }
  else
  {
    if (x < of->minbound[X]) of->minbound[X] = x;
    if (y < of->minbound[Y]) of->minbound[Y] = y;
    if (z < of->minbound[Z]) of->minbound[Z] = z;
    if (x > of->maxbound[X]) of->maxbound[X] = x;
    if (y > of->maxbound[Y]) of->maxbound[Y] = y;
    if (z > of->maxbound[Z]) of->maxbound[Z] = z;
  }
  p->outcode = ocode;
  return 0;
}

/* compute the normals to the right and top clipping planes
 * (we assume symmetry for the left and bottom clipping planes)
 */

static void 
_ether_render_camera_update(EtherCameraOld *camera)
{
  EtherVector right;
  EtherVector up;
  ether_vector_create(right, ETHER_FLOAT_TO_SCALAR(camera->zoom * 10000), 0, ETHER_FLOAT_TO_SCALAR(-10000));
  ether_vector_normalize(right);
  camera->aright = right[X];
  camera->cright = right[Z];
  ether_vector_create(up, 0, ETHER_FLOAT_TO_SCALAR(camera->zoom * camera->aspect * 10000), ETHER_FLOAT_TO_SCALAR(-10000));
  ether_vector_normalize(up);
  camera->btop = up[Y];
  camera->ctop = up[Z];
}

static void 
_ether_render_horizon_poly(EtherOutputvertex *vertices, EtherColor color)
{
  EtherOutputvertex *v = vertices;
  do
  {
    v->red = color;
    v = v->next;
  } while (v != vertices);

  if (!_ether_render_display_clips)
    vertices = ether_display_xyclip_poly(vertices, _ETHER_DISPLAY_XYCLIP_CLIP_X|_ETHER_DISPLAY_XYCLIP_CLIP_Y);
  if (vertices)
    ether_display_send_command(ETHER_DISPLAY_POLY, ETHER_SURF_SIMPLE, vertices);
}

static void 
_ether_render_draw_horizon(long int x1, long int y1, long int x2, long int y2)
{
  EtherOutputvertex v[4];
  long int rise, run;
  int sky, ground;
  rise = y2 - y1;
  run  = x2 - x1;
  v[0].next = &v[1];  v[0].prev = &v[3];
  v[1].next = &v[2];  v[1].prev = &v[0];
  v[2].next = &v[3];  v[2].prev = &v[1];
  v[3].next = &v[0];  v[3].prev = &v[2];
  if(rise == 0 && run == 0)
  {
    ether_display_clear((_ether_render_viewdir[Y] < 0) ? ether_world_ground_color_get() : ether_world_sky_color_get());
    return;
  }
  if(abs((int)rise) < abs((int)run))
  {
    if (run < 0)
    {
      ground = ether_world_sky_color_get();
      sky    = ether_world_ground_color_get();
    }
    else
    {
      sky    = ether_world_sky_color_get();
      ground = ether_world_ground_color_get();
    }
    v[0].x = _ether_render_left_clip - 1 + _ether_render_window_halfwidth;
    v[0].y = y1 + (long)ether_math_scalar_mult_div((float)rise, (float)(_ether_render_left_clip - x1), (float)run) + _ether_render_window_halfheight;
    v[1].x = _ether_render_left_clip - 1 + _ether_render_window_halfwidth;
    v[1].y = -((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[2].x = _ether_render_right_clip + _ether_render_window_halfwidth;
    v[2].y = -((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[3].x = _ether_render_right_clip + _ether_render_window_halfwidth;
    v[3].y = y2 + (long)ether_math_scalar_mult_div((float)rise, (float)(_ether_render_right_clip - x2), (float)run) + _ether_render_window_halfheight;
    _ether_render_horizon_poly(v, sky);
    v[3].next = &v[2];  v[3].prev = &v[0];
    v[2].next = &v[1];  v[2].prev = &v[3];
    v[1].next = &v[0];  v[1].prev = &v[2];
    v[0].next = &v[3];  v[0].prev = &v[1];
    v[1].y = ((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[2].y = ((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    _ether_render_horizon_poly(&v[3], ground);
  }
  else /* rise > run */
  {
    if (rise < 0)
    {
      sky    = ether_world_sky_color_get();
      ground = ether_world_ground_color_get();
    }
    else
    {
      ground = ether_world_sky_color_get();
      sky    = ether_world_ground_color_get();
    }
    v[0].y = _ether_render_bottom_clip + 1 + _ether_render_window_halfheight;
    v[0].x = x1 + (long)ether_math_scalar_mult_div((float)run,  (float)(_ether_render_bottom_clip - y1), (float)rise) +  _ether_render_window_halfwidth;
    v[1].y = _ether_render_bottom_clip + 1 + _ether_render_window_halfheight;
    v[1].x = -((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[2].y = _ether_render_top_clip - 1 + _ether_render_window_halfheight;
    v[2].x = -((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[3].y = _ether_render_top_clip - 1 + _ether_render_window_halfheight;
    v[3].x = x2 + (long)ether_math_scalar_mult_div((float)run, (float)(_ether_render_top_clip - y2), (float)rise) + _ether_render_window_halfwidth;
    _ether_render_horizon_poly(v, sky);
    v[3].next = &v[2];  v[3].prev = &v[0];
    v[2].next = &v[1];  v[2].prev = &v[3];
    v[1].next = &v[0];  v[1].prev = &v[2];
    v[0].next = &v[3];  v[0].prev = &v[1];
    v[1].x = ((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    v[2].x = ((long int) 32767) << _ETHER_SCREEN_FRACT_BITS;
    _ether_render_horizon_poly(&v[3], ground);
  }
}

static void 
_ether_render_process_object(EtherObject *obj)
{
  EtherMatrix vto, wto;
  EtherVector v;
  EtherRep *rep;
  EtherLight *lgt;
  register int i;
  ++Stats.objects_processed;
#ifdef NEW_PROC
  if(_ether_render_nobjs >= _ether_render_max_objs && !_ether_process_direct)
#else
    if(_ether_render_nobjs >= _ether_render_max_objs)
#endif
    {
      _ether_render_status.objects = 1;
      return;
    }
  if(obj->invisible) return;
  if(obj->layer)
    if((_ether_render_layerbits[obj->layer >> 3] & _ether_render_bitmasks[obj->layer & 7]) == 0)
      return;
  if (obj->shape == NULL) return;
  ++Stats.objects_not_invisible;
  /* set up a pointer to our array of pointers to surfaces */
  _ether_render_current_surfmap = obj->surfmap;
  if(_ether_render_current_surfmap == NULL)
    _ether_render_current_surfmap = obj->shape->default_surfacemap;
  if(_ether_render_current_surfmap == NULL)
    return;
  /* compute object-to-view matrix */
  if(ether_object_is_fixed(obj))
    ether_matrix_copy(_ether_render_otv, _ether_render_wtv);
  else
    ether_matrix_multiply(_ether_render_otv, _ether_render_wtv, obj->globalmat);
  /* cull objects behind us or too far in front of us */
  v[Z] =  _ether_transform_z(_ether_render_otv, obj->shape->center);
  if(v[Z] + obj->shape->radius < _ether_render_hither)
    return;
  if(v[Z] - obj->shape->radius > _ether_render_yon)
    return;
  _ether_render_need_z_clipping = 1;
  if(v[Z] - obj->shape->radius > _ether_render_hither)
    _ether_render_need_z_clipping = 0;
  ++Stats.objects_not_behind;
  _ether_render_need_x_clipping = _ether_render_need_y_clipping = 1;  /* need both x and y */
  v[X] = _ether_transform_x(_ether_render_otv, obj->shape->center);
  if(_ether_render_ortho && 0)
  {  /* simpler box check */
    if (v[X] - obj->shape->radius > _ether_render_cright) return;
    if (v[X] + obj->shape->radius < -_ether_render_cright) return;
    if (v[X] - obj->shape->radius > -_ether_render_cright
	&& v[X] + obj->shape->radius < _ether_render_cright)
      _ether_render_need_x_clipping = 0;  /* don't need to clip X */
  }
  else
  {
    /* this next bit's tricky; we're substituting the object center into
       the plane equation for the clipping planes in order to find the
       distance of the point to the plane, since we know that if that
       distance is greater than the radius of the bounding sphere then
       the object can be culled
       */
    if(ether_math_factor_multiply(_ether_render_aright, v[X]) + ether_math_factor_multiply(_ether_render_cright, v[Z]) > obj->shape->radius)
      return;
    if(ether_math_factor_multiply(-_ether_render_aright, v[X]) + ether_math_factor_multiply(_ether_render_cright, v[Z]) > obj->shape->radius)
      return;
    /* now we reverse the test to see if the object is entirely *inside*
       the clipping volume; if it is, we don't need to do facet clipping
       later on.
       */
    if (ether_math_factor_multiply(-_ether_render_aright, v[X]) + ether_math_factor_multiply(-_ether_render_cright, v[Z]) > obj->shape->radius
	&& ether_math_factor_multiply(_ether_render_aright, v[X]) + ether_math_factor_multiply(-_ether_render_cright, v[Z]) > obj->shape->radius)
      _ether_render_need_x_clipping = 0;  /* don't need to clip X */
  }
  ++Stats.objects_not_leftright;
  v[Y] = _ether_transform_y(_ether_render_otv, obj->shape->center);
  if(_ether_render_ortho && 0)
  {  /* simpler box check */
    if (v[Y] - obj->shape->radius > _ether_render_ctop) return;
    if (v[Y] + obj->shape->radius < -_ether_render_ctop) return;
    if (v[Y] - obj->shape->radius > -_ether_render_ctop
	&& v[Y] + obj->shape->radius < _ether_render_ctop)
      _ether_render_need_y_clipping = 0;  /* don't need to clip Y */
  }
  else
  {
    if(ether_math_factor_multiply(_ether_render_btop, v[Y]) + ether_math_factor_multiply(_ether_render_ctop, v[Z]) > obj->shape->radius)
      return;
    if(ether_math_factor_multiply(-_ether_render_btop, v[Y]) + ether_math_factor_multiply(_ether_render_ctop, v[Z]) > obj->shape->radius)
      return;
    if(ether_math_factor_multiply(-_ether_render_btop, v[Y]) + ether_math_factor_multiply(-_ether_render_ctop, v[Z]) > obj->shape->radius
	&& ether_math_factor_multiply(_ether_render_btop, v[Y]) + ether_math_factor_multiply(-_ether_render_ctop, v[Z]) > obj->shape->radius)
      _ether_render_need_y_clipping = 0;  /* don't need to clip Y */
  }
  ++Stats.objects_not_abovebelow;
  /* select a representation (i.e., level of detail) */
  if(obj->forced_rep)  /* if there's a forced rep, use it */
    rep = obj->forced_rep;
  else if(v[Z] <= 0)   /* the object is *very* close; use first rep */
    rep = obj->shape->replist;
  else   /* search for a suitable rep */
  {
    EtherScalar r = ether_math_scalar_mult_div(
	obj->shape->radius, 
	(float)(max(_ether_render_screenscale_x, _ether_render_screenscale_y) >> _ETHER_SCREEN_FRACT_BITS), 
	v[Z]);
    rep = ether_shape_rep_get(obj->shape, r);
  }
  if(rep == NULL)
    return;
  ++Stats.objects_not_toosmall;
#ifdef ETHER_NEW_PROC
  if (!_ether_render_direct)
  {
#endif
    /* allocate a new output object */
    _ether_render_current_obj = _ether_memory_alloc(sizeof(EtherRenderOutputObject));
    if (_ether_render_current_obj == NULL) return;
    _ether_render_objarray[_ether_render_nobjs++] = _ether_render_current_obj;  /* add to array */
    _ether_render_current_obj->original = obj;
    _ether_render_current_obj->outfacets = &_ether_render_outfacets[_ether_render_nout_facets];
    _ether_render_current_obj->noutfacets = 0;
    _ether_render_current_obj->not_drawn = 1;
    /* move bounding box into world coordinates */
#ifdef OLD
    if(!ether_object_is_fixed(obj))
    {
      ether_vector_add(_ether_render_current_obj->minbound, obj->shape->minbound, obj->globalmat[_ETHER_TRANS]);
      ether_vector_add(_ether_render_current_obj->maxbound, obj->shape->maxbound, obj->globalmat[_ETHER_TRANS]);
    }
#endif
#ifdef NEW_PROC
  }
#endif
  /* if the object is highlighted, all its facets must be as well */
  _ether_render_highlight_facets = obj->highlight ? 1 : 0;
  /* find the view-to-object transform and the view in object coordinates */
  ether_matrix_inverse(vto, _ether_render_otv);
  /* a little magic here... we want to transform the view vector
     [0,0,1] into object space, but that just means selecting the third
     column of vto and adding the translations; since the third column
     of vto is the third row of _ether_render_otv, it's easy, and saves a transform! */
  if(_ether_render_ortho && 0)
  {
    ether_vector_add(_ether_render_vp, _ether_render_otv[2], vto[3]);   /* only used by _ether_render_back_facing() */
  }
  else
  {
    ether_vector_copy(_ether_render_vp, vto[3]);  /* only used by _ether_render_back_facing() */
  }
  /* initialize _ether_render_vert_array[], _ether_render_norm_array[] */
  _ether_render_vert_array = rep->vertices;
  _ether_render_norm_array = rep->normals;
  for(i = 0; i < rep->nvertices; ++i)
  {
    _ether_render_tempverts[i].intensity = 0;
    _ether_render_tempverts[i].status = TEMP_VERTEX_UNPROCESSED;
  }
  /* Find the world-to-object transform and use it to compute the position
     of each light source in object space */
  if(!ether_object_is_fixed(obj))
    ether_matrix_inverse(wto, obj->globalmat);
  _ether_render_nlights = 0;
  if(!_ether_render_wireframe)
  {
    for(lgt = _ether_render_lightlist; lgt && _ether_render_nlights < _ether_render_maxlights; lgt = lgt->next)
    {
      if(lgt->on)
      {
	_ether_render_locallights[_ether_render_nlights].type = lgt->type;
	_ether_render_locallights[_ether_render_nlights].intensity = lgt->intensity;
	if (lgt->object == NULL)   /* lights with no parents are ambient by definition */
	  _ether_render_locallights[_ether_render_nlights].type = ETHER_LIGHT_AMBIENT;
	else if (ether_object_is_fixed(lgt->object))   /* same for lights attached to fixed objects */
	  _ether_render_locallights[_ether_render_nlights].type = ETHER_LIGHT_AMBIENT;
	/* if we're a fixed object, the light's world position is what we use */
	else if (ether_object_is_fixed(obj))
	  ether_matrix_copy(_ether_render_locallights[_ether_render_nlights].mat, lgt->object->globalmat);
	else
	  ether_matrix_multiply(_ether_render_locallights[_ether_render_nlights].mat, wto, lgt->object->globalmat);
	++_ether_render_nlights;
      }
    }
  }
  /* process the facets */
#ifdef NEW_PROC
  if(_ether_process_direct)
  {
    Ether_Facet *f;
    new_process_object(obj, rep);
    for (f = rep->facets; f; f = f->farside)
      new_process_facet(f);
  }
  else
#endif
    if(rep->sorttype == ETHER_SORT_OTHER)  /* if they're in a BSP tree, use it */
      _ether_render_bsp_process_facet(rep->facets);
    else  /* otherwise, just copy them over; only farside is used */
    {
      EtherFacet *f;
      /* this next bit becomes two consecutive for() loops eventually,
	 to support inside and outside facet lists */
      for(f = rep->facets; f; f = f->farside)
      {
	++Stats.facets_processed;
	if (!_ether_render_back_facing(f))
	  _ether_render_process_facet(f, NULL);
      }
      if(_ether_render_facet_sorting)
	switch (rep->sorttype)
	{
	  case ETHER_SORT_NEAREST:
	    qsort(_ether_render_current_obj->outfacets, _ether_render_current_obj->noutfacets, sizeof(EtherOutputfacet *), _ether_render_facet_sort_nearest);
	    break;
	  case ETHER_SORT_FARTHEST:
	    qsort(_ether_render_current_obj->outfacets, _ether_render_current_obj->noutfacets, sizeof(EtherOutputfacet *), _ether_render_facet_sort_farthest);
	    break;
	  case ETHER_SORT_AVERAGE:
	  case ETHER_SORT_NONE:
	  case ETHER_SORT_OTHER:
	  default: break;
	}
    }
}

static void 
_ether_render_draw(EtherRenderOutputObject *obj)
{
  register int i;
  EtherRenderOutputObject *p;
  obj->not_drawn = 0;             /* remove from consideration */
  for(i = _ether_render_first; i < _ether_render_nobjs; ++i)  /* draw all obscured objects */
  {
    p = _ether_render_objarray[i];
    if (p->not_drawn)
      if (_ether_render_obscures(obj, p))
        _ether_render_draw(p);
  }

  _ether_render_draw_obj(obj);    /* draw this object */
}

static int 
_ether_render_obscures(EtherRenderOutputObject *a, EtherRenderOutputObject *b)
{
  int i, overlap = 0;
  //Ether_Matrix tmpmat;
  //Ether_Vector tmpvect, tmpvect2, tmpvp;
  for(i = 0; i < 3; ++i)
  {
    /* check if b is on our side of a (either min or max sides) */
    if (_ether_render_viewpoint[i] <= a->original->minbound[i]
	&& b->original->maxbound[i] <= a->original->minbound[i])
      return 0;
    if (_ether_render_viewpoint[i] >= a->original->maxbound[i]
	&& b->original->minbound[i] >= a->original->maxbound[i])
      return 0;  /* viewpoint and b both greater than a's maxbound */
    if (_ether_render_viewpoint[i] >= b->original->minbound[i]
	&& a->original->maxbound[i] < b->original->minbound[i])
      return 0;
    if (_ether_render_viewpoint[i] <= b->original->maxbound[i]
	&& a->original->minbound[i] > b->original->maxbound[i])
      return 0;
    /* check if a and b overlap in this axis */
    if (b->original->maxbound[i] > a->original->minbound[i]
	&& b->original->minbound[i] < a->original->maxbound[i])
      ++overlap;
  }
  if(overlap == 3)  /* bounding boxes overlap */
    for(i = 0; i < 3; ++i)
    {
      /* check if centroid of b is on our side of a */
      if (_ether_render_viewpoint[i] <= a->original->minbound[i]
	  && ((b->original->maxbound[i]+b->original->minbound[i])/2) < a->original->minbound[i])
	return 0;
      if (_ether_render_viewpoint[i] >= a->original->maxbound[i]
	  && ((b->original->maxbound[i]+b->original->minbound[i])/2) >= a->original->minbound[i])
	return 0;
    }
  return 1;   /* otherwise assume that a obscures b */
}

static void
_ether_render_monitor(EtherOutputfacet *of)
{
  int c = 0;
  EtherOutputvertex *p = of->points, *q;
  long int xi, xj, yi, yj;
  if(p)
  {
    do
    {
      if(p->next)
	q = p->next;
      else
	q = of->points;
      xi = q->x >> _ETHER_SCREEN_FRACT_BITS;
      xj = p->x >> _ETHER_SCREEN_FRACT_BITS;
      yi = q->y >> _ETHER_SCREEN_FRACT_BITS;
      yj = p->y >> _ETHER_SCREEN_FRACT_BITS;
      if(((yi <= _ether_render_monitor_y) && (_ether_render_monitor_y < yj)) || 
	  ((yj <= _ether_render_monitor_y) && (_ether_render_monitor_y < yi)))
      {
	int r = (int)(ether_math_scalar_mult_div((EtherScalar)xj - xi, (EtherScalar)(_ether_render_monitor_y - yi), (EtherScalar)(yj - yi)) + xi);
	if(_ether_render_monitor_x < r)
	  c = !c;
      }
      p = p->next;
    } while (p != of->points);
  }
  if (c)
    _ether_render_monitor_facet = of;
}

static int
_ether_render_show_facet(EtherOutputfacet *of)
{
  EtherOutputfacet *detail;
  int stat = 0;
  int xyclip = of->xclip | of->yclip;
  unsigned int flags;
  short int ourcolor;
  EtherOutputvertex *vertices = of->points, *v;

  /* one vertex => point */
  if(vertices->next == vertices)
  {
    if(xyclip && !_ether_render_display_clips)
      vertices = ether_display_xyclip_point(vertices, xyclip);
    if(vertices == NULL)
      return 0;
    if(of->highlight)
      vertices->red = _ether_render_highlight_color;
    else if(_ether_render_wireframe)
      vertices->red = _ether_render_wireframe_color;
    else
      vertices->red = ether_display_compute_color(of->surface, vertices->intensity, _ether_render_ambient, (EtherScalar)vertices->z);
    stat = ether_display_send_command(ETHER_DISPLAY_POINT, 0, vertices);
  }
  else if(vertices->next->next == vertices)  /* two vertices => line */
  {
    if(xyclip && !_ether_render_display_clips)
      vertices = ether_display_xyclip_line(vertices, xyclip);
    if(vertices == NULL)
      return 0;
    if(of->highlight)
      vertices->red = vertices->next->red = _ether_render_highlight_color;
    else if(_ether_render_wireframe)
      vertices->red = vertices->next->red = _ether_render_wireframe_color;
    else
    {
      vertices->red = ether_display_compute_color(of->surface, vertices->intensity, _ether_render_ambient, (EtherScalar)vertices->z);
      vertices->next->red = ether_display_compute_color(of->surface, vertices->next->intensity, _ether_render_ambient, (EtherScalar)vertices->next->z);
    }
    stat = ether_display_send_command(ETHER_DISPLAY_LINE, 0, vertices);
  }
  else /* more than two vertices => actual polygon */
  {
    if(xyclip && !_ether_render_display_clips) {
      flags = _ETHER_GOURAUD_SHADING(of->surface) ? _ETHER_DISPLAY_XYCLIP_INTENSITY : 0;
      vertices = ether_display_xyclip_poly(vertices, flags | xyclip);
    }
    if(vertices == NULL)
      return 0;
    of->points = vertices;
    if(_ether_render_wireframe)
    {
      ourcolor = of->highlight ? _ether_render_highlight_color : _ether_render_wireframe_color;
      v = vertices;
      do
      {
        v->red = ourcolor;
        v = v->next;
      } while (v != vertices);
      ether_display_send_command(ETHER_DISPLAY_CLOSED_LINE, of->surface->type, vertices);
    }
    else
    {
      if(_ETHER_GOURAUD_SHADING(of->surface))
      {
        v = vertices;
        do
        {
          ether_display_compute_vertex_color(v, of->surface, v->intensity, _ether_render_ambient, (EtherScalar)of->maxbound[Z]);
          v = v->next;
        } while (v != vertices);
      }
      else  /* flat shading */
        of->color = ether_display_compute_color(of->surface, of->intensity, _ether_render_ambient, (EtherScalar)of->maxbound[Z]);
      ether_display_send_command(ETHER_DISPLAY_POLY, 0, of);
      if(of->highlight)
      {
        v = vertices;
        do
        {
          v->red = _ether_render_highlight_color;
          v = v->next;
        } while (v != vertices);
        ether_display_send_command(ETHER_DISPLAY_CLOSED_LINE, of->surface->type, vertices);
      }
    }
  }
  if(_ether_render_monitoring)
    _ether_render_monitor(of);
  for(detail = of->details; detail && !stat; detail = detail->next)
    stat = _ether_render_show_facet(detail);
  return stat;
}

static void
_ehter_render_show_bbox(EtherRenderOutputObject *obj)
{
  EtherVector m[8], s[8];
  int i;
  /* define the vertices of the object's bounding box: */
  m[0][X] = m[3][X] = m[4][X] = m[7][X] = obj->original->minbound[X];
  m[1][X] = m[2][X] = m[5][X] = m[6][X] = obj->original->maxbound[X];
  m[0][Y] = m[1][Y] = m[2][Y] = m[3][Y] = obj->original->minbound[Y];
  m[4][Y] = m[5][Y] = m[6][Y] = m[7][Y] = obj->original->maxbound[Y];
  m[0][Z] = m[1][Z] = m[4][Z] = m[5][Z] = obj->original->minbound[Z];
  m[2][Z] = m[3][Z] = m[6][Z] = m[7][Z] = obj->original->maxbound[Z];
  for(i = 0; i < 8; ++i)
  {
    _ether_transform(s[i], _ether_render_wtv, m[i]);
    /* don't divide by zero! */
    if(s[i][Z] < _ether_render_hither)
      return;  
    s[i][X] = ether_math_scalar_mult_div(s[i][X], (EtherScalar)_ether_render_screenscale_x, s[i][Z]) + _ether_render_window_halfwidth;
    s[i][X] += _ether_render_horizontal_shift;
    s[i][Y] = ether_math_scalar_mult_div(s[i][Y], (EtherScalar)_ether_render_screenscale_y, s[i][Z]) + _ether_render_window_halfheight;
  }

  ether_display_line((int)s[0][X], (int)s[0][Y], (int)s[1][X], (int)s[1][Y], _ether_render_bbox_color);
  ether_display_line((int)s[1][X], (int)s[1][Y], (int)s[2][X], (int)s[2][Y], _ether_render_bbox_color);
  ether_display_line((int)s[2][X], (int)s[2][Y], (int)s[3][X], (int)s[3][Y], _ether_render_bbox_color);
  ether_display_line((int)s[3][X], (int)s[3][Y], (int)s[0][X], (int)s[0][Y], _ether_render_bbox_color);

  ether_display_line((int)s[4][X], (int)s[4][Y], (int)s[5][X], (int)s[5][Y], _ether_render_bbox_color);
  ether_display_line((int)s[5][X], (int)s[5][Y], (int)s[6][X], (int)s[6][Y], _ether_render_bbox_color);
  ether_display_line((int)s[6][X], (int)s[6][Y], (int)s[7][X], (int)s[7][Y], _ether_render_bbox_color);
  ether_display_line((int)s[7][X], (int)s[7][Y], (int)s[4][X], (int)s[4][Y], _ether_render_bbox_color);

  ether_display_line((int)s[0][X], (int)s[0][Y], (int)s[4][X], (int)s[4][Y], _ether_render_bbox_color);
  ether_display_line((int)s[1][X], (int)s[1][Y], (int)s[5][X], (int)s[5][Y], _ether_render_bbox_color);
  ether_display_line((int)s[2][X], (int)s[2][Y], (int)s[6][X], (int)s[6][Y], _ether_render_bbox_color);
  ether_display_line((int)s[3][X], (int)s[3][Y], (int)s[7][X], (int)s[7][Y], _ether_render_bbox_color);
}

static void 
_ether_render_draw_obj(EtherRenderOutputObject *obj)
{
  register int j;
  EtherOutputfacet **ofs = obj->outfacets;
  if(obj->original->contents)
  {
    void *p;
    int tnf, tno;
    if(!obj->original->invisible)
    {
      /* display inside facets */
      for(j = 0; j < obj->noutfacets; ++j)
      {
        EtherOutputfacet *of = ofs[j];
        if (of->original->interior)
          _ether_render_show_facet(of);
      }
    }
    /* "push" the noutfacets and nobjs variables, mark memory */
    tnf = _ether_render_nout_facets;
    tno = _ether_render_nobjs;
    p = _ether_memory_mark();
    /* recursively process object's contents */
    ether_render_objlist(obj->original->contents);
    /* "pop" the variables and release the mark */
    _ether_memory_release(p);
    _ether_render_nobjs = tno;
    _ether_render_nout_facets = tnf;
    if(!obj->original->invisible)
    {
      /* display outside facets */
      for(j = 0; j < obj->noutfacets; ++j)
      {
        EtherOutputfacet *of = ofs[j];
        if (!(of->original->interior))
          _ether_render_show_facet(of);
      }
    }
  }
  else
  {
    for(j = 0; j < obj->noutfacets; ++j)  /* display facets */
      _ether_render_show_facet(ofs[j]);
  }
  if(_ether_render_show_bb)
    _ehter_render_show_bbox(obj);
}

static EtherFactor 
_ether_render_compute_lighting(EtherVector normal, EtherVector vertex)
{
  EtherFactor intensity = 0;
  EtherFactor lightscale = ether_math_scalar_divide(1, (float)_ether_render_nlights);
  /* lightscale is the amount by which to scale the intensity of
     each source, so that the total doesn't exceed ETHER_UNITY. */
  EtherVector tmpvec;
  int i;
  for(i = 0; i < _ether_render_nlights; ++i)
  {
    EtherLocalLight *lgt = &_ether_render_locallights[i];
    EtherFactor inten = ether_math_factor_multiply(lightscale, lgt->intensity);
    /* inten is the fraction of the total maximum intensity that this
       light is producing */
    switch(lgt->type)
    {
      case ETHER_LIGHT_AMBIENT:
	break;
#ifdef WORKING
      case ETHER_LIGHT_POINTSOURCE:
	/* find vector from light to this vertex */
	ether_vector_sub(tmpvec, lgt->mat[TRANS], vertex);
	ether_vector_normalize(tmpvec);
	/* scale the intensity of the light by the angle to the source */
	inten = ether_math_factor_multiply(inten, -ether_vector_dotproduct(tmpvec, normal));
	break;
#else
      case ETHER_LIGHT_POINTSOURCE:   /* for now, treat them all as directional */
#endif
      case ETHER_LIGHT_DIRECTIONAL:
	tmpvec[X] = lgt->mat[X][Z];
	tmpvec[Y] = lgt->mat[Y][Z];
	tmpvec[Z] = lgt->mat[Z][Z];
	/* scale the intensity of the light by the angle to the direction of the light */
	inten = ether_math_factor_multiply(inten, -ether_vector_dotproduct(tmpvec, normal));
	break;
      default:   /* unknown light type */
	inten = 0;
	break;
    }
    if(inten > ETHER_UNITY)
      inten = ETHER_UNITY;  /* clip */
    if(inten > 0)
      intensity += inten;
  }
  if(intensity < 0)
    intensity = 0;
  else if(intensity > ETHER_UNITY)
    intensity = ETHER_UNITY;
  return intensity;
}

static int 
_ether_render_back_facing(EtherFacet *facet)
{
  EtherVector v;
  EtherScalar result;
  if(facet->npoints < 3)
    return 0; /* points or lines */
  if(_ether_render_ortho && 0)
    result = ether_vector_dotproduct(_ether_render_vp, facet->normal);
  else
  {
    ether_vector_sub(v, _ether_render_vp, _ether_render_vert_array[facet->points[0]]);
    result = ether_vector_dotproduct(v, facet->normal);
  }
  return result < 0;
}

static void  
_ether_render_process_facet(EtherFacet *facet, EtherOutputfacet *parent)
{
  EtherOutputfacet *of  = NULL;  /* output facet */
  EtherFacet *detail    = NULL;  /* points to details on current facet */
  EtherFactor intensity = 0;     /* total amount of light hitting this facet */
  int i;
  EtherSurface *surf = ether_surface_map_surface_get(_ether_render_current_surfmap, facet->surface);
  ++Stats.facets_not_backfacing;

  /* handle lighting calculations for flat shading */
  if(!_ether_render_wireframe 
      && (surf->type == ETHER_SURF_FLAT 
        || surf->type == ETHER_SURF_METAL 
        || surf->type == ETHER_SURF_GLASS))
  {
    if (parent)
      intensity = parent->intensity;
    else
      intensity = _ether_render_compute_lighting(facet->normal, _ether_render_vert_array[facet->points[0]]);
    ++Stats.facets_flatshaded;
  }
  if(!_ether_render_need_z_clipping)
  {
    ++Stats.facets_not_behind;
    for (i = 0; i < facet->npoints; ++i)
    {
      int vertnum = facet->points[i];
      register EtherTempVertex *tv = &_ether_render_tempverts[vertnum];
      ++Stats.vertices_zchecked;  
      ++Stats.vertices_xychecked;
      if (tv->status < TEMP_VERTEX_XY_DONE)  /* not already z-transformed */
      {
	_ether_transform(tv->vertex, _ether_render_otv, _ether_render_vert_array[vertnum]);
	tv->status = TEMP_VERTEX_XY_DONE;
	++Stats.vertices_ztransformed;
	++Stats.vertices_xytransformed;
      }
      if (_ether_render_norm_array && _ETHER_GOURAUD_SHADING(surf))
      {
	tv->intensity = _ether_render_compute_lighting(_ether_render_norm_array[vertnum], tv->vertex);
	++Stats.facets_gouraud_shaded;
      }
      else
	tv->intensity = intensity;  /* use facet's intensity */
    }
  }
  else
  {
    /* transform Z components of vertices, keeping track of maxdepth */
    EtherScalar maxdepth = _ether_render_hither - 1;  /* if nothing closer than this, skip this facet */
    for (i = 0; i < facet->npoints; ++i)
    {
      //Ether_Scalar z;
      int vertnum = facet->points[i];
      register EtherTempVertex *tv = &_ether_render_tempverts[vertnum];
      ++Stats.vertices_zchecked;
      if (tv->status < TEMP_VERTEX_Z_DONE)  /* not already z-transformed */
      {
	tv->vertex[Z] = _ether_transform_z(_ether_render_otv, _ether_render_vert_array[vertnum]);
	tv->status = TEMP_VERTEX_Z_DONE;
	++Stats.vertices_ztransformed;
      }
      if (tv->vertex[Z] > maxdepth) maxdepth = tv->vertex[Z];
    }
    if (maxdepth < _ether_render_hither) return;  /* entirely behind us */
    ++Stats.facets_not_behind;
    /* transform X and Y components of the vertices */
    for (i = 0; i < facet->npoints; ++i)
    {
      int vertnum = facet->points[i];
      register EtherTempVertex *tv = &_ether_render_tempverts[vertnum];
      ++Stats.vertices_xychecked;
      if (tv->status >= TEMP_VERTEX_XY_DONE) continue;  /* already XY-transformed */
      tv->vertex[X] = _ether_transform_x(_ether_render_otv, _ether_render_vert_array[vertnum]);
      tv->vertex[Y] = _ether_transform_y(_ether_render_otv, _ether_render_vert_array[vertnum]);
      tv->status = TEMP_VERTEX_XY_DONE;
      ++Stats.vertices_xytransformed;
      if (_ether_render_norm_array && _ETHER_GOURAUD_SHADING(surf))
      {
	tv->intensity = _ether_render_compute_lighting(_ether_render_norm_array[vertnum], tv->vertex);
	++Stats.facets_gouraud_shaded;
      }
      else
	tv->intensity = intensity;  /* use facet's intensity */
    }
  }
  /* create a new output facet */
  of = _ether_memory_alloc(sizeof(EtherOutputfacet));
  if(of == NULL) 
    return;  /* out of memory */
  of->xclip = _ether_render_need_x_clipping ? _ETHER_DISPLAY_XYCLIP_CLIP_X : 0;
  of->yclip = _ether_render_need_y_clipping ? _ETHER_DISPLAY_XYCLIP_CLIP_Y : 0;
  if(of->xclip) 
    ++Stats.facets_need_x_clipping;
  if(of->yclip) 
    ++Stats.facets_need_y_clipping;
  of->highlight = (facet->highlight | _ether_render_highlight_facets) ? 1 : 0;
  of->surface   = surf;       /* surface descriptor */
  of->intensity = intensity;
  of->details   = NULL;       /* no detail facets (yet) */
  of->original  = facet;      /* point back at original facet */
  of->outobj    = _ether_render_current_obj; /* point to this output facet's output object */
  _ether_render_first_point    = _ether_memory_alloc(0);
  of->points    = _ether_render_first_point; /* points to vertices */
  if(_ether_render_first_point == NULL)
    return;
  _ether_render_last_point = NULL;
  if(_ether_render_need_z_clipping)
    switch(_ether_render_zclip(facet, of))
    {
      case 2: of->xclip = of->yclip = 0;  /* no xy clipping needed */
      case 0: break;   /* needs xy clipping */
      case 1: return;  /* entirely xy clipped */
    }
  else
    switch(_ether_render_no_zclip(facet, of))
    {
      case 2: of->xclip = of->yclip = 0;  /* no xy clipping needed */
      case 0: break;   /* needs xy clipping */
      case 1: return;  /* entirely xy clipped */
    }
  if(parent)  /* we're a detail facet; attach us to our new parent */
  {
    of->details = parent->details;
    parent->details = of;
    return;  /* details don't have details */
  }		
  else  /* we need to be put into the outfacets[] array */
  {
    if(_ether_render_nout_facets >= _ether_render_max_facets)
    {
      _ether_render_status.facets = 1;
      return;  /* table full */
    }
    _ether_render_outfacets[_ether_render_nout_facets++] = of;
    ++Stats.not_toomanyfacets;
    ++_ether_render_current_obj->noutfacets;
  }
  /* now process our detail facets; should save this for "showtime" */
  for(detail = facet->details; detail; detail = detail->farside)
  {
    _ether_render_process_facet(detail, of);
    ++Stats.detail_facets;
  }
}

static void  
_ether_render_bsp_process_facet(EtherFacet *facet)
{
  if(_ether_render_back_facing(facet))
  {
    if(facet->nearside)
      _ether_render_bsp_process_facet(facet->nearside);
    if(facet->farside)
      _ether_render_bsp_process_facet(facet->farside);
  }
  else
  {
    if(facet->farside)
      _ether_render_bsp_process_facet(facet->farside);
    _ether_render_process_facet(facet, NULL);
    if(facet->nearside)
      _ether_render_bsp_process_facet(facet->nearside);
  }
}

static int   
_ether_render_facet_sort_nearest(const void *a, const void *b)
{
#define f1 (*((EtherOutputfacet **) a))
#define f2 (*((EtherOutputfacet **) b))
    
  if(f1->minbound[Z] < f2->minbound[Z])
    return 1;
  if(f1->minbound[Z] > f2->minbound[Z])
    return -1;
  return 0;
}

static int   
_ether_render_facet_sort_farthest(const void *a, const void *b)
{
#define f1 (*((EtherOutputfacet **) a))
#define f2 (*((EtherOutputfacet **) b))
    
  if(f1->maxbound[Z] < f2->maxbound[Z]) 
    return 1;
  if(f1->maxbound[Z] > f2->maxbound[Z]) 
    return -1;
  return 0;
}

#ifdef LAYER_SUPPORT
static void 
_ether_render_layer_on(int n)
{
  _ether_render_layerbits[n >> 3] |= _ether_render_bitmasks[n & 7];
}

static void 
_ether_render_layer_off(int n)
{
  _ether_render_layerbits[n >> 3] &= ~_ether_render_bitmasks[n & 7];
}

static void 
_ether_render_layer_toggle(int n)
{
  _ether_render_layerbits[n >> 3] ^= _ether_render_bitmasks[n & 7];
}

static int 
_ether_render_layer_is_on(int n)
{
  return _ether_render_layerbits[n >> 3] & _ether_render_bitmasks[n & 7];
}

static void 
_ether_render_layer_all_on(void)
{
  memset(_ether_render_layerbits, 0xFFFF, sizeof(_ether_render_layerbits));
}

static void 
_ether_render_layer_all_off(void)
{
  memset(_ether_render_layerbits, 0x0000, sizeof(_ether_render_layerbits));
}
#endif
