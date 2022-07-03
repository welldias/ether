#ifndef __ETHER_FACET_H__
#define __ETHER_FACET_H__

#include "ether_defines.h"

EAPI EtherFacet   *ether_face_init             (EtherFacet *facet, int npts);
EAPI void          ether_facet_traverse        (EtherFacet *facet, int (*function)(EtherFacet *f));
EAPI void          ether_facet_compute_normal  (EtherFacet *facet, EtherVector *vertices);
EAPI EtherFacet   *ether_facet_create          (int npts);
EAPI void          ether_facet_surf_num_set    (EtherFacet *facet, int n);
EAPI int           ether_facet_surf_num_get    (EtherFacet *facet);
EAPI int           ether_facet_count_points    (EtherFacet *facet);
EAPI void          ether_facet_vertex_get      (EtherRep *rep, EtherFacet *facet, int n, EtherVector v);
EAPI void          ether_facet_highligth_set   (EtherFacet *facet, int high);
EAPI int           ether_facet_highligth_get   (EtherFacet *facet);
EAPI void          ether_facet_toggle_highligth(EtherFacet *facet);
EAPI void          ether_facet_interior_set    (EtherFacet *facet, int inter);
EAPI int           ether_facet_interior_get    (EtherFacet *facet);
EAPI void          ether_facet_toggle_interior (EtherFacet *facet);
EAPI void          ether_facet_id_set          (EtherFacet *facet, unsigned int n);
EAPI unsigned int  ether_facet_id_get          (EtherFacet *facet);
EAPI void          ether_facet_normal_get      (EtherFacet *facet, EtherVector v);
EAPI void          ether_facet_point_set       (EtherFacet *facet, int n, int m);
EAPI int           ether_facet_point_get       (EtherFacet *facet, int n);   

#endif /* __ETHER_FACET_H__ */
