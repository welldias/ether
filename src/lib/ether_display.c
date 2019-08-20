#include <Ether.h>
#include "ether_private.h"

static unsigned char _ether_display_current_palette[256*3];
static char          _ether_display_current_desc[100];
static char          _ether_video_current_desc[100];
static int           _ether_display_viewpage          = 0;
static int           _ether_display_drawpage          = 0;
static int           _ether_display_npages            = 1;
static int           _ether_display_depth             = 8;
//static int           _ether_display_in_graphics_mode  = 0;

extern EtherDisplayDriverFunction  ether_drivers_display_default;
extern EtherVideoDriverFunction    ether_drivers_video_default;
extern EtherDisplayDriverFunction  ether_allegro_display_default;
extern EtherVideoDriverFunction    ether_allegro_video_default;

EtherDisplayDriverFunction        *_ether_display_current_driver  =  NULL;
EtherVideoDriverFunction          *_ether_video_current_driver    = NULL;

static void _ether_display_real_palette_set  (int start, int end, void *palette);
static void _ether_display_real_window_set   (int x1, int y1, int x2, int y2);
static void _ether_display_display_reset     (void);

/* XY Clip */
#define _ETHER_DISPLAY_XYCLIP_MAXPOINTS 20
static EtherOutputvertex _ether_display_xyclip_a1[100];
static EtherOutputvertex _ether_display_xyclip_a2[100];
static int                _ether_display_xyclip_left;
static int                _ether_display_xyclip_right;
static int                _ether_display_xyclip_top;
static int                _ether_display_xyclip_bottom;

void
ether_display_driver_set(EtherDisplayDriverFunction *driver)
{
	_ether_display_current_driver = driver;
}

int
ether_display_init(EtherRaster *raster)
{
  int n;
#if 0
  if(_ether_video_current_driver == NULL)
    _ether_video_current_driver = ether_drivers_video_default;
  if(_ether_display_current_driver == NULL)
    _ether_display_current_driver = ether_drivers_display_default;
#else
  if(_ether_video_current_driver == NULL)
    _ether_video_current_driver = ether_allegro_video_default;
  if(_ether_display_current_driver == NULL)
    _ether_display_current_driver = ether_allegro_display_default;
#endif

  n = _ether_display_current_driver(ETHER_DISPLAY_INIT, 0, raster);
  if(n)
    return n;
  _ether_display_display_reset();
  return 0;
}

void
ether_display_quit(void)
{
	(*_ether_display_current_driver)(ETHER_DISPLAY_QUIT, 0, NULL);
}

void
ether_display_clear(EtherColor color)
{
	(*_ether_display_current_driver)(ETHER_DISPLAY_CLEAR, color, NULL);
}

void
ether_display_point(int x, int y, EtherColor color)
{
	EtherOutputvertex v;
	v.x = x;
	v.y = y;
	v.z = 1;
	v.x <<= _ETHER_SCREEN_FRACT_BITS;  
	v.y <<= _ETHER_SCREEN_FRACT_BITS;
	v.next = v.prev = &v;
	v.red = color;
	(*_ether_display_current_driver)(ETHER_DISPLAY_POINT, ETHER_SURF_SIMPLE, &v);
}

void
ether_display_line(int x1, int y1, int x2, int y2, EtherColor color)
{
	EtherOutputvertex v1, v2, *v;
	v1.x = x1;
	v1.y = y1;
	v1.z = 1;
	v2.x = x2;
	v2.y = y2;
	v2.z = 1;
	v1.x <<= _ETHER_SCREEN_FRACT_BITS;
	v1.y <<= _ETHER_SCREEN_FRACT_BITS;
	v2.x <<= _ETHER_SCREEN_FRACT_BITS;
	v2.y <<= _ETHER_SCREEN_FRACT_BITS;
	v1.red  = v2.red  = color;
	v1.next = v1.prev = &v2;
	v2.prev = v2.next = &v1;
	v = ether_display_xyclip_poly(&v1, _ETHER_DISPLAY_XYCLIP_CLIP_X|_ETHER_DISPLAY_XYCLIP_CLIP_Y);
	if (v)
		(*_ether_display_current_driver)(ETHER_DISPLAY_LINE, ETHER_SURF_SIMPLE, v);
}

