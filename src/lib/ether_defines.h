#ifndef __ETHER_DEFINES_H__
#define __ETHER_DEFINES_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EAPI
/*
#ifdef EAPI
#undef EAPI
#endif
#ifdef _WIN32
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef GCC_HASCLASSVISIBILITY
#  define EAPI __attribute__ ((visibility("default")))
# else
#  define EAPI
# endif
#endif
#define EAPI
*/

#define ETHER_UNITY 1.0
#define ETHER_ANGLE_CONVERSION 1

#ifndef NULL
#define NULL 0
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define X    0
#define Y    1
#define Z    2
#define XROT 3
#define YROT 4
#define ZROT 5

/* Three special data types are used in Ether:
 * - Ether_Scalar : a measure of virtual distance
 * - Ether_Angle  : a multiplication factor, usually in the range -1 to +1
 * - Ether_Factor : measured in 65536th's of a degree
 */

typedef float EtherScalar;
typedef float EtherAngle;
typedef float EtherFactor;

typedef unsigned int EtherTime;
typedef unsigned int EtherColor;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define ETHER_FLOAT_TO_SCALAR(f) ((EtherScalar)(f))
#define ETHER_SCALAR_TO_FLOAT(s) ((float)(s))
#define ETHER_FLOAT_TO_FACTOR(f) ((EtherFactor) ((f) * ETHER_UNITY))
#define ETHER_FACTOR_TO_FLOAT(f) ((float)((f) / ETHER_UNITY))
#define ETHER_FLOAT_TO_ANGLE(f)  ((EtherAngle) ((f) * ETHER_ANGLE_CONVERSION))
#define ETHER_ANGLE_TO_FLOAT(a)  ((float)((a)/ETHER_ANGLE_CONVERSION))

typedef enum _Ether_Sorting_Type EtherSortingType;
typedef enum _Ether_Surface_Type EtherSurfaceType;
typedef enum _Ether_Lighting_Type EtherLightingType;
typedef enum _Ether_Coord_Frame EtherCoordFrame;
typedef enum _Ether_Display_Command EtherDisplayCommand;
typedef enum _Ether_Video_Command EtherVideoCommand;

typedef struct _Ether_Facet EtherFacet;
typedef struct _Ether_Rep EtherRep;
typedef struct _Ether_Edge EtherEdge;
typedef struct _Ether_Surface EtherSurface;
typedef struct _Ether_Surface_Map EtherSurfaceMap;
typedef struct _Ether_Hue EtherHue;
typedef struct _Ether_Palette EtherPalette;
typedef struct _Ether_Shape EtherShape;
typedef struct _Ether_Object EtherObject;
typedef struct _Ether_Light EtherLight;
typedef struct _Ether_Camera EtherCamera;
typedef struct _Ether_World EtherWorld;
typedef struct _Ether_Render_Status EtherRenderStatus;
typedef struct _Ether_Render_Output_Object EtherRenderOutputObject;
typedef struct _Ether_Temp_Vertex EtherTempVertex;
typedef struct _Ether_Statistics EtherStatistics;
typedef struct _Ether_Outputvertex EtherOutputvertex;
typedef struct _Ether_Outputfacet EtherOutputfacet;
typedef struct _Ether_Raster EtherRaster;

typedef int (*EtherObjectFunction)(EtherObject *obj);

typedef int EtherDisplayDriverFunction(EtherDisplayCommand cmd, int lparm, void *pparm1);
typedef int EtherVideoDriverFunction(EtherVideoCommand cmd, int lparm, void *pparm1);

typedef void your_blit_especial_func(EtherRaster *raster);

extern EtherDisplayDriverFunction *_ether_display_current_driver;
extern EtherVideoDriverFunction *_ether_video_current_driver;

extern EtherWorld *ether_world_current; /*< the currently active world */

/***************************************************/
// Ether Matrix
/***************************************************/
/* A Ether_Matrix is a 4 by 3 array that stores location
 * and orientation information.  All Ether matrices are homogeneous;
 * the upper 3 by 3 submatrix stores rotation information and
 * the last 3-element row stores a translation vector.
 * You should never have to deal with the Ether_Matrix type directly.
 * However, in case you do have a need to deal with actual matrices
 *
 */

