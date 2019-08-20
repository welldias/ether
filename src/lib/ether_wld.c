#include <Ether.h>
#include "ether_private.h"
#include <ctype.h>

#define _ETHER_WLD_DEFAULT_SURFACEMAP_ENTRIES 10

typedef struct _EtherWldListnode EtherWldList;

struct _EtherWldListnode
{
	char *name;
	void *value;
	EtherWldList *next;
};

enum _ether_wld_statements
{
	ST_OBJECT,      ST_POLYOBJ,     ST_POLYOBJ2, 
	ST_CAMERA,      ST_HITHER,      ST_YON,
	ST_LIGHT,       ST_MLIGHT,      ST_AMBIENT, 
	ST_INCLUDE,     ST_LOADPATH,    ST_ANGLESTEP, 
	ST_SPACESTEP,   ST_STEPSIZE,    ST_FLYMODE,
	ST_SCREENCOLOR, ST_SCREENCLEAR, ST_SKYCOLOR, 
	ST_GROUNDCOLOR, ST_PALETTE,     ST_SURFACEDEF,
	ST_SURFACEMAP,  ST_SURFACE,     ST_USEMAP, 
	ST_ATTACH,      ST_DETACH,      ST_ATTACHVIEW, 
	ST_POSITION,    ST_ROTATE,      ST_SEGMENT,
	ST_FIGURE,      ST_OPTIONS,     ST_TITLE, 
	ST_WORLDSCALE,  ST_VERSION,
	NCMDS
};

static char *_ether_wld_statement_table[] =
{
	"object",       "polyobj",      "polyobj2", 
	"camera",       "hither",       "yon",
	"light",        "mlight",       "ambient", 
	"include",      "loadpath",     "anglestep", 
	"spacestep",    "stepsize",     "flymode",
	"screencolor",  "screenclear",  "skycolor", 
	"groundcolor",  "palette",      "surfacedef", 
	"surfacemap",   "surface",      "usemap", 
	"attach",       "detach",       "attachview", 
	"position",     "rotate",       "segment",
	"figure",       "options",      "title", 
	"worldscale",   "version"
};

/* These all "accumlate" as world files are read */
static char               _ether_wld_loadpath[100];              /* initialized to a null string */
static char               _ether_wld_title[20][80];              /* title to display at startup */
static int                _ether_wld_titlerows          = 0;     /* number of rows in title */
static EtherWldList    *_ether_wld_named_surfaces     = NULL;  /* list of named surface definitions */
static EtherWldList    *_ether_wld_named_surfacemaps  = NULL;  /* list of named surface maps */
static EtherSurfaceMap *_ether_wld_currmap            = NULL;  /* points to current map */
static int                _ether_wld_currmap_maxentries = 0;     /* number of entries allocated in current map */
static EtherSurface      _ether_wld_default_surface    = { ETHER_SURF_SIMPLE, 15, 128 };
static EtherScalar       _ether_wld_default_hither     = ETHER_FLOAT_TO_SCALAR(10);
static EtherScalar       _ether_wld_default_yon        = ETHER_FLOAT_TO_SCALAR(1000000000L);
static EtherCamera      *_ether_wld_most_recent_camera = NULL;
static char              *_ether_wld_options            = "";    /* set by "options" statement */

static int     _ether_wld_getline        (char *buff, int maxbytes, FILE *in);
static int     _ether_wld_parse          (char *buff, char *seps, char *argv[]);
static int     _ether_wld_lookup         (char *token, char *table[], int n);
static void   *_ether_wld_add_to_list    (EtherWldList **list, char *name, void *value);
static void   *_ether_wld_find_on_list   (EtherWldList *list, char *name);
static int     _ether_wld_create_object  (int argc, char *argv[]);
static int     _ether_wld_create_polyobj (int argc, char *argv[], int nsides);
static int     _ether_wld_create_segment (int argc, char *argv[]);
static int     _ether_wld_create_figure  (int argc, char *argv[]);
static int     _ether_wld_create_light   (int argc, char *argv[]);
static int     _ether_wld_create_camera  (int argc, char *argv[]);

void 
_ether_wld_file_set_load_path(char *path)
{
	if (path)
		strcpy(_ether_wld_loadpath, path);
	else
		_ether_wld_loadpath[0] = '\0';
}

