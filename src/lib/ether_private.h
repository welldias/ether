#ifndef _ETHER_PRIVATE_H
#define _ETHER_PRIVATE_H

# define _ETHER_TRANS 3
# define _ETHER_SCREEN_FRACT_BITS 8 // screen coords are 24.8

#define _ETHER_DISPLAY_XYCLIP_CLIP_X     0x01
#define _ETHER_DISPLAY_XYCLIP_CLIP_Y     0x02
#define _ETHER_DISPLAY_XYCLIP_Z          0x04
#define _ETHER_DISPLAY_XYCLIP_INTENSITY  0x08

#define _ETHER_ZEROMEM(obj) memset(&obj, 0, sizeof(obj))
#define _ETHER_MEMSET(obj, c) memset(&obj, c, sizeof(obj))
#define _ETHER_NUM_ELEMS(a) (sizeof(a)/sizeof(a[0]))

#ifndef WIN32_WCE
	#include <signal.h>
#endif

#ifdef WIN32
#include <ctype.h>
#define strcasecmp(a, b) _stricmp(a, b)
#define strdup(a) _strdup(a)
#define strncasecmp(a, b, c) _strnicmp(a, b, c)
#endif

#include "ether_defines.h"

typedef struct _Ether_Local_Light   EtherLocalLight;

struct _Ether_Local_Light {
	EtherLightingType type;
	EtherFactor intensity;
	EtherMatrix mat;		/* transforms from light space into object space */
};

/* ether_math.c */
void _ether_math_mult3x3   (EtherMatrix result, EtherMatrix m1, EtherMatrix m2);

/* ether_transform.c */
void        _ether_transform   (EtherVector result, EtherMatrix m, EtherVector v);
EtherScalar _ether_transform_x (EtherMatrix m, EtherVector v);
EtherScalar _ether_transform_y (EtherMatrix m, EtherVector v);
EtherScalar _ether_transform_z (EtherMatrix m, EtherVector v);

/* ether_memory.c */
int   _ether_memory_init    (unsigned int maxmem);
void  _ether_memory_quit    (void);
void  _ether_memory_clear   (void);
void *_ether_memory_alloc   (int n);
void *_ether_memory_mark    (void);
void  _ether_memory_release (void *ptr);

/* ether_wld.c */
void   _ether_wld_file_set_load_path  (char *path);
char  *_ether_wld_file_fixup_filename (char *filename);

#endif /* _ETHER_PRIVATE_H */
