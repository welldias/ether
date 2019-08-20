#include <Ether.h>
#include "ether_private.h"
#include <ctype.h>

/* maximum number of *unique* surface descriptors per object: */
#define _ETHER_PLG_MAX_SURFS_PER_SHAPE 200
#define _ETHER_PLG_DEFAULT_SURFDESC    0x0001

static int              _ether_plg_nlines = 0;
static int              _ether_plg_multi;          /* set while reading multi-rep PLG files */
static char            *_ether_plg_seps = " \t";   /* strtok() field separators are spaces and tabs */
static float            _ether_plg_sx = 1;
static float            _ether_plg_sy = 1;
static float            _ether_plg_sz = 1;
static float            _ether_plg_tx = 0;
static float            _ether_plg_ty = 0;
static float            _ether_plg_tz = 0;
static FILE            *_ether_plg_outfile;
static EtherSurfaceMap *_ether_plg_map;

static int           _ether_plg_line_get               (char *buff, int n, FILE *stream);
static unsigned int  _ether_plg_map_ascii_surface_name (char *buff);
static int           _ether_plg_output_vertex          (EtherVector *v, EtherVector *n);
static int           _ether_plg_output_facet           (EtherFacet *facet);
static int           _ether_plg_output_rep             (EtherRep *rep);

void
ether_plg_scale_set(float x, float y, float z)
{
  _ether_plg_sx = x;
  _ether_plg_sy = y;
  _ether_plg_sz = z;
}

void
ether_plg_offset_set(float x, float y, float z)
{
  _ether_plg_tx = x;
  _ether_plg_ty = y;
  _ether_plg_tz = z;
}

