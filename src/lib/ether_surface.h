#ifndef __ETHER_SURFACE_H__
#define __ETHER_SURFACE_H__

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


typedef enum _Ether_Surface_Type
{
   ETHER_SURF_SIMPLE = 0,
   ETHER_SURF_FLAT,
   ETHER_SURF_METAL,
   ETHER_SURF_GLASS,
   ETHER_SURF_GOURAUD,
   ETHER_SURF_SPECULAR
} EtherSurfaceType;

struct _Ether_Surface
{
   EtherSurfaceType type;
   unsigned char hue;
   unsigned char brightness;
   unsigned char exponent;
   EtherSurface *next;
};

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

/*
 * Surface maps contain an array of pointers to surfaces; you can 
 * create a surface map with room for a particular number of entries, find 
 * out how many entries the map contains, and access entries within a map.
 * 
 */

struct _Ether_Surface_Map
{
   int nentries;
   EtherSurface **entries;
   EtherSurfaceMap *next;
};

EAPI EtherSurfaceMap  *ether_surface_map_create        (int n);
EAPI EtherSurfaceMap  *ether_surface_map_list_get      (void);
EAPI int               ether_surface_map_count_entries (EtherSurfaceMap *map);
EAPI int               ether_surface_map_surface_set   (EtherSurfaceMap *map, int surfnum, EtherSurface *surf);
EAPI EtherSurface     *ether_surface_map_surface_get   (EtherSurfaceMap *map, int surfnum);
EAPI EtherSurfaceMap  *ether_surface_map_next_get      (EtherSurfaceMap *map);

#endif /* __ETHER_SURFACE_H__ */
