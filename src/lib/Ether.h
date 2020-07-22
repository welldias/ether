#ifndef _ETHER_H
#define _ETHER_H

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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <cglm\cglm.h>

#define ETHER_UNITY 1.0
#define ETHER_ANGLE_CONVERSION 1

#ifndef NULL
#define NULL 0
#endif

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define X    0
#define Y    1
#define Z    2
#define XROT 3
#define YROT 4
#define ZROT 5

#define ETHER_FLOAT_TO_SCALAR(f) ((EtherScalar)(f))
#define ETHER_SCALAR_TO_FLOAT(s) ((float)(s))
#define ETHER_FLOAT_TO_FACTOR(f) ((EtherFactor) ((f) * ETHER_UNITY))
#define ETHER_FACTOR_TO_FLOAT(f) ((float)((f) / ETHER_UNITY))
#define ETHER_FLOAT_TO_ANGLE(f)  ((EtherAngle) ((f) * ETHER_ANGLE_CONVERSION))
#define ETHER_ANGLE_TO_FLOAT(a)  ((float)((a)/ETHER_ANGLE_CONVERSION))

/* Three special data types are used in Ether:
 * - EtherScalar : a measure of virtual distance
 * - EtherAngle  : a multiplication factor, usually in the range -1 to +1
 * - EtherFactor : measured in 65536th's of a degree
 */

typedef float EtherScalar;
typedef float EtherAngle;
typedef float EtherFactor;

typedef size_t       EtherTime;
typedef unsigned int EtherColor;

typedef unsigned int EtherShader;
typedef unsigned int EtherTexture;

typedef struct _Ether_Shape        EtherShape;
typedef struct _Ether_Surface_Map  EtherSurfaceMap;
typedef struct _Ether_Facet        EtherFacet;
typedef struct _Ether_Edge         EtherEdge;
typedef struct _Ether_Rep          EtherRep;
typedef struct _Ether_Surface      EtherSurface;
typedef struct _Ether_Object       EtherObject;
typedef struct _Ether_Light        EtherLight;
typedef struct _Ether_Camera       EtherCamera;
typedef struct _Ether_Camera_Old   EtherCameraOld;
typedef struct _Ether_World        EtherWorld;
typedef struct _Ether_Statistics   EtherStatistics;
typedef struct _Ether_Raster       EtherRaster;
typedef struct _Ether_Outputvertex EtherOutputvertex;
typedef struct _Ether_Outputfacet  EtherOutputfacet;

#include <ether_math.h>
#include <ether_timer.h>
#include <ether_vector.h>
#include <ether_matrix.h>
#include <ether_surface.h>
#include <ether_palette.h>
#include <ether_facet.h>
#include <ether_rep.h>
#include <ether_shape.h>
#include <ether_object.h>
#include <ether_light.h> 
#include <ether_camera.h>
#include <ether_camera_old.h>
#include <ether_world.h>
#include <ether_primitives.h>
#include <ether_statistics.h>
#include <ether_raster.h>
#include <ether_display.h>
#include <ether_render.h>
#include <ether_application.h>
#include <ether_system.h>
#include <ether_task.h>
#include <ether_plg.h>
#include <ether_fig.h>
#include <ether_wld.h>
#include <ether_shader.h>
#include <ether_texture.h>

#ifdef __cplusplus
}
#endif

#endif /* _ETHER_H */