void
ether_display_box(int x1, int y1, int x2, int y2, EtherColor color)
{
	EtherOutputvertex v1, v2;
	v1.x = x1;  
	v1.y = y1;  
	v1.z = 1;
	v2.x = x2;  
	v2.y = y2;  
	v2.z = 1;
	v1.x <<= _ETHER_SCREEN_FRACT_BITS;
	v1.y <<= _ETHER_SCREEN_FRACT_BITS;
	v2.x <<= _ETHER_SCREEN_FRACT_BITS;
	v2.y <<= _ETHER_SCREEN_FRACT_BITS;
	v1.red = v2.red = color;
	v1.next = v1.prev = &v2;  
	v2.prev = v2.next = &v1;
	(*_ether_display_current_driver)(ETHER_DISPLAY_BOX, ETHER_SURF_SIMPLE, &v1);
}

void
ether_display_text(int x, int y, EtherColor color, char *message)
{
	(*_ether_display_current_driver)(ETHER_DISPLAY_TEXT_POSITION, ((x << 16) | y), NULL);
	(*_ether_display_current_driver)(ETHER_DISPLAY_TEXT, color, message);
}

int
ether_display_text_width_get(char *string)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_GET_TEXTWIDTH, 0, string);
}

int
ether_display_text_height_get(char *string)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_GET_TEXTHEIGHT, 0, string);
}

int
ether_display_sort_order_get(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_GET_SORTORDER, 0, NULL);
}

int
ether_display_can_gouraud(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_CAN_GOURAUD, 0, NULL);
}

int
ether_display_can_xyclip(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_CAN_XY_CLIP, 0, NULL);
}

int
ether_display_version_get(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_GET_VERSION, 0, NULL);
}

char *
ether_display_description_get(void)
{
	strcpy(_ether_display_current_desc, "Unknown display driver");
	(*_ether_display_current_driver)(ETHER_DISPLAY_GET_DESCRIPTION, sizeof(_ether_display_current_desc)-2, _ether_display_current_desc);
	return _ether_display_current_desc;
}

int
ether_display_begin_frame(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_BEGIN_FRAME, 0, NULL);
}

int
ether_display_end_frame(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_END_FRAME, 0, NULL);
}

int
ether_display_raster_set(EtherRaster *raster)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_SET_RASTER, 0, raster);
}

EtherRaster *
ether_display_raster_get(void)
{
	EtherRaster *raster = NULL;
	(*_ether_display_current_driver)(ETHER_DISPLAY_GET_RASTER, 0, &raster);
	return raster;
}

int
ether_display_zbuffer_set(EtherRaster *raster)
{ 
	return (*_ether_display_current_driver)(ETHER_DISPLAY_SET_Z_BUFFER, 0,(raster));
}

int
ether_display_zbuffer_get(void)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_GET_Z_BUFFER, 0, NULL);
}

int
ether_display_clear_zbuffer(int depth)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_CLEAR_Z_BUFFER, depth, NULL);
}

int
ether_display_use_zbuffer(int flag)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_USE_Z_BUFFER, flag, NULL);
}

int
ether_display_update_palette(EtherPalette *palette)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_UPDATE_PALETTE, 0, palette);
}

int
ether_display_shading_set(int value)
{
	return (*_ether_display_current_driver)(ETHER_DISPLAY_SET_SHADING,(value), NULL);
}

EtherColor
ether_display_compute_color(EtherSurface *surf, EtherFactor intensity, EtherFactor ambient, EtherScalar depth)
{
	EtherHue *huemap = ether_palette_huemap_get(ether_world_palette_get());
	EtherHue *hueptr = &huemap[surf->hue];
	int value;
	if(ambient)
		intensity = ether_math_scalar_mult_div(intensity + ambient, 1, 2);  /* should use a coefficient */
	if(intensity < 0)
		intensity = 0;
	if(intensity > ETHER_UNITY)
		intensity = ETHER_UNITY;
	if(depth == 4)
	{
		if (surf->type == ETHER_SURF_SIMPLE)
			return (surf->brightness >> 4) & 0x0F;
		return surf->hue & 0x0F;
	}
	/* else depth == 8 */
	switch(surf->type)
	{
		case ETHER_SURF_SIMPLE: 
			return surf->brightness;
		case ETHER_SURF_METAL:
		case ETHER_SURF_GLASS:
			return (EtherColor)(hueptr->start + ether_math_factor_multiply(intensity, hueptr->maxshade));
		case ETHER_SURF_FLAT:
		case ETHER_SURF_GOURAUD:
		case ETHER_SURF_SPECULAR:
			value = (int)(ether_math_factor_multiply(intensity, (float)(surf->brightness * hueptr->maxshade)));
			return (EtherColor)hueptr->start + ((value + 128) >> 8);  /* round up */
		default:
			break;
	}
	return 1;  /* by default, return color 1 (blue in most palettes) */
}