typedef EtherScalar EtherMatrix[4][3];

/***************************************************/
// Ether Vector
/***************************************************/
/*
 * A Ether_Vector is a three-element array, which can be
 * indexed by the #defined constants X, Y and Z; for example,
 * if v is a vector then v[X] is the X-component of the vector.
 * In general, Ether_Vector are made up of three Ether_Scalars;
 * however, a normalized vector (such as a facet normal, a basis
 * vector, or a vector that's been normalized using the
 * Ether_VectorNormalize() function) will actually have
 * Ether_Factors as elements.
 */
typedef EtherScalar EtherVector[3];

/***************************************************/
// Ether Rep
/***************************************************/
enum _Ether_Sorting_Type {
  ETHER_SORT_UNDEF = -1,
  ETHER_SORT_NONE = 0,
  ETHER_SORT_FARTHEST,
  ETHER_SORT_NEAREST,
  ETHER_SORT_AVERAGE,
  ETHER_SORT_OTHER
};

struct _Ether_Edge {
  int v1, v2;
};

struct _Ether_Rep {
  EtherScalar size;           /* size (in pixels) at which to use this rep */
  EtherRep *next;             /* next less-detailed rep */
  EtherSortingType sorttype;  /* type of poly sorting to do on this rep */
  int nvertices;              /* number of vertices (and normals if present) */
  EtherVector *vertices;      /* array of vertices */
  EtherVector *normals;       /* array of vertex normals; can be NULL */
  EtherFacet *facets;         /* facets are kept in a linked list */
  EtherEdge *edges;
};

/***************************************************/
// Ether Facet
/***************************************************/
struct _Ether_Facet {
  int surface;                      /* index into object's surface array */
  EtherVector normal;               /* perpendicular to facet, left-hand rule */
  unsigned int id;                  /* identifier for this facet */
  int highlight : 1;                /* facet is highlighted */
  int interior : 1;                 /* facet is on the interior of an object (not used) */
  EtherFacet *details;              /* points to linked list of detail facets (not used) */
  EtherFacet *nearside, *farside;   /* only farside is used */
  int npoints;                      /* number of points in the facet */
  int *points;                      /* indices into array of vertices of the facet points */
  int *edges;                       /* indices into array edges */
};

/***************************************************/
// Ether Surface
/***************************************************/
/*
 * Ether surfaces are designed for expandability. At
 * the moment, each ether_surface* consists of a type,
 * a hue and a brightness. The types are ETHER_SURF_SIMPLE
 * (no lighting, just a fixed color), ETHER_SURF_FLAT (for flat shading),
 * ETHER_SURF_GOURAUD (for Gouraud shading), ETHER_SURF_METAL
 * (for a pseudo-metallic effect) and ETHER_SURF_GLASS (for a partially
 * transparent effect).
 *
 */

typedef enum _Ether_Surface_Type {
  ETHER_SURF_SIMPLE = 0,
  ETHER_SURF_FLAT,
  ETHER_SURF_METAL,
  ETHER_SURF_GLASS,
  ETHER_SURF_GOURAUD,
  ETHER_SURF_SPECULAR
} EtherSurfaceType;

struct _Ether_Surface {
  EtherSurfaceType type;
  unsigned char hue;
  unsigned char brightness;
  unsigned char exponent;
  EtherSurface *next;
};

/*
 * Surface maps contain an array of pointers to surfaces; you can
 * create a surface map with room for a particular number of entries, find
 * out how many entries the map contains, and access entries within a map.
 *
 */
struct _Ether_Surface_Map {
  int nentries;
  EtherSurface **entries;
  EtherSurfaceMap *next;
};