EtherShape *
ether_plg_read(FILE *in)
{
  char buff[100];
  int i;
  int nsurfs_defined = 0;
  int got_header     = 0;
  EtherRep     *lastrep = NULL;
  EtherSurface *tempsurfs[_ETHER_PLG_MAX_SURFS_PER_SHAPE];
  EtherShape   *shape = NULL;
  _ether_plg_nlines  = 0;	
  _ether_plg_multi   = 0;
  while(_ether_plg_line_get(buff, sizeof(buff), in))
  {
    int nv, nf;
    char name[100], *p;
    EtherFacet *parent = NULL;
    EtherRep *rep;
    got_header = 1;
    if(shape == NULL)
    {
      shape = ether_shape_create();
      if (shape == NULL)
        return NULL;
    }
    rep = malloc(sizeof(EtherRep));
    if(rep == NULL)
      return NULL;
    rep->next = NULL;
    if(lastrep)
      lastrep->next = rep;
    else
      shape->replist = rep;
    lastrep = rep;
    if(sscanf(buff, "%s %d %d %*d %d", name, &nv, &nf, &rep->sorttype) < 5)
    {
      if(nf < 6)   /* objects with fewer than 6 facets are almost always convex */
        rep->sorttype = ETHER_SORT_NONE;
      else
        rep->sorttype = ETHER_SORT_FARTHEST;
    }
    rep->size = 0;
    if((p = strchr(name, '_')) != NULL)
      if(isdigit((int)p[1]))
        rep->size = (float)atoi(&p[1]);
    rep->nvertices = nv;
    rep->vertices = calloc(nv, sizeof(EtherVector));
    if(rep->vertices == NULL)
      return NULL;
    rep->edges = NULL;
    rep->normals = NULL;
    rep->facets = NULL;
    for(i = 0; i < nv; ++i)
    {
      float x, y, z, nx, ny, nz;
      _ether_plg_line_get(buff, sizeof(buff), in);
      if(sscanf(buff, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz) == 6)
      {
        if(rep->normals == NULL)
          rep->normals = calloc(nv, sizeof(EtherVector));
        if(rep->normals)
        {
          rep->normals[i][X] = ETHER_FLOAT_TO_FACTOR(nx);
          rep->normals[i][Y] = ETHER_FLOAT_TO_FACTOR(ny);
          rep->normals[i][Z] = ETHER_FLOAT_TO_FACTOR(nz);
        }
      }
      rep->vertices[i][X] = ETHER_FLOAT_TO_SCALAR(x * _ether_plg_sx + _ether_plg_tx);
      rep->vertices[i][Y] = ETHER_FLOAT_TO_SCALAR(y * _ether_plg_sy + _ether_plg_ty);
      rep->vertices[i][Z] = ETHER_FLOAT_TO_SCALAR(z * _ether_plg_sz + _ether_plg_tz);
    }
    for(i = 0; i < nf; ++i)
    {
      char *ptr = NULL;
      int j, detail = 0;
      unsigned int surfvalue;
      EtherFacet *facet = malloc(sizeof(EtherFacet));
      if(facet == NULL)
        return NULL;
      _ether_plg_line_get(buff, sizeof(buff), in);
      if(isdigit(buff[0]))
        surfvalue = (unsigned int)strtoul(buff, &ptr, 0);
      else
        surfvalue = _ether_plg_map_ascii_surface_name(buff);
      facet->id = 0;
      if(surfvalue & 0x8000)  /* indexed color */
        facet->surface = surfvalue & 0x7FFF;
      else
      {
        EtherSurface surf;
        ether_surface_from_desc((unsigned short)surfvalue, &surf);  /* convert value to surface */
        for(j = 0; j < nsurfs_defined; ++j)  /* seen one of these before? */
          if(surf.type == tempsurfs[j]->type && 
              surf.hue == tempsurfs[j]->hue  && 
              surf.brightness == tempsurfs[j]->brightness)
            break;
        facet->surface = j;
        if(j >= nsurfs_defined)  /* new one... add to tempsurfs[] */
        {
          EtherSurface *s = malloc(sizeof(EtherSurface));
          if(s == NULL)
            return NULL;
          ether_surface_init(s);
          tempsurfs[nsurfs_defined++] = s;
          s->type = surf.type;
          s->hue = surf.hue;
          s->brightness = surf.brightness;
        }
      }
      facet->highlight = facet->interior = 0;
      facet->details   = NULL;
      facet->nearside  = facet->farside = NULL;
      facet->npoints   = (int)strtoul(ptr, &ptr, 0);
      facet->points    = calloc(facet->npoints, sizeof(int));
      facet->edges     = NULL;
      if(facet->points == NULL)
        return NULL;
      for(j = 0; j < facet->npoints; ++j)
        facet->points[facet->npoints-1-j] = (int)strtoul(ptr, &ptr, 0);
      ether_facet_compute_normal(facet, rep->vertices);
      p = strtok(ptr, _ether_plg_seps);
      while(p)
      {
        if(*p == 'i')
          facet->interior = 1;
        if(*p == 'd')
          detail = 1;
        /* for now, ignore other stuff, like Nfacet#, Ffacet#, Cr,g,b,i Ttexture */
        p = strtok(NULL, _ether_plg_seps);
      }
      if(detail && parent)  /* if we're a detail facet attach us to our parent*/
      {
        facet->farside = parent->details;
        parent->details = facet;
      }
      else /* otherwise, attach us to the rep's facet list */
      {
        facet->farside = rep->facets;
        rep->facets = facet;
      }
      if(!detail)
        parent = facet;  /* if we're not a detail, we're a (potenial) parent */
    }
    if(!_ether_plg_multi)
      break;
  }
  if(!_ether_plg_multi && !got_header)
    return NULL;
  if(nsurfs_defined){
    shape->default_surfacemap = ether_surface_map_create(nsurfs_defined);
  }
  if(shape->default_surfacemap) {
    for(i = 0; i < nsurfs_defined; ++i)
      ether_surface_map_surface_set(shape->default_surfacemap, i, tempsurfs[i]);
  }

  ether_shape_compute_bounds(shape);
  return shape;
}

int
ether_plg_write(EtherShape *shape, FILE *out)
{
  int n;
  if(shape == NULL)
    return -1;
  n = ether_shape_count_reps(shape);
  if(n == 0)
    return -2;
  if(n > 1)
    fprintf(out, "##MULTI\n");
  _ether_plg_map = ether_shape_surface_map_get(shape);
  _ether_plg_outfile = out;
  ether_shape_traverse_reps(shape, _ether_plg_output_rep);
  return 0;
}

EtherObject *
ether_plg_load_object(char *filename)
{
  EtherObject *obj;
  FILE *in = fopen(_ether_wld_file_fixup_filename(filename), "r");
  if(in == NULL)
    return NULL;
  obj = ether_object_load_from_plg(in);
  fclose(in);
  return obj;
}

int ether_plg_save_object(EtherObject *object, char *filename)
{
  FILE *out;
  EtherShape *shape;
  if(object == NULL && filename == NULL)
    return -1;
  shape = object->shape;
  if(shape == NULL)
    return -2;
  out = fopen(filename, "r");
  if (out == NULL)
    return -1;
  ether_plg_write(shape, out);
  fclose(out);
  return 0;
}