char *
_ether_wld_file_fixup_filename(char *filename)
{
	static char fixup_filename[100];
	if(*filename == '\0' || *filename == '\\' || *filename == '/' || _ether_wld_loadpath[0] == '\0')
		return filename;
#ifdef WIN32
	sprintf(fixup_filename, "%s\\%s", _ether_wld_loadpath, filename);
#else
	sprintf(fixup_filename, "%s/%s", _ether_wld_loadpath, filename);
#endif
	return fixup_filename;
}

int 
ether_wld_read(FILE *in)
{
	char buff[256];
	EtherCamera *cam;
	while(_ether_wld_getline(buff, sizeof(buff), in))
		ether_wld_process_line(buff);
	cam = ether_world_find_camera("1");
	if(cam)
		ether_world_camera_set(cam);
	return 0;
}

int 
ether_wld_load(char *filename)
{
	int retvalue;
	FILE *in;
  
  in = fopen(_ether_wld_file_fixup_filename(filename), "r");
 	if (in == NULL)
		return 0;
	retvalue = ether_wld_read(in);
	fclose(in);
	return retvalue;
}

int 
ether_wld_process_line(char *buff)
{
	int argc;
	char buffcopy[256];
	char *argv[20];
	strcpy(buffcopy, buff); /* unparsed version for "title" statement */
	argc = _ether_wld_parse(buff, " \t,", argv);
	if(argc < 1) /* ignore blank lines */
		return 0;
	if(argc < 2)  /* for now, all statements have at least one argument */
		return 0;
	switch(_ether_wld_lookup(argv[0], _ether_wld_statement_table, NCMDS))
	{
		case ST_OBJECT: 
			_ether_wld_create_object(argc, argv); 
			break;
		case ST_POLYOBJ: 
			_ether_wld_create_polyobj(argc, argv, 1); 
			break;
		case ST_POLYOBJ2: 
			_ether_wld_create_polyobj(argc, argv, 2); 
			break;
		case ST_SEGMENT: 
			_ether_wld_create_segment(argc, argv); 
			break;
		case ST_FIGURE: 
			_ether_wld_create_figure(argc, argv); 
			break;
		case ST_LIGHT:  /* fall through... same as mlight */
		case ST_MLIGHT: 
			_ether_wld_create_light(argc, argv); 
			break;
		case ST_CAMERA: 
			_ether_wld_create_camera(argc, argv); 
			break;
		case ST_AMBIENT: 
			ether_world_ambient_set(ETHER_FLOAT_TO_FACTOR(atof(argv[1])/128.0)); 
			break;
		case ST_WORLDSCALE: 
			ether_world_scale_set(ETHER_FLOAT_TO_SCALAR(atof(argv[1]))); 
			break;
		case ST_LOADPATH: 
			_ether_wld_file_set_load_path(argv[1]);
			break;
		case ST_ANGLESTEP: 
			ether_world_turnstep_set(ETHER_FLOAT_TO_ANGLE(atof(argv[1]))); 
			break;
		case ST_STEPSIZE: 
		case ST_SPACESTEP: 
			ether_world_movestep_set(ETHER_FLOAT_TO_SCALAR(atof(argv[1]))); 
			break;
		case ST_FLYMODE: 
			ether_world_movement_mode_set(strtoul(argv[1], NULL, 0) ? 1 : 0); 
			break;
		case ST_SCREENCLEAR: 
			ether_world_screenclear_set(strtoul(argv[1], NULL, 0) ? 1 : 0); 
			break;
		case ST_SCREENCOLOR:  /* fall through... same as skycolor */
		case ST_SKYCOLOR: 
			ether_world_sky_color_set((int)strtoul(argv[1], NULL, 0));
			break;
		case ST_GROUNDCOLOR: 
			ether_world_ground_color_set((int)strtoul(argv[1], NULL, 0)); 
			break;
		case ST_OPTIONS:
			{
			char *p;
			_ether_wld_options = strdup(argv[1]);
			for(p = _ether_wld_options; *p; ++p)
				switch (*p)
				{
					case 'h': ether_world_horizon_set(0); break;
					case 'H': ether_world_horizon_set(1); break;
					default: break;
				}
			}
			break;
		case ST_YON:
			{
				EtherScalar y = ETHER_FLOAT_TO_SCALAR(atof(argv[1]));
				if(_ether_wld_most_recent_camera)
					ether_camera_yon_set(_ether_wld_most_recent_camera, y);
				_ether_wld_default_yon = y;
			}
			break;
		case ST_ATTACH:
			{
				EtherObject *obj    = ether_world_find_object(argv[1]);
				EtherObject *parent = ether_world_find_object(argv[2]);
				if(obj && parent)
					ether_object_attach(obj, parent);
			}
			break;
		case ST_DETACH:
			{
				EtherObject *obj = ether_world_find_object(argv[1]);
				if(obj)
					ether_object_detach(obj);
			}
			break;
		case ST_POSITION:
			{
				EtherScalar x, y, z;
				EtherObject *obj = ether_world_find_object(argv[1]);
				if(obj == NULL)
					break;
				x = y = z = 0;
				if(argc > 2)
					x = ETHER_FLOAT_TO_SCALAR(atof(argv[2]));
				if(argc > 3)
					y = ETHER_FLOAT_TO_SCALAR(atof(argv[3]));
				if(argc > 4)
					z = ETHER_FLOAT_TO_SCALAR(atof(argv[4]));
				ether_object_move(obj, x, y, z);
			}
			break;
		case ST_ROTATE:
			{
				EtherAngle rx, ry, rz;
				EtherObject *obj = ether_world_find_object(argv[1]);
				if(obj == NULL)
					break;
				rx = ry = rz = 0;
				if(argc > 2) 
					rx = ETHER_FLOAT_TO_ANGLE(atof(argv[2]));
				if(argc > 3) 
					ry = ETHER_FLOAT_TO_ANGLE(atof(argv[3]));
				if(argc > 4) 
					rz = ETHER_FLOAT_TO_ANGLE(atof(argv[4]));
				ether_object_rot_reset(obj);
				ether_object_rot_y(obj, ry);
				ether_object_rot_x(obj, rx);
				ether_object_rot_z(obj, rz);
			}
			break;
		case ST_ATTACHVIEW:
			{
				EtherObject *parent = ether_world_find_object(argv[1]);
				if(parent && _ether_wld_most_recent_camera)
					if(!ether_object_is_fixed(parent))
						ether_object_attach(ether_camera_obj_get(_ether_wld_most_recent_camera), parent);
			}
			break;
		case ST_TITLE:
			if(strcasecmp(argv[1], "memory"))
				strncpy(_ether_wld_title[_ether_wld_titlerows++], buffcopy, sizeof(_ether_wld_title[0])-1);
			break;
		case ST_PALETTE:
			{
				EtherPalette *pal = ether_world_palette_get();
				if(pal)
				{
					FILE *palfile = fopen(_ether_wld_file_fixup_filename(argv[1]), "rb");
					if(palfile)
					{
						ether_palette_read(pal, palfile);
						fclose(palfile);
					}
				}
			}
			break;
		case ST_INCLUDE:
			{
				FILE *inc = fopen(_ether_wld_file_fixup_filename(argv[1]), "r");
				if(inc)
				{
					ether_wld_read(inc);
					fclose(inc);
				}
			}
			break;
		case ST_SURFACEDEF:
			{
				EtherSurface *surf = malloc(sizeof(EtherSurface));
				if(surf == NULL)
					break;
				ether_surface_init(surf);
				ether_surface_from_desc((unsigned short)strtoul(argv[2], NULL, 0), surf);
				_ether_wld_add_to_list(&_ether_wld_named_surfaces, argv[1], surf);
			}
			break;
		case ST_SURFACEMAP:
			{
				int i;
				if(argc > 2)
					_ether_wld_currmap_maxentries = atoi(argv[2]);
				else
					_ether_wld_currmap_maxentries = _ETHER_WLD_DEFAULT_SURFACEMAP_ENTRIES;
				_ether_wld_currmap = ether_surface_map_create(_ether_wld_currmap_maxentries);
				if(_ether_wld_currmap == NULL)
					break;
				_ether_wld_add_to_list(&_ether_wld_named_surfacemaps, argv[1], _ether_wld_currmap);
				for(i = 0; i < _ether_wld_currmap_maxentries; ++i)
					ether_surface_map_surface_set(_ether_wld_currmap, i, &_ether_wld_default_surface);
			}
			break;
		case ST_USEMAP:
			_ether_wld_currmap = _ether_wld_find_on_list(_ether_wld_named_surfacemaps, argv[1]);
			break;
		case ST_SURFACE:
			{
				int n = atoi(argv[1]);
				if(n >= _ether_wld_currmap_maxentries)
					break;
				if(_ether_wld_currmap == NULL)
					break;
				if(argc < 3)
					break;
				ether_surface_map_surface_set(_ether_wld_currmap, n, _ether_wld_find_on_list(_ether_wld_named_surfaces, argv[2]));
				if(ether_surface_map_surface_get(_ether_wld_currmap, n) == NULL)
					ether_surface_map_surface_set(_ether_wld_currmap, n, &_ether_wld_default_surface);
			}
			break;
		case ST_VERSION: 
			break;
		default:
			break;
	}
	return 0;
}

