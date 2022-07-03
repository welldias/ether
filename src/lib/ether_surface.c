#include <stdlib.h>

#include "ether_surface.h"

static EtherSurface     *_ether_surfaces = NULL;
static EtherSurfaceMap  *_ether_maps     = NULL;

EtherSurface *
ether_surface_init(EtherSurface *surf)
{
    if (surf == NULL) return NULL;
    surf->type = ETHER_SURF_FLAT;
    surf->hue = 0;
    surf->brightness = 128;
    surf->exponent = 0;
    surf->next = _ether_surfaces;
    _ether_surfaces = surf;
    return surf;
}

EtherSurface *
ether_surface_from_desc(unsigned short int desc, EtherSurface *surf)
{
    if (surf == NULL) return NULL;
    surf->type = (desc >> 12) & 0x0F;
    surf->hue = (desc >> 8) & 0x0F;
    surf->brightness = desc & 0xFF;
    if (surf->type == ETHER_SURF_SIMPLE && surf->hue != 0)
    {
        surf->hue = 0;
        surf->brightness = (surf->hue << 4) | ((surf->brightness >> 4) & 0x0F);
    }
    if (surf->type == ETHER_SURF_SPECULAR)
    {
        surf->exponent = surf->brightness & 0x0F;
        surf->brightness &= 0xF0;
    }
    return surf;
}

unsigned short int
ether_surface_to_desc(EtherSurface *surf)
{
    unsigned short int desc;
    desc = (surf->type << 12) | (surf->hue << 8) | (surf->brightness);
    if (surf->type == ETHER_SURF_SPECULAR)
        desc |= surf->exponent;
    return desc;
}

EtherSurface *
ether_surface_list_get(void)
{
    return _ether_surfaces;
}

EtherSurface *
ether_surface_create(unsigned char hue)
{
    EtherSurface *s = ether_surface_init(malloc(sizeof(EtherSurface)));
    if (s == NULL) return NULL;
    s->hue = hue;
    return s;
}

void
ether_surface_type_set(EtherSurface *surf, EtherSurfaceType type)
{
    if(surf)
       surf->type = type;
}

EtherSurfaceType
ether_surface_type_get(EtherSurface *surf)
{
    if(surf)
       return surf->type;
    else
        return 0;
}

void
ether_surface_hue_set(EtherSurface *surf, unsigned char hue)
{
    if(surf)
       surf->hue = hue;
}

unsigned char
ether_surface_hue_get(EtherSurface *surf)
{
    if(surf)
       return surf->hue;
    else
        return 0;
}

void
ether_surface_brightness_set(EtherSurface *surf, unsigned char brightness)
{
    if(surf)
       surf->brightness = brightness;
}

unsigned char
ether_surface_brightness_get(EtherSurface *surf)
{
    if(surf)
       return surf->brightness;
    else
        return 0;
}

void
ether_surface_exponent_set(EtherSurface *surf, unsigned char exponent)
{
    if(surf)
       surf->exponent = exponent;
    
}

unsigned char 
ether_surface_exponent_get(EtherSurface *surf)
{
     if(surf)
       return surf->exponent;
    else
        return 0;
}

EtherSurface *
ether_surface_next_get(EtherSurface *surf)
{
    if(surf)
       return surf->next;
    else
        return NULL;   
}

/* Ether_Suface_Map */

EtherSurfaceMap *
ether_surface_map_create(int n)
{
    int i;
    EtherSurfaceMap *m = malloc(sizeof(EtherSurfaceMap));
    if (m == NULL) return NULL;
    m->nentries = n;
    m->entries = malloc(n*sizeof(EtherSurface *));
    if (m->entries == NULL)
    {
        if(m)
            free(m);
        return NULL;
    }
    for (i = 0; i < n; ++i)
        m->entries[i] = NULL;
    m->next = _ether_maps;  
    _ether_maps = m;
    return m;
}

EtherSurfaceMap *
ether_surface_map_list_get(void)
{
    return _ether_maps;
}

int
ether_surface_map_count_entries (EtherSurfaceMap *map)
{
    if(map)
        return map->nentries;
    else
        return 0;
}

int
ether_surface_map_surface_set(EtherSurfaceMap *map, int surfnum, EtherSurface *surf)
{
    if(map && map->nentries > surfnum)
    {
        map->entries[surfnum] = surf;
        return 1;
    }
    else
        return 0;
}

EtherSurface *
ether_surface_map_surface_get(EtherSurfaceMap *map, int surfnum)
{
    if(map && map->nentries > surfnum)
        return map->entries[surfnum];
    else
        return NULL;
    
}

EtherSurfaceMap *
ether_surface_map_next_get(EtherSurfaceMap *map)
{
    if(map)
        return map->next;
    else
        return NULL;
}