void
ether_display_compute_vertex_color(EtherOutputvertex *v, EtherSurface *surf, EtherFactor intensity, EtherFactor ambient, EtherScalar depth)
{
	EtherHue *huemap = ether_palette_huemap_get(ether_world_palette_get());
	EtherHue *hueptr = &huemap[surf->hue];
	int value;
	if(ambient)
		intensity = ether_math_scalar_mult_div(intensity + ambient, 1, 2);  /* should use a coefficient */
	if(intensity < 0)
		intensity = 0;
	if(intensity > ETHER_UNITY) 
		intensity = ETHER_UNITY;
	if(depth == 4)
	{
		if(surf->type == ETHER_SURF_SIMPLE)
			v->red = (surf->brightness >> 4) & 0x0F;
		else
			v->red = surf->hue & 0x0F;
		return;
	}
	/* else depth == 8 */
	switch (surf->type)
	{
		case ETHER_SURF_SIMPLE:
			v->red = surf->brightness; 
			return;
		case ETHER_SURF_METAL:
		case ETHER_SURF_GLASS:
			v->red = (EtherColor)(hueptr->start + ether_math_factor_multiply(intensity, hueptr->maxshade));
			break;
		case ETHER_SURF_FLAT:
		case ETHER_SURF_GOURAUD:
		case ETHER_SURF_SPECULAR:
			value  = (int)ether_math_factor_multiply(intensity, (float)(surf->brightness * (hueptr->maxshade-1)));
			v->red = (hueptr->start << 8) + value + 256;
			return;
		default:
			break;
	}
	v->red = 1;  /* by default, return color 1 (blue in most palettes) */
	return;
}

void
ether_display_window_set(int x1, int y1, int x2, int y2)
{
	_ether_display_real_window_set(x1, y1, x2, y2);
}

void
ether_display_window_get(int *x1, int *y1, int *x2, int *y2)
{
	ether_raster_window_get(ether_display_raster_get(), x1, y1, x2, y2);
}

int
ether_display_width_get(void)
{
	return ether_raster_width_get(ether_display_raster_get());
}

int
ether_display_height_get(void)
{
	return ether_raster_height_get(ether_display_raster_get());
}
int
ether_display_depth_get(void)
{
	return ether_raster_depth_get(ether_display_raster_get());
}

int
ether_display_update(void)
{
	return ether_video_blit(ether_display_raster_get());
}

void
ether_display_xyclip_set(int left, int top, int right, int bottom)
{
	_ether_display_xyclip_left   = 0;
	_ether_display_xyclip_right  = ((int)(right-left)) << _ETHER_SCREEN_FRACT_BITS;
	_ether_display_xyclip_top    = 0;
	_ether_display_xyclip_bottom = ((int)(bottom-top)) << _ETHER_SCREEN_FRACT_BITS;;
}

int  ether_display_send_command(EtherDisplayCommand cmd, int lparm, void *pparm1)
{
	return (*_ether_display_current_driver)(cmd, lparm, pparm1);
}

/* Video functions  */
void
ether_video_driver_set(EtherVideoDriverFunction *driver)
{
	_ether_video_current_driver = driver;
}

int
ether_video_mode_get(void)
{
	return(*_ether_video_current_driver)(ETHER_VIDEO_GET_MODE, 0, NULL);
}

void
ether_video_draw_page_set(int page)
{
	_ether_display_drawpage = page;
	(*_ether_video_current_driver)(ETHER_VIDEO_SET_DRAW_PAGE, page, NULL);
}

int
ether_video_draw_page_get(void)
{
	return _ether_display_drawpage;
}

void
ether_video_view_page_set(int page)
{
	_ether_display_viewpage = page;
	(*_ether_video_current_driver)(ETHER_VIDEO_SET_VIEW_PAGE, page, NULL);
}

int
ether_video_view_page_get(void)
{
	return _ether_display_viewpage;
}

int
ether_video_npages_get(void)
{
	int n = _ether_display_npages;
	return n;
}

int
ether_video_has_palette(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_HAS_PALETTE, 0, NULL);
}

