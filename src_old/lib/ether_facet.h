#ifndef __ETHER_FACET_H__
#define __ETHER_FACET_H__

struct _Ether_Facet
{
	int surface;                         /**< index into object's surface array */
	EtherVector normal;                  /**< perpendicular to facet, left-hand rule */
	unsigned int id;                     /**< identifier for this facet */
	int highlight : 1;                   /**< facet is highlighted */
	int interior : 1;                    /**< facet is on the interior of an object (not used) */
	EtherFacet* details;                 /**< points to linked list of detail facets (not used) */
	EtherFacet* nearside, * farside;     /**< only farside is used */
	int npoints;                         /**< number of points in the facet */
	int* points;                         /**< indices into array of vertices of the facet points */
	int* edges;                          /**< indices into array edges */
};

EAPI EtherFacet*   ether_face_init(EtherFacet* facet, int npts);
EAPI void          ether_facet_traverse(EtherFacet* facet, int (*function)(EtherFacet* f));
EAPI void          ether_facet_compute_normal(EtherFacet* facet, EtherVector* vertices);
EAPI EtherFacet*   ether_facet_create(int npts);
EAPI void          ether_facet_surf_num_set(EtherFacet* facet, int n);
EAPI int           ether_facet_surf_num_get(EtherFacet* facet);
EAPI int           ether_facet_count_points(EtherFacet* facet);
EAPI void          ether_facet_vertex_get(EtherRep* rep, EtherFacet* facet, int n, EtherVector v);
EAPI void          ether_facet_highligth_set(EtherFacet* facet, int high);
EAPI int           ether_facet_highligth_get(EtherFacet* facet);
EAPI void          ether_facet_toggle_highligth(EtherFacet* facet);
EAPI void          ether_facet_interior_set(EtherFacet* facet, int inter);
EAPI int           ether_facet_interior_get(EtherFacet* facet);
EAPI void          ether_facet_toggle_interior(EtherFacet* facet);
EAPI void          ether_facet_id_set(EtherFacet* facet, unsigned int n);
EAPI unsigned int  ether_facet_id_get(EtherFacet* facet);
EAPI void          ether_facet_normal_get(EtherFacet* facet, EtherVector v);
EAPI void          ether_facet_point_set(EtherFacet* facet, int n, int m);
EAPI int           ether_facet_point_get(EtherFacet* facet, int n);

#endif /* __ETHER_FACET_H__ */
