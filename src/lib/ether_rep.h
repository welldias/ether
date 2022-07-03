#ifndef __ETHER_REP_H__
#define __ETHER_REP_H__

#include "ether_defines.h"
#include "ether_vector.h"

EAPI EtherRep                *ether_rep_create                (int nvertices, int has_normals);
EAPI EtherRep                *ether_rep_init                  (EtherRep *rep, int nvertices, int has_normals);
EAPI void                     ether_rep_add_facet             (EtherRep *rep, EtherFacet *facet);
EAPI void                     ether_rep_traverse_vertices     (EtherRep *rep, int (*function)(EtherVector *vertex, EtherVector *normal));
EAPI void                     ether_rep_traverse_facets       (EtherRep *rep, int (*function)(EtherFacet *facet));
EAPI EtherFacet              *ether_rep_facet_get             (EtherRep *rep, int n);
EAPI EtherFacet              *ether_rep_find_facet            (EtherRep *rep, unsigned int id);
EAPI int                      ether_rep_count_facets          (EtherRep *rep);
EAPI void                     ether_rep_compute_vertex_normals(EtherRep *rep);
EAPI void                     ether_rep_build_edges           (EtherRep *rep);
EAPI void                     ether_rep_sorting_set           (EtherRep *rep, EtherSortingType type);
EAPI EtherSortingType         ether_rep_sorting_get           (EtherRep *rep);
EAPI EtherScalar              ether_rep_size_get              (EtherRep *rep); 
EAPI int                      ether_rep_count_vertices        (EtherRep *rep); 
EAPI EtherVector             *ether_rep_vertex_get            (EtherRep *rep, int n, EtherVector vector); 
EAPI EtherVector             *ether_rep_vertex_set            (EtherRep *rep, int n, EtherVector vector); 
EAPI int                      ether_rep_has_normals           (EtherRep *rep);
EAPI EtherVector             *ether_rep_normals_get           (EtherRep *rep, int n, EtherVector vector);
EAPI EtherRep                *ether_rep_next_get              (EtherRep *rep);

#endif /* __ETHER_REP_H__ */