void
ether_video_palette_set(int start, int end, EtherPalette *palette)
{
	memcpy(&_ether_display_current_palette[3*start], &palette->data[3*start], 3*(end - start + 1));
	_ether_display_real_palette_set(start, end, palette->data);
	ether_display_update_palette(palette);
}

void
ether_video_palette_get(int start, int end, EtherPalette *palette)
{
	memcpy(&palette->data[3*start], &_ether_display_current_palette[3*start], 3*(end - start + 1));
}

int
ether_video_ntsc_set(int flag)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_SET_NTSC, flag, NULL);
}

int
ether_video_check_retrace(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_CHECK_RETRACE, 0, NULL);
}

EtherRaster  *
ether_video_raster_get(void)
{
	EtherRaster *raster = NULL;
	(*_ether_video_current_driver)(ETHER_VIDEO_GET_RASTER, 0, &raster);
	return raster;
}

int
ether_video_blit(EtherRaster *buffer)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_BLIT, 0, (buffer));
}

int
ether_video_version_get(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_GET_VERSION, 0, NULL);
}

char *
ether_video_description_get(void)
{
	strcpy(_ether_video_current_desc, "Unknown video driver");
	(*_ether_video_current_driver)(ETHER_VIDEO_GET_DESCRIPTION, sizeof(_ether_video_current_desc)-2, _ether_video_current_desc);
	return _ether_video_current_desc;
}

int
ether_video_cursor_hide(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_CURSOR_HIDE, 0, NULL);
}

int 
ether_video_cursor_show(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_CURSOR_SHOW, 0, NULL);
}

int 
ether_video_cursor_reset(void)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_CURSOR_RESET, 0, NULL);
}

int
ether_video_cursor_move(int x, int y)
{
	return (*_ether_video_current_driver)(ETHER_VIDEO_CURSOR_MOVE, (x << 16) | (y & 0xFFFF), NULL);
}

/* XY clipping functions */
EtherOutputvertex *
ether_display_xyclip_point(EtherOutputvertex *vertices, unsigned int flags)
{
	return (
		vertices->x < _ether_display_xyclip_left  || 
		vertices->y < _ether_display_xyclip_top   || 
		vertices->x > _ether_display_xyclip_right || 
		vertices->y > _ether_display_xyclip_bottom) ? NULL : vertices;
}

EtherOutputvertex *
ether_display_xyclip_line(EtherOutputvertex *vertices, unsigned int flags)
{
	return ether_display_xyclip_poly(vertices, flags);
}

