#ifndef __ETHER_PRIMITIVES_H__
#define __ETHER_PRIMITIVES_H__

/* Ether currently provides five utility routines for creating simple geometric 
 * primitives. Each takes a surface map pointer; if the value is NULL, 
 * the default color for geometric primitives is used.
 *
 */

EAPI EtherShape	*ether_primitives_box		(EtherScalar width, EtherScalar height, EtherScalar depth, EtherSurfaceMap *map);
EAPI EtherShape *ether_primitives_triangle  (EtherScalar width, EtherScalar height, EtherSurfaceMap *map);
EAPI EtherShape	*ether_primitives_cone		(EtherScalar radius, EtherScalar height, int nsides, EtherSurfaceMap *map);
EAPI EtherShape	*ether_primitives_cylinder	(EtherScalar bottom_radius, EtherScalar top_radius, EtherScalar height, int nsides, EtherSurfaceMap *map);
EAPI EtherShape	*ether_primitives_prism		(EtherScalar width, EtherScalar height, EtherScalar depth, EtherSurfaceMap *map);
EAPI EtherShape	*ether_primitives_sphere	(EtherScalar radius, int vsides, int hsides, EtherSurfaceMap *map);

#endif /* __ETHER_PRIMITIVES_H__ */