/***************************************************/
// Ether Palette
/***************************************************/
/*
 * Ether makes use of a "palette", a collection of (up to) 256 colors.
 * Each of those colors has three 8-bit components -- one for red, one for 
 * green and one for blue.
 * The Ether_Color values that Ether uses are (in a paletted implementation) 
 * used to index the palette to select an actual color. In order to do 
 * shading of facets and vertices, Ether divides the palette into a number 
 * of "hues", with a number of shades for each hue. By default, the first 
 * 16 entries of the 256-color palette are simple non-shaded colors (for use 
 * in menus and overlaid text); the remaining 240 colors are treated as 15 
 * hues with 16 shades each.
 *
 * However, this is not etched in stone. Ether supports the use of a "hue map",
 * which relates a hue index to a start color in the palette and a count of the 
 * number of shades.  For example, by using the hue map, you could choose to 
 * have 64 shades of flesh tone (instead of 16) in order to represent human 
 * beings more accurately.
 *
 * A hue is represented by a Ether_Hue type, and a palette by a Ether_Palette type.
 * Palettes have a flag that indicates that they've been changed; this is important,
 * since it forces the system to update the physical palette stored in the video hardware.
 */
struct _Ether_Hue {
  unsigned char start;      /* starting index into palette */
  unsigned char maxshade;   /* maximum number of shades */
};

struct _Ether_Palette {
  unsigned char data[256][3];
  EtherHue huemap[256];
  int changed : 1;
};

/***************************************************/
// Ether Lighting
/***************************************************/
/* Lights in Ether have a number of properties; they can be on or
 * off, they can have an intensity, they can have a "type", and they
 * can be associated with an object.  The on/off and intensity properties
 * are similar to a household dimmer; rotating the knob on a dimmer alters
 * the intensity, and pushing it in toggles the light on and off.
 * The current version of Ether only supports ambient lights and directional
 * lights; point sources will be supported soon. Any light that is not
 * associated with an object is considered ambient; this is in addition to
 * the overall ambient light level for the world.  A directional light uses
 * the orientation of the object it's associated with to determine which
 * direction the light should come from. A point source light (once implemented)
 * will use the location of the object it's associated with to determine where
 * the light comes from.
 * As with worlds and objects, lights can be statically or dynamically created
 * and destroyed.
 *
 */

enum _Ether_Lighting_Type {
  ETHER_LIGHT_AMBIENT = 0,
  ETHER_LIGHT_DIRECTIONAL,
  ETHER_LIGHT_POINTSOURCE
};

/***************************************************/
// Ether Shape
/***************************************************/
struct _Ether_Shape {
  EtherVector center;   /**< center of bounding sphere */
  EtherScalar radius;   /**< radius of bounding sphere */
  EtherVector minbound; /**< bounding box */
  EtherVector maxbound;
  EtherSurfaceMap *default_surfacemap; /**< default surface map for this shape */
  EtherRep *replist;       /**< linked list of representations */
  char *name;              /**< name of this shape */
  EtherShape *next;        /**< shapes are kept in a linked list */
};

/***************************************************/
// Ether Object
/***************************************************/
/* Objects are the most important entities in a virtual world.
 * All objects have a location and orientation, and they can be
 * attached to each other in a tree-structured hierarchy.
 * Each object can have a shape (i.e. geometric description) and
 * a surface map. You can create an object statically (by declaring
 * a variable of type Ether_Object) or dynamically (either by using
 * malloc() to allocate the space and ether_object_init() to initialize
 * it, or by simply calling ether_object_create()). If you use
 * ether_object_create(), you can optionally specify a shape for the
 * object to use; if you don't want to assign a shape, use NULL. You
 * can also destroy objects using Ether_object_destroy().
 *
 */
enum _Ether_Coord_Frame {
  ETHER_COORD_LOCAL = 0,
  ETHER_COORD_PARENT,
  ETHER_COORD_WORLD,
  ETHER_COORD_OBJREL
};

typedef int (*EtherObjectFunction)(EtherObject *obj);