EtherOutputvertex *
ether_display_xyclip_poly(EtherOutputvertex *vertices, unsigned int flags)
{
	EtherOutputvertex *src = _ether_display_xyclip_a2;
	EtherOutputvertex *dst = _ether_display_xyclip_a1;
	EtherOutputvertex *v1, *v2, *v = vertices;
	int nout = 0, i, npts = 0;
	do {
		src[npts++] = *v;
		v = v->next;
	} while(v != vertices && npts < _ETHER_DISPLAY_XYCLIP_MAXPOINTS);

	v1 = &src[npts-1];
	for(i = 0; i < npts; ++i)
	{
		v2 = &src[i];
		if(v1->x >= _ether_display_xyclip_left && v2->x >= _ether_display_xyclip_left)  /* on-screen */
			dst[nout++] = src[i];
		else if(v1->x < _ether_display_xyclip_left && v2->x < _ether_display_xyclip_left)  /* off-screen */
			;
		else if(v1->x >= _ether_display_xyclip_left && v2->x < _ether_display_xyclip_left)   /* leaving */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_left - v2->x);
			EtherScalar denom = (EtherScalar)(v1->x - v2->x);
			dst[nout].x = _ether_display_xyclip_left;
			dst[nout].y = (long)(v2->y + ether_math_scalar_mult_div((float)(v1->y - v2->y), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v2->z + ether_math_scalar_mult_div((float)(v1->z - v2->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v2->intensity + ether_math_scalar_mult_div(v1->intensity - v2->intensity, numer, denom);
			else
				dst[nout].intensity = v2->intensity;
			++nout;
		}
		else  /* entering */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_left - v1->x);
			EtherScalar denom = (EtherScalar)(v2->x - v1->x);
			dst[nout].x = _ether_display_xyclip_left;
			dst[nout].y = (long)(v1->y + ether_math_scalar_mult_div((float)(v2->y - v1->y), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v1->z + ether_math_scalar_mult_div((float)(v2->z - v1->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v1->intensity + ether_math_scalar_mult_div(v2->intensity - v1->intensity, numer, denom);
			else
				dst[nout].intensity = v1->intensity;
			++nout;
			dst[nout++] = src[i];
		}
		v1 = v2;
	}

	if (dst == _ether_display_xyclip_a1)
	{ 
		dst = _ether_display_xyclip_a2; 
		src = _ether_display_xyclip_a1; 
	}
	else
	{ 
		dst = _ether_display_xyclip_a1; 
		src = _ether_display_xyclip_a2;
	}
	npts = nout;
	nout = 0;
	v1 = &src[npts-1];
	for (i = 0; i < npts; ++i)
	{
		v2 = &src[i];
		if(v1->x <= _ether_display_xyclip_right && v2->x <= _ether_display_xyclip_right)  /* on-screen */
			dst[nout++] = src[i];
		else if(v1->x > _ether_display_xyclip_right && v2->x > _ether_display_xyclip_right)  /* off-screen */
			;
		else if(v1->x <= _ether_display_xyclip_right && v2->x > _ether_display_xyclip_right)   /* leaving */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_right - v2->x);
			EtherScalar denom = (EtherScalar)(v1->x - v2->x);
			dst[nout].x = _ether_display_xyclip_right;
			dst[nout].y = (long)(v2->y + ether_math_scalar_mult_div((float)(v1->y - v2->y), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v2->z + ether_math_scalar_mult_div((float)(v1->z - v2->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v2->intensity + ether_math_scalar_mult_div(v1->intensity - v2->intensity, numer, denom);
			else
				dst[nout].intensity = v2->intensity;
			++nout;
		}
		else  /* entering */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_right - v1->x);
			EtherScalar denom = (EtherScalar)(v2->x - v1->x);
			dst[nout].x = _ether_display_xyclip_right;
			dst[nout].y = (long)(v1->y + ether_math_scalar_mult_div((float)(v2->y - v1->y), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v1->z + ether_math_scalar_mult_div((float)(v2->z - v1->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v1->intensity + ether_math_scalar_mult_div(v2->intensity - v1->intensity, numer, denom);
			else
				dst[nout].intensity = v1->intensity;
			++nout;
			dst[nout++] = src[i];
		}
		v1 = v2;
	}

	if(dst == _ether_display_xyclip_a1)
	{ 
		dst = _ether_display_xyclip_a2; 
		src = _ether_display_xyclip_a1;
	}
	else
	{
		dst = _ether_display_xyclip_a1; 
		src = _ether_display_xyclip_a2;
	}
	npts = nout;
	nout = 0;
	v1 = &src[npts-1];
	for (i = 0; i < npts; ++i)
	{
		v2 = &src[i];
		if(v1->y >= _ether_display_xyclip_top && v2->y >= _ether_display_xyclip_top)  /* on-screen */
			dst[nout++] = src[i];
		else if(v1->y < _ether_display_xyclip_top && v2->y < _ether_display_xyclip_top)  /* off-screen */
			;
		else if(v1->y >= _ether_display_xyclip_top && v2->y < _ether_display_xyclip_top)   /* leaving */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_top - v2->y);
			EtherScalar denom = (EtherScalar)(v1->y - v2->y);
			dst[nout].x = (long)(v2->x + ether_math_scalar_mult_div((float)(v1->x - v2->x), numer, denom));
			dst[nout].y = _ether_display_xyclip_top;
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v2->z + ether_math_scalar_mult_div((float)(v1->z - v2->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v2->intensity + ether_math_scalar_mult_div(v1->intensity - v2->intensity, numer, denom);
			else
				dst[nout].intensity = v2->intensity;
			++nout;
		}
		else  /* entering */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_top - v1->y);
			EtherScalar denom = (EtherScalar)(v2->y - v1->y);
			dst[nout].x = (long)(v1->x + ether_math_scalar_mult_div((float)(v2->x - v1->x), numer, denom));
			dst[nout].y = _ether_display_xyclip_top;
			if(flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v1->z + ether_math_scalar_mult_div((float)(v2->z - v1->z), numer, denom));
			if(flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v1->intensity + ether_math_scalar_mult_div(v2->intensity - v1->intensity, numer, denom);
			else
				dst[nout].intensity = v1->intensity;
			++nout;
			dst[nout++] = src[i];
			}
		v1 = v2;
	}

	if(dst == _ether_display_xyclip_a1)
	{ 
		dst = _ether_display_xyclip_a2; 
		src = _ether_display_xyclip_a1; 
	}
	else
	{ 
		dst = _ether_display_xyclip_a1; 
		src = _ether_display_xyclip_a2; 
	}
	npts = nout;
	nout = 0;
	v1 = &src[npts-1];
	for (i = 0; i < npts; ++i)
	{
		v2 = &src[i];
		if(v1->y <= _ether_display_xyclip_bottom && v2->y <= _ether_display_xyclip_bottom)  /* on-screen */
			dst[nout++] = src[i];
		else if(v1->y > _ether_display_xyclip_bottom && v2->y > _ether_display_xyclip_bottom)  /* off-screen */
			;
		else if(v1->y <= _ether_display_xyclip_bottom && v2->y > _ether_display_xyclip_bottom)   /* leaving */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_bottom - v2->y);
			EtherScalar denom = (EtherScalar)(v1->y - v2->y);
			dst[nout].x = (long)(v2->x + ether_math_scalar_mult_div((float)(v1->x - v2->x), numer, denom));
			dst[nout].y = _ether_display_xyclip_bottom;
			if (flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v2->z + ether_math_scalar_mult_div((float)(v1->z - v2->z), numer, denom));
			if (flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v2->intensity + ether_math_scalar_mult_div(v1->intensity - v2->intensity, numer, denom);
			else
				dst[nout].intensity = v2->intensity;
			++nout;
		}
		else  /* entering */
		{
			EtherScalar numer = (EtherScalar)(_ether_display_xyclip_bottom - v1->y);
			EtherScalar denom = (EtherScalar)(v2->y - v1->y);
			dst[nout].x = (long)(v1->x + ether_math_scalar_mult_div((float)(v2->x - v1->x), numer, denom));
			dst[nout].y = _ether_display_xyclip_bottom;
			if (flags & _ETHER_DISPLAY_XYCLIP_Z)
				dst[nout].z = (long)(v1->z + ether_math_scalar_mult_div((float)(v2->z - v1->z), numer, denom));
			if (flags & _ETHER_DISPLAY_XYCLIP_INTENSITY)
				dst[nout].intensity = v1->intensity + ether_math_scalar_mult_div(v2->intensity - v1->intensity, numer, denom);
			else
				dst[nout].intensity = v1->intensity;
			++nout;
			dst[nout++] = src[i];
		}
		v1 = v2;
	}
	{
		int i;
		for (i = 0; i < nout; ++i)
		{
			dst[i].next = &dst[i+1];
			dst[i].prev = &dst[i-1];
		}
		if (nout)
		{
			dst[nout-1].next = &dst[0];
			dst[0].prev = &dst[nout-1];
		}
	}
	return nout ? dst : NULL;
}


static void 
_ether_display_real_palette_set(int start, int end, void *palette)
{
	(*_ether_video_current_driver)(ETHER_VIDEO_SET_PALETTE, (((int)start) << 16) | end, palette);
}

static void 
_ether_display_real_window_set(int x1, int y1, int x2, int y2)
{
	ether_raster_window_set(ether_display_raster_get(), x1, y1, x2, y2);
	ether_display_xyclip_set(x1, y1, x2, y2);
}

static void 
_ether_display_display_reset(void)
{
	EtherPalette* palette = NULL;
	_ether_display_viewpage = 0;
	_ether_display_drawpage = 0;
	_ether_display_depth    = ether_display_depth_get();
	_ether_display_npages   = (*_ether_video_current_driver)(ETHER_VIDEO_GET_NPAGES, 0, NULL);
	if(ether_display_depth_get() == 4)
	{
		palette = ether_palette_default_palette_16_get();
		memcpy(_ether_display_current_palette, palette->data, 48);
		_ether_display_real_palette_set(0, 15, &palette->data);
		ether_display_update_palette(palette);
	}
	else  /* assume 8 */
	{
		palette = ether_palette_default_palette_256_get();
		memcpy(_ether_display_current_palette, palette->data, 256);
		_ether_display_real_palette_set(0, 255, &palette->data);
		ether_display_update_palette(palette);
	}
	ether_display_window_set(0, 0, ether_display_width_get()-1, ether_display_height_get()-1);
}
