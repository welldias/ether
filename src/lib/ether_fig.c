#define _CRT_SECURE_NO_WARNINGS

#include <Ether.h>
#include "ether_private.h"
#include <ctype.h>


#define match(a, b) (!strncasecmp((a), (b), strlen(b)))

static int _ether_fig_process_attribute(char *buff, EtherObject *obj, char *rootname);
/*
   static char *_ether_fig_err_msgs[] = 
   { 
   "No error", 
   "Couldn't open plg file", 
   "Error reading plg file", 
   "Bad syntax", 
   NULL
   };
   */
static int            _ether_fig_error           = 0;
static EtherObject **_ether_fig_part_array      = NULL;
static int            _ether_fig_part_array_size = 0;
static float          _ether_fig_xscale          = 1;
static float          _ether_fig_yscale          = 1;
static float          _ether_fig_zscale          = 1;

void
ether_fig_part_array_set(EtherObject **ptr, int maxparts)
{
  _ether_fig_part_array = ptr;
  _ether_fig_part_array_size = maxparts;
}

void
ether_fig_scale_set(float x, float y, float z)
{
  _ether_fig_xscale = x;
  _ether_fig_yscale = y;
  _ether_fig_zscale = z;
}

EtherObject *
ether_fig_read(FILE *in, EtherObject *parent, char *rootname)
{
  char buff[256];
  int c, i = 0;
  EtherObject *obj = ether_object_create(NULL);
  if(obj == NULL)
    return NULL;
  if(parent)
    ether_object_attach(obj, parent);
  while((c = getc(in)) != EOF)
  {
    switch (c)
    {
    case '#':   /* ignore comments */
      while((c = getc(in)) != EOF)
        if (c == '\n')
          break;
        break;
    case '{':
      ether_fig_read(in, obj, rootname);
      break;
    case '}':
      return obj;
    case ';':
      buff[i] = '\0';
      _ether_fig_process_attribute(buff, obj, rootname);
      i = 0;
      break;
    default:
      if(i < sizeof(buff)-1)
        buff[i++] = c;
      break;
    }
  }
  return obj;
}

EtherObject *
ether_fig_load(char *filename)
{
  FILE *in = fopen(_ether_wld_file_fixup_filename(filename), "r");
  EtherObject *obj;
  if(in == NULL)
    return NULL;
  obj = ether_fig_read(in, NULL, NULL);
  fclose(in);
  return obj;
}

static int 
_ether_fig_process_attribute(char *buff, EtherObject *obj, char *rootname)
{
  while(isspace((int)*buff))
    ++buff;
  if(match(buff, "plgfile"))
  {
    FILE *in;
    char  filename[100];
    float sx = 1, sy = 1, sz = 1;
    long  tx = 0, ty = 0, tz = 0;
    EtherShape *shape;
    sscanf(buff, "plgfile = %s scale %f,%f,%f shift %ld,%ld,%ld", filename, &sx, &sy, &sz, &tx, &ty, &tz);
    ether_plg_scale_set(sx  *  _ether_fig_xscale, sy * _ether_fig_yscale, sz * _ether_fig_zscale);
    ether_plg_offset_set(tx * _ether_fig_xscale, ty * _ether_fig_yscale, tz * _ether_fig_zscale);
    if((in = fopen(_ether_wld_file_fixup_filename(filename), "r")) == NULL)
      return _ether_fig_error = -1;
    if((shape = ether_plg_read(in)) == NULL)
    {
      fclose(in);
      return _ether_fig_error = -2;
    }
    fclose(in);
    ether_object_shape_set(obj, shape);
    return _ether_fig_error = 0;
  }
  else if(match(buff, "pos"))
  {
    long tx, ty, tz;
    sscanf(buff, "pos = %ld,%ld,%ld", &tx, &ty, &tz);
    ether_object_move(obj, 
      ETHER_FLOAT_TO_SCALAR(tx*_ether_fig_xscale), 
      ETHER_FLOAT_TO_SCALAR(ty*_ether_fig_yscale), 
      ETHER_FLOAT_TO_SCALAR(tz*_ether_fig_zscale));
  }
  else if (match(buff, "rot"))
  {
    float rx, ry, rz;
    sscanf(buff, "rot = %f,%f,%f", &rx, &ry, &rz);
    ether_object_rot_y(obj, ETHER_FLOAT_TO_ANGLE(ry));
    ether_object_rot_x(obj, ETHER_FLOAT_TO_ANGLE(rx));
    ether_object_rot_z(obj, ETHER_FLOAT_TO_ANGLE(rz));
  }
  else if (match(buff, "name"))
  {
    char *p, partname[100];
    if((p = strchr(buff, '=')) == NULL)
    {
      _ether_fig_error = -3;
      return -3;
    }
    do ++p;
    while(isspace(*p));
    if(rootname)
    {
      sprintf(partname, "%s.%s", rootname, p);
      ether_object_name_set(obj, strdup(partname));
    }
  }
  else if(match(buff, "segnum"))
  {
    int n;
    sscanf(buff, "segnum = %d", &n);
    if(_ether_fig_part_array && n < _ether_fig_part_array_size)
      _ether_fig_part_array[n] = obj;
  }
  else if(match(buff, "partnum"))
  {
    int n;
    sscanf(buff, "partnum = %d", &n);
    if(_ether_fig_part_array && n < _ether_fig_part_array_size)
      _ether_fig_part_array[n] = obj;
  }
  /* ignore anything we don't understand */
  return 0;
}