struct _Ether_Object {
  EtherShape *shape;        /**< geometry information */
  EtherSurfaceMap *surfmap; /**< array of pointers to surface descriptors */
  EtherMatrix localmat;     /**< transformation matrix relative to our parent */
  EtherMatrix globalmat;    /**< transformation matrix relative to the world */
  EtherObject *parent;      /**< pointer to our parent in the hierarchy */
  EtherObject *children;    /**< pointer to our children */
  EtherObject *siblings;    /**< pointers to our siblings */
  EtherVector minbound, maxbound; /**< bounding box (world coords) */
  int fixed : 1;                  /**< set if object is immobile */
  int moved : 1;                  /**< set when our local matrix has changed */
  int rotate_box : 1;             /**< set if bounding box should rotate */
  int highlight : 1;              /**< set if object is highlighted */
  int invisible : 1;              /**< set if object is invisible */
  char *name;                     /**< name of the object (may be NULL) */
  void *applic_data;              /**< pointer to application-specific data */
  unsigned char layer;            /**< the layer we're on (0 for all, 1-255) */
  EtherObject
      *contents; /**< points to objects contained by this one (not used) */
  EtherRep *forced_rep;         /**< if not NULL, forces a rep to be used */
  EtherObjectFunction function; /**< object function */
  EtherObject *next;            /**< points to next object on a list */
};

/***************************************************/
// Ether Light
/***************************************************/
/* Lights in Ether have a number of properties; they can be on or
 * off, they can have an intensity, they can have a "type", and they
 * can be associated with an object.  The on/off and intensity properties
 * are similar to a household dimmer; rotating the knob on a dimmer alters
 * the intensity, and pushing it in toggles the light on and off.
 * The current version of Ether only supports ambient lights and directional
 * lights; point sources will be supported soon. Any light that is not
 * associated with an object is considered ambient; this is in addition to
 * the overall ambient light level for the world.  A directional light uses
 * the orientation of the object it's associated with to determine which
 * direction the light should come from. A point source light (once implemented)
 * will use the location of the object it's associated with to determine where
 * the light comes from.
 * As with worlds and objects, lights can be statically or dynamically created
 * and destroyed.
 */
struct _Ether_Light {
   EtherLightingType type;              /* type of light source */
   EtherFactor intensity;               /* how bright the light is */
   int on : 1;                          /* set if the light is on */
   char *name;                          /* name of this light */
   void *applic_data;                   /* pointer to application-specific data */
   int (*function)(EtherLight *light);  /* light function */
   EtherObject *object;                 /* the object this light is associated with, if any */
   EtherLight *next;                    /* lights are kept in a linked list */
};

/***************************************************/
// Ether Display
/***************************************************/
/*
 * Ether allows you to have any number of virtual cameras. Each camera 
 * is associated with an object, much as lights are. However, unlike 
 * lights, cameras must be associated with an object; there's no such 
 * thing as an "ambient" camera.  Cameras are initialized and destroyed 
 * just like objects or light sources.
 * 
 * Cameras have only a few properties that are important; in particular, 
 * a zoom factor, an aspect ratio, and hither and yon clipping plane distances.
 */
struct _Ether_Camera {
   EtherScalar hither;                     /* distance to near clipping plane */
   EtherScalar yon;                        /* distance to far culling plane */
   float zoom;                             /* zoom factor (1/tan(FOV/2)) */
   float aspect;                           /* aspect ratio */
   int ortho : 1;                          /* set if we want orthographic projection (not used) */
   char *name;                             /* name of this camera */
   void *applic_data;                      /* pointer to application-specific data */
   unsigned char need_updating;            /* set when zoom or aspect is changed */
   EtherScalar orthodist;                  /* apparent "distance" for orthographic projection (not used) */
   EtherObject *object;                    /* the object this camera is attached to */
   EtherFactor aright, cright, btop, ctop; /* only used internally, for object culling */
   EtherCamera *next;                      /* cameras are kept in a linked list */
};

/***************************************************/
// Ether World
/***************************************************/
/* In Ether, a virtual world is a collection of objects, light sources, virtual 
 * cameras and miscellaneous attributes.  You can have any number of worlds within 
 * a single Ether application; they're distinct from each other, and you can switch 
 * between them whenever you like.
 */
