#ifndef __ETHER_SURFACE_H__
#define __ETHER_SURFACE_H__

#include "ether_defines.h"

EAPI EtherSurface       *ether_surface_init           (EtherSurface *surf);
EAPI EtherSurface       *ether_surface_from_desc      (unsigned short int desc, EtherSurface *surf);
EAPI unsigned short int  ether_surface_to_desc        (EtherSurface *surf);
EAPI EtherSurface       *ether_surface_list_get       (void);
EAPI EtherSurface       *ether_surface_create         (unsigned char hue);
EAPI void                ether_surface_type_set       (EtherSurface *surf, EtherSurfaceType type);
EAPI EtherSurfaceType    ether_surface_type_get       (EtherSurface *surf);
EAPI void                ether_surface_hue_set        (EtherSurface *surf, unsigned char hue);
EAPI unsigned char       ether_surface_hue_get        (EtherSurface *surf);
EAPI void                ether_surface_brightness_set (EtherSurface *surf, unsigned char brightness);
EAPI unsigned char       ether_surface_brightness_get (EtherSurface *surf);
EAPI void                ether_surface_exponent_set   (EtherSurface *surf, unsigned char exponent);
EAPI unsigned char       ether_surface_exponent_get   (EtherSurface *surf);
EAPI EtherSurface       *ether_surface_next_get       (EtherSurface *surf);

EAPI EtherSurfaceMap  *ether_surface_map_create        (int n);
EAPI EtherSurfaceMap  *ether_surface_map_list_get      (void);
EAPI int               ether_surface_map_count_entries (EtherSurfaceMap *map);
EAPI int               ether_surface_map_surface_set   (EtherSurfaceMap *map, int surfnum, EtherSurface *surf);
EAPI EtherSurface     *ether_surface_map_surface_get   (EtherSurfaceMap *map, int surfnum);
EAPI EtherSurfaceMap  *ether_surface_map_next_get      (EtherSurfaceMap *map);

#endif /* __ETHER_SURFACE_H__ */