static int 
_ether_wld_getline(char *buff, int maxbytes, FILE *in)
{
	char *p;
	if(fgets(buff, maxbytes, in) == NULL)
		return 0;
	if((p = strchr(buff, '\r')) != NULL)
		*p = '\0';
	if((p = strchr(buff, '\n')) != NULL)
		*p = '\0';
	return 1;
}

static int 
_ether_wld_parse(char *buff, char *seps, char *argv[])
{
	char *p;
	int argc = 0;
	if ((p = strchr(buff, '#')) != NULL) *p = '\0';
	p = strtok(buff, seps);
	while (p)
		{
		argv[argc++] = p;
		p = strtok(NULL, seps);
		}
	return argc;
}

static int 
_ether_wld_lookup(char *token, char *table[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		if (!strcasecmp(token, table[i]))
			return i;
	return -1;
}

static void *
_ether_wld_add_to_list(EtherWldList **list, char *name, void *value)
{
	EtherWldList *p = malloc(sizeof(EtherWldList));
	if (p)
	{
		p->next = *list;
		*list = p;
		p->name = strdup(name);
		p->value = value;
	}
	return p;
}

static void *
_ether_wld_find_on_list(EtherWldList *list, char *name)
{
	while(list)
		if (!strcasecmp(name, list->name))
			return list->value;
		else
			list = list->next;
	return NULL;
}


static int 
_ether_wld_create_object(int argc, char *argv[])
{
	char *name = NULL;
	char *filename, *p;
	char fname[100];
	char *parent = NULL;
	float sx = 1, sy = 1, sz = 1;
	EtherAngle rx = 0, ry = 0, rz = 0;
	EtherScalar tx = 0, ty = 0, tz = 0;
	EtherShape *shape;
	EtherSurfaceMap *map = NULL;
	FILE *in;
	switch (argc)
	{
		default: /* ignore extra arguments */
		case 14: parent = argv[13];
		case 13: map = _ether_wld_find_on_list(_ether_wld_named_surfacemaps, argv[12]);
		case 12: /* we ignore the depthtype field for now; should use it */
		case 11: tz = ETHER_FLOAT_TO_SCALAR(atof(argv[10]));
		case 10: ty = ETHER_FLOAT_TO_SCALAR(atof(argv[9]));
		case 9:  tx = ETHER_FLOAT_TO_SCALAR(atof(argv[8]));
		case 8:  rz = ETHER_FLOAT_TO_SCALAR(atof(argv[7]));
		case 7:  ry = ETHER_FLOAT_TO_SCALAR(atof(argv[6]));
		case 6:  rx = ETHER_FLOAT_TO_SCALAR(atof(argv[5]));
		case 5:  sz = (float)atof(argv[4]);
		case 4:  sy = (float)atof(argv[3]);
		case 3:  sx = (float)atof(argv[2]);
		case 2:  name = argv[1]; break;
		case 1: 
		case 0: 
			return -2;
	}
	filename = strchr(name, '=');
	if(filename)
		*filename++ = '\0';
	else
	{
		filename = name;
		name = NULL;
	}
	ether_plg_scale_set(sx, sy, sz);
	ether_plg_offset_set(0, 0, 0);
	if((p = strchr(filename, '.')) != NULL)
		*p = '\0';
	strcpy(fname, _ether_wld_file_fixup_filename(filename));
	strcat(fname, ".plg");
	in = fopen(fname, "r");
	if(in == NULL)
		return -3;
	while((shape = ether_plg_read(in)) != NULL)
	{
		EtherObject *obj;
		obj = ether_object_create(shape);
		if(obj == NULL)
		{
			fclose(in);
			return -1;
		}
		/* if a map was specified, use it */
		if(map)
			ether_object_surfacemap_set(obj, map);
		/* otherwise, use the shape's default map if there is one */
		else if(ether_shape_surface_map_get(shape))
			ether_object_surfacemap_set(obj, ether_shape_surface_map_get(shape));
		/* otherwise, use the "current" map */
		else
			ether_object_surfacemap_set(obj, _ether_wld_currmap );
		if(parent)
		{
			if(!strcasecmp(parent, "fixed"))
			{
				ether_matrix_copy(obj->globalmat, obj->localmat);
				ether_object_make_fixed(obj);
			}
			else
				ether_object_attach(obj, ether_world_find_object(parent));
		}
		ether_object_rot_y(obj, ry);
		ether_object_rot_x(obj, rx);
		ether_object_rot_z(obj, rz);
		ether_object_move(obj, tx, ty, tz);
		if(name)
			ether_object_name_set(obj, strdup(name));
	}
	fclose(in);
	return 0;
}

static int 
_ether_wld_create_polyobj(int argc, char *argv[], int nsides)
{
	int i;
	EtherShape  *shape  = ether_shape_create();
	EtherObject *obj    = ether_object_create(shape);
	EtherRep    *rep    = ether_rep_create(atoi(argv[1]), 0);
	EtherFacet  *facet1 = ether_facet_create(atoi(argv[1]));
	EtherFacet  *facet2 = NULL;
	if(obj == NULL || shape == NULL || rep == NULL || facet1 == NULL)
		return -1;
	if(nsides > 1)
		facet2 = ether_facet_create(atoi(argv[1]));  /* if it fails, it fails */
	ether_shape_add_rep(shape, rep, 0);
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	ether_rep_add_facet(rep, facet1);
	ether_shape_surface_map_set(shape, ether_surface_map_create(nsides));
	if(ether_shape_surface_map_get(shape) == NULL)
		return -1;
	ether_object_surfacemap_set(obj, ether_shape_surface_map_get(shape));
	if(isdigit((int)*argv[2]))
	{
		EtherSurface *s = ether_surface_create(0);
		if(s)
			ether_surface_from_desc((unsigned short)strtoul(argv[2], NULL, 0), s);  /* convert value to surface */
		ether_surface_map_surface_set(ether_shape_surface_map_get(shape), 0, s);
	}
	else
		ether_surface_map_surface_set(ether_shape_surface_map_get(shape), 0, _ether_wld_find_on_list(_ether_wld_named_surfaces, argv[2]));
	if(nsides > 1) /* shift to allow for second color */
	{ 
		--argc; ++argv;
	}  
	if(facet2)
	{
		ether_facet_surf_num_set(facet2, 1);
		if(isdigit(*argv[2]))
		{
			EtherSurface *s = ether_surface_create(0);
			if(s)
				ether_surface_from_desc((unsigned short)strtoul(argv[2], NULL, 0), s);  /* convert value to surface */
			ether_surface_map_surface_set(ether_object_surfacemap_get(obj), 1, s);
		}
		else
			ether_surface_map_surface_set(ether_object_surfacemap_get(obj), 1, _ether_wld_find_on_list(_ether_wld_named_surfaces, argv[2]));
		ether_rep_add_facet(rep, facet2);
	}
	for(i = 0; i < ether_rep_count_vertices(rep); ++i)
	{
		rep->vertices[i][X] = ETHER_FLOAT_TO_SCALAR(atof(argv[3 + i * 3]));
		rep->vertices[i][Y] = ETHER_FLOAT_TO_SCALAR(atof(argv[3 + i * 3 + 1]));
		rep->vertices[i][Z] = ETHER_FLOAT_TO_SCALAR(atof(argv[3 + i * 3 + 2]));
		ether_facet_point_set(facet1, i, i);
		if(facet2)
			ether_facet_point_set(facet2, i, ether_rep_count_vertices(rep)-1 - i);
	}
	ether_facet_compute_normal(facet1, rep->vertices);
	if(facet2)
		ether_facet_compute_normal(facet2, rep->vertices);
	ether_shape_compute_bounds(shape);
	return 0;
}

static int _ether_wld_create_segment(int argc, char *argv[])
{
	char *name = NULL;
	EtherAngle rx = 0, ry = 0, rz = 0;
	EtherScalar tx = 0, ty = 0, tz = 0;
	EtherObject *obj = ether_object_create(NULL);
	if(obj == NULL)
		return -1;
	switch (argc)
	{
		default: /* ignore extra arguments */
		case 8: tz = ETHER_FLOAT_TO_SCALAR(atof(argv[7]));
		case 7: ty = ETHER_FLOAT_TO_SCALAR(atof(argv[6]));
		case 6: tx = ETHER_FLOAT_TO_SCALAR(atof(argv[5]));
		case 5: rz = ETHER_FLOAT_TO_SCALAR(atof(argv[4]));
		case 4: ry = ETHER_FLOAT_TO_SCALAR(atof(argv[3]));
		case 3: rx = ETHER_FLOAT_TO_SCALAR(atof(argv[2]));
		case 2: name = argv[1];
			break;
		case 1: case 0: return -2;
	}
	if(name)
	{
		char *attachment = strchr(name, '=');
		if (attachment)
		{
			EtherObject *parent;
			*attachment++ = '\0';
			parent = ether_world_find_object(attachment);
			ether_object_attach(obj, parent);
		}
		ether_object_name_set(obj, strdup(name));
	}
	ether_object_rot_y(obj, ry);
	ether_object_rot_x(obj, rx);
	ether_object_rot_z(obj, rz);
	ether_object_move(obj, tx, ty, tz);
	return 0;
}

static int _ether_wld_create_figure(int argc, char *argv[])
{
	char *name = NULL;
	char *filename;
	char fname[100];
	char *parent = NULL;
	float sx = 1, sy = 1, sz = 1;
	EtherAngle rx = 0, ry = 0, rz = 0;
	EtherScalar tx = 0, ty = 0, tz = 0;
	EtherObject *obj, *par = NULL;
	FILE *in;
	switch(argc)
	{
		default: /* ignore extra arguments */
		case 12: parent = argv[11];
		case 11: tz  = ETHER_FLOAT_TO_SCALAR(atof(argv[10]));
		case 10: ty  = ETHER_FLOAT_TO_SCALAR(atof(argv[9]));
		case 9: tx   = ETHER_FLOAT_TO_SCALAR(atof(argv[8]));
		case 8: rz   = ETHER_FLOAT_TO_SCALAR(atof(argv[7]));
		case 7: ry   = ETHER_FLOAT_TO_SCALAR(atof(argv[6]));
		case 6: rx   = ETHER_FLOAT_TO_SCALAR(atof(argv[5]));
		case 5: sz   = (float)atof(argv[4]);
		case 4: sy   = (float)atof(argv[3]);
		case 3: sx   = (float)atof(argv[2]);
		case 2: name = argv[1]; break;
		case 1: case 0: return -2;
	}
	filename = strchr(name, '=');
	if(filename)
		*filename++ = '\0';
	else
	{
		filename = name;
		name = NULL;
	}
	if(parent)
		par = ether_world_find_object(parent);
	ether_plg_scale_set(sx, sy, sz);
	strcpy(fname, _ether_wld_file_fixup_filename(filename));
	strcat(fname, ".fig");
	in = fopen(fname, "r");
	if(in == NULL)
		return -3;
	obj = ether_fig_read(in, par, name);
	fclose(in);
	ether_object_rot_x(obj, rx);
	ether_object_rot_y(obj, ry);
	ether_object_rot_z(obj, rz);
	ether_object_move(obj, tx, ty, tz);
	return 0;
}

static int 
_ether_wld_create_light(int argc, char *argv[])
{
	int spot = 0;
	char *name = NULL;
	EtherScalar x = 0, y = 0, z = 0;
	EtherObject *obj;
	EtherLight  *light = ether_light_create();
	if(light == NULL)
		return -1;
	obj = ether_object_create(NULL);
	if(obj == NULL)
	{
		ether_world_remove_light(light);
		free(light);
		return -1;
	}
	ether_light_associate(light, obj);
	switch (argc)
	{
		default:  /* ignore extra arguments */
		case 7: name = argv[6];
		case 6: ether_light_intensity_set(light, ETHER_FLOAT_TO_FACTOR(atof(argv[5])/128));
		case 5:
			if(!strcasecmp(argv[4], "spot"))
				spot = 1;
			else
				spot = atoi(argv[4]);
		case 4: z = ETHER_FLOAT_TO_FACTOR(atof(argv[3]));
		case 3: y = ETHER_FLOAT_TO_FACTOR(atof(argv[2]));
		case 2: x = ETHER_FLOAT_TO_FACTOR(atof(argv[1]));
			break;
		case 1: case 0: return -2;
	}
	if(name)
	{
		char *attachment = strchr(name, '=');
		if(attachment)
		{
			EtherObject *parent;
			*attachment++ = '\0';
			parent = ether_world_find_object(attachment);
			if(parent)
				ether_object_attach(obj, parent);
		}
		ether_light_name_set(light, strdup(name));
		ether_world_add_light(light);
	}
	if(spot)
	{
		EtherVector tmp, tmp2, tmp3;
		ether_matrix_identity(obj->localmat);
		ether_vector_create(tmp, x, y, z);
		if(ether_vector_magnitude(tmp) == 0)
			return -3;
		ether_vector_normalize(tmp);
		/* Z column is our normalized view vector */
		obj->localmat[X][Z] = tmp[X];
		obj->localmat[Y][Z] = tmp[Y];
		obj->localmat[Z][Z] = tmp[Z];
		/* Now we find a vector perpendicular to the view vector */
		if(x < 0)    /* find absolute values of x and */
			x = -x;  
		if(y < 0)    /* y (works in both floating and fixed) */
			y = -y;  
		if(x > y)    /* if X greater than Y, swap X and Z */
			ether_vector_create(tmp2, tmp[Z], tmp[Y], -tmp[X]);
		else   /* otherwise, swap Y and Z */
			ether_vector_create(tmp2, tmp[X], tmp[Z], -tmp[Y]);
		/* tmp2 is now a vector perpendicular to tmp, and like tmp is normalized */
		/* make it the Y column */
		obj->localmat[X][Y] = tmp2[X];
		obj->localmat[Y][Y] = tmp2[Y];
		obj->localmat[Z][Y] = tmp2[Z];
		/* the X column is just the cross product of the other two */
		ether_vector_crossproduct(tmp3, tmp2, tmp);
		obj->localmat[X][X] = tmp3[X];
		obj->localmat[Y][X] = tmp3[Y];
		obj->localmat[Z][X] = tmp3[Z];
		obj->moved = 1;
		ether_light_type_set(light, ETHER_LIGHT_DIRECTIONAL);
	}
	else
	{
		ether_light_type_set(light, ETHER_LIGHT_POINTSOURCE);
		ether_object_move(obj, x, y, z);
	}
	return 0;
}

static int _ether_wld_create_camera(int argc, char *argv[])
{
	float zoom = 4.0;
	char *name;
	EtherScalar x = 0, y = 0, z = 0, hither = _ether_wld_default_hither, yon = _ether_wld_default_yon;
	EtherAngle tilt = 0, pan = 0, roll = 0;
	EtherObject *obj;
	EtherCamera *camera = ether_camera_create();
	if(camera == NULL)
		return -1;
	obj = ether_object_create(NULL);
	if(obj == NULL)
	{
		ether_world_remove_camera(camera);
		free(camera);
		return -1;
	}
	ether_camera_obj_set(camera, obj);
	switch (argc)
	{
		default:  /* ignore extra arguments */
		case 11: yon    = ETHER_FLOAT_TO_SCALAR(atof(argv[10]));
		case 10: hither = ETHER_FLOAT_TO_SCALAR(atof(argv[9]));
		case 9:  zoom   = (float)atof(argv[8]);
		case 8:  roll   = ETHER_FLOAT_TO_ANGLE(atof(argv[7]));
		case 7:  pan    = ETHER_FLOAT_TO_ANGLE(atof(argv[6]));
		case 6:  tilt   = ETHER_FLOAT_TO_ANGLE(atof(argv[5]));
		case 5:  z      = ETHER_FLOAT_TO_SCALAR(atof(argv[4]));
		case 4:  y      = ETHER_FLOAT_TO_SCALAR(atof(argv[3]));
		case 3:  x      = ETHER_FLOAT_TO_SCALAR(atof(argv[2]));
		case 2:  name   = argv[1];
			break;
		case 1: case 0: return -2;
	}
	ether_camera_hiter_set(camera, hither);
	ether_camera_yon_set(camera, yon);
	ether_camera_zoom_set(camera, zoom);
	ether_camera_name_set(camera, strdup(name));
	ether_world_add_camera(camera);
	ether_object_rot_x(obj, tilt);
	ether_object_rot_y(obj, pan);
	ether_object_rot_z(obj, roll);
	ether_object_move(obj, x, y, z);
	_ether_wld_most_recent_camera = camera;
	if(ether_world_camera_get() == NULL)
		ether_world_camera_set(camera);
	return 0;
}