struct _Ether_World {
    EtherObject *objects;         /* tree of objects */
    EtherLight  *lights;          /* linked list of lights */
    EtherCamera *cameras;         /* linked list of cameras */
    EtherCamera *camera;          /* current camera */
    EtherCamera *left_camera;     /* left-eye camera */
    EtherCamera *right_camera;    /* right-eye camera */
    EtherFactor  ambient;         /* ambient light level */
    EtherScalar  scale;           /* millimeters per unit of virtual space */
    EtherScalar  movestep;        /* default movement step size */
    EtherAngle   rotstep;         /* default rotation step size */
    int movement_mode : 1;        /* non-zero if we can fly by looking up */
    int screenclear : 1;          /* set if we should clear the screen */
    int horizon : 1;              /* set if we should draw a horizon   */
    EtherColor horizoncolors[10]; /* entry 0 is ground, entry n is sky */
    int nhorizoncolors;           /* number of colors used in horizoncolors[] */
    EtherPalette palette;         /* the palette, if one is used */
};

/***************************************************/
// Ether Display
/***************************************************/
/*
 * The display subsystem is the "back end" of the rendering pipeline;
 * it's responsible for actually drawing polygons (and lines, and text)
 * into a raster.
 *
 * Internally, the display driver is just a function (much like the
 * video driver, or any of the input device drivers);
 * The application-visible functions that access that driver are described here.
 */

enum _Ether_Display_Command {
  ETHER_DISPLAY_GET_VERSION = 0,
  ETHER_DISPLAY_GET_DESCRIPTION,
  ETHER_DISPLAY_INIT,
  ETHER_DISPLAY_QUIT,
  ETHER_DISPLAY_CLEAR,
  ETHER_DISPLAY_POINT,
  ETHER_DISPLAY_LINE,
  ETHER_DISPLAY_CLOSED_LINE,
  ETHER_DISPLAY_POLY,
  ETHER_DISPLAY_BOX,
  ETHER_DISPLAY_TEXT,
  ETHER_DISPLAY_TEXT_POSITION,
  ETHER_DISPLAY_GET_TEXTWIDTH,
  ETHER_DISPLAY_GET_TEXTHEIGHT,
  ETHER_DISPLAY_GET_SORTORDER,
  ETHER_DISPLAY_CAN_GOURAUD,
  ETHER_DISPLAY_UPDATE_PALETTE,
  ETHER_DISPLAY_CAN_XY_CLIP,
  ETHER_DISPLAY_BEGIN_FRAME,
  ETHER_DISPLAY_END_FRAME,
  ETHER_DISPLAY_SET_RASTER,
  ETHER_DISPLAY_GET_RASTER,
  ETHER_DISPLAY_SET_Z_BUFFER,
  ETHER_DISPLAY_GET_Z_BUFFER,
  ETHER_DISPLAY_USE_Z_BUFFER,
  ETHER_DISPLAY_CLEAR_Z_BUFFER,
  ETHER_DISPLAY_SET_SHADING,
};

enum _Ether_Video_Command {
  ETHER_VIDEO_GET_VERSION = 0,
  ETHER_VIDEO_GET_DESCRIPTION,
  ETHER_VIDEO_SETUP,
  ETHER_VIDEO_SHUTDOWN,
  ETHER_VIDEO_GET_MODE,
  ETHER_VIDEO_SET_DRAW_PAGE,
  ETHER_VIDEO_SET_VIEW_PAGE,
  ETHER_VIDEO_GET_NPAGES,
  ETHER_VIDEO_HAS_PALETTE,
  ETHER_VIDEO_SET_PALETTE,
  ETHER_VIDEO_SET_NTSC,
  ETHER_VIDEO_CHECK_RETRACE,
  ETHER_VIDEO_GET_RASTER,
  ETHER_VIDEO_BLIT,
  ETHER_VIDEO_CURSOR_HIDE,
  ETHER_VIDEO_CURSOR_SHOW,
  ETHER_VIDEO_CURSOR_RESET,
  ETHER_VIDEO_CURSOR_MOVE,
  ETHER_VIDEO_CURSOR_SET_APPEARANCE
};

struct _Ether_Outputvertex {
  long int x, y, z;        /* X, Y screen coordinates and Z-depth */
  EtherColor red;          /* components of the color */
  EtherColor green;
  EtherColor blue;
  EtherOutputvertex *next; /* doubly-linked circular list */
  EtherOutputvertex *prev;