static int _ether_plg_line_get(char *buff, int n, FILE *stream)
{
  char *p;
  do
  {
    if(fgets(buff, n, stream) == NULL)
      return 0;
    ++_ether_plg_nlines;
    if((p = strchr(buff, '\n')) != NULL) 
      *p = '\0';  /* strip newline */
    if(!strcasecmp(buff, "##MULTI")) 
      _ether_plg_multi = 1;
    if((p = strchr(buff, '#')) != NULL) 
      *p = '\0';   /* strip comments */
    for(p = buff; *p && isspace((int)*p); ++p);
  } while(*p == '\0');
  return 1;
}

static unsigned int _ether_plg_map_ascii_surface_name(char *buff)
{
  char *p = strtok(buff, "_");
  unsigned char hue, brightness;
  if(p == NULL)
    return _ETHER_PLG_DEFAULT_SURFDESC;  /* no string found; return color 1 */
  if(!strcasecmp(p, "indexed"))
  {
    p = strtok(NULL, "_");
    if(p) 
      return 0x8000 | (int)strtoul(p, NULL, 0);
    return _ETHER_PLG_DEFAULT_SURFDESC;
  }
  if(!strcasecmp(p, "shaded"))
  {
    p = strtok(NULL, "_");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    hue = (unsigned char)strtoul(p, NULL, 0) & 0x0F;
    p = strtok(NULL, "");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    brightness = (unsigned char)strtoul(p, NULL, 0) & 0xFF;
    return 0x1000 | (hue << 8) | brightness;
  }
  if(!strcasecmp(p, "metal"))
  {
    p = strtok(NULL, "_");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    hue = (unsigned char)strtoul(p, NULL, 0) & 0x0F;
    p = strtok(NULL, "");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    brightness = (unsigned char)strtoul(p, NULL, 0) & 0xFF;
    return 0x2000 | (hue << 8) | ((brightness & 0x1F) << 3);
  }
  if(!strcasecmp(p, "glass"))
  {
    p = strtok(NULL, "_");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    hue = (unsigned char)strtoul(p, NULL, 0) & 0x0F;
    p = strtok(NULL, "");
    if(p == NULL)
      return _ETHER_PLG_DEFAULT_SURFDESC;
    brightness = (unsigned char)strtoul(p, NULL, 0) & 0xFF;
    return 0x3000 | (hue << 8) | ((brightness & 0x1F) << 3);
  }
  return _ETHER_PLG_DEFAULT_SURFDESC;
}

static int
_ether_plg_output_vertex(EtherVector *v, EtherVector *n)
{
  fprintf(_ether_plg_outfile, 
      "%ld %ld %ld", 
      (long)ETHER_SCALAR_TO_FLOAT((*v)[X]), 
      (long)ETHER_SCALAR_TO_FLOAT((*v)[Y]), 
      (long)ETHER_SCALAR_TO_FLOAT((*v)[Z]));
  if(n)
    fprintf(_ether_plg_outfile,
	"%ld %ld %ld",
	(long) ETHER_SCALAR_TO_FLOAT((*n)[X]),
	(long) ETHER_SCALAR_TO_FLOAT((*n)[Y]),
	(long) ETHER_SCALAR_TO_FLOAT((*n)[Z]));
  fprintf(_ether_plg_outfile, "\n");
  return 0;
}

static int
_ether_plg_output_facet(EtherFacet *facet)
{
  EtherSurface *surf = ether_surface_map_surface_get(_ether_plg_map, ether_facet_surf_num_get(facet));
  int np = ether_facet_count_points(facet);
  int i;
  fprintf(_ether_plg_outfile, "0x%x %d", ether_surface_to_desc(surf), np);
  for(i = 0; i < np; ++i)
    fprintf(_ether_plg_outfile, " %d", ether_facet_point_get(facet, i));
  fprintf(_ether_plg_outfile, "\n");
  return 0;
}

static int
_ether_plg_output_rep(EtherRep *rep)
{
  fprintf(_ether_plg_outfile, 
      "rep_%ld %d %d %d %d\n",
      (long) ETHER_SCALAR_TO_FLOAT(ether_rep_size_get(rep)),
      ether_rep_count_vertices(rep),
      ether_rep_count_facets(rep),
      0,
      ether_rep_sorting_get(rep));
  ether_rep_traverse_vertices(rep, _ether_plg_output_vertex);
  ether_rep_traverse_facets(rep, _ether_plg_output_facet);
  return 0;
}