  /* don't rely on anything below here staying the same */
  int u, v;                /* texture map coordinates */
  EtherFactor intensity;   /* intensity of light at this vertex */
  int outcode;             /* used for XY clipping */
};

struct _Ether_Outputfacet {
  EtherOutputvertex *points; /* linked list of vertices for this facet */
  EtherSurface *surface;     /* surface properties */
  EtherColor color;          /* color of this facet (flat shading only) */

  /* don't rely on anything below here staying the same */
  EtherFactor intensity;     /* amount of light falling on this facet */
  int xclip;                 /* this facet needs X clipping */
  int yclip;                 /* this facet needs Y clipping */
  int highlight : 1;         /* this facet is highlighted */
  EtherFacet *original;      /* points back at the facet we came from */
  void *outobj;              /* points back at the outobject we belong to */
  long int minbound[3];      /* bounding box for smarter sorting */
  long int maxbound[3];
  EtherOutputfacet *details; /* linked list of detail facets */
  EtherOutputfacet *next;    /* only used to link detail facets */
};

/***************************************************/
// Ether Raster
/***************************************************/
/*
 * Ether uses the notion of a "raster", a rectangular array of pixel values.
 * In most cases, all rendering is done into a raster, which may or may not
 * correspond to an actual physical screen; in cases where it doesn't,
 * it's necessary to copy ("blit") the raster to the display.
 * Each raster has a height, a width, a depth (number of bits per pixel) a
 * window (the rectangular region within the raster into which
 * rendering is done) and a "rowbytes" value (the number of bytes per
 * horizontal row of pixels).
 *
 * The reason for setting the number of bytes per row of pixels has to
 * do with alternate scan-line encoding; if you render the left and right
 * images into the left and right halves of a 640-pixel wide screen, and
 * then set the rowbytes value to 320, you wind up with a raster that has
 * the left-eye image on the even scanlines and the right-eye image on the
 * odd scanlines.
 */
struct _Ether_Raster {
  int width;
  int height;
  int depth;
  int left;
  int top;
  int right;
  int bottom;
  int rowbytes;
  unsigned char *data;
};

/***************************************************/
// Ether Render
/***************************************************/
struct _Ether_Render_Status {
  int objects : 1; /**< set if there were too many objects */
  int facets : 1;  /**< set if there were too many facets */
};

struct _Ether_Render_Output_Object {
  EtherObject *original;        /* points back at the object we came from */
  EtherOutputfacet **outfacets; /* array of output facets */
  int noutfacets;               /* number of output facets */
  int not_drawn;                /* set if object hasn't been drawn yet */
};

struct _Ether_Temp_Vertex {
  EtherVector vertex;    /* transformed vertex */
  long int projected_x;  /* projected X */
  long int projected_y;  /* projected Y */
  EtherFactor intensity; /* for Gouraud shading */
  enum {
    TEMP_VERTEX_UNPROCESSED = 0,
    TEMP_VERTEX_Z_DONE,
    TEMP_VERTEX_XY_DONE,
    TEMP_VERTEX_PROJECTED
  } status;
};

/***************************************************/
// Ether Statistics
/***************************************************/
struct _Ether_Statistics {
  int objects_processed;
  int objects_not_invisible;
  int objects_not_behind;
  int objects_not_leftright;
  int objects_not_abovebelow;
  int objects_not_toosmall;
  int lights_processed;
  int facets_processed;
  int facets_not_backfacing;
  int facets_flatshaded;
  int vertices_zchecked;
  int vertices_ztransformed;
  int facets_not_behind;
  int vertices_xychecked;
  int vertices_xytransformed;
  int vertices_projected_regular;
  int facets_gouraud_shaded;
  int vertex_pointlights;
  int vertex_dirlights;
  int detail_facets;
  int not_toomanyfacets;
  int facets_need_x_clipping;
  int facets_need_y_clipping;
  int facets_need_z_clipping;
};

#endif /* __ETHER_DEFINES_H__ */