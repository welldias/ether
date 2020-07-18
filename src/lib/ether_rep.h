#ifndef __ETHER_REP_H__
#define __ETHER_REP_H__

typedef enum _Ether_Sorting_Type
{
	ETHER_SORT_UNDEF = -1,
	ETHER_SORT_NONE = 0, 
	ETHER_SORT_FARTHEST, 
	ETHER_SORT_NEAREST,
	ETHER_SORT_AVERAGE, 
	ETHER_SORT_OTHER
} Ether_Sorting_Type;

struct _Ether_Edge
{ 
   int v1, v2; 
};

struct _Ether_Rep
{
   EtherScalar size;           /**< size (in pixels) at which to use this rep */
   EtherRep *next;             /**< next less-detailed rep */
   Ether_Sorting_Type sorttype; /**< type of poly sorting to do on this rep */
   int nvertices;               /**< number of vertices (and normals if present) */
   EtherVector *vertices;      /**< array of vertices */
   EtherVector *normals;       /**< array of vertex normals; can be NULL */
   EtherFacet *facets;         /**< facets are kept in a linked list */
   EtherEdge *edges;
};

EAPI EtherRep            *ether_rep_create                (int nvertices, int has_normals);
EAPI EtherRep            *ether_rep_init                  (EtherRep *rep, int nvertices, int has_normals);
EAPI void                 ether_rep_add_facet             (EtherRep *rep, EtherFacet *facet);
EAPI void                 ether_rep_traverse_vertices     (EtherRep *rep, int (*function)(EtherVector *vertex, EtherVector *normal));
EAPI void                 ether_rep_traverse_facets       (EtherRep *rep, int (*function)(EtherFacet *facet));
EAPI EtherFacet          *ether_rep_facet_get             (EtherRep *rep, int n);
EAPI EtherFacet          *ether_rep_find_facet            (EtherRep *rep, unsigned int id);
EAPI int                  ether_rep_count_facets          (EtherRep *rep);
EAPI void                 ether_rep_compute_vertex_normals(EtherRep *rep);
EAPI void                 ether_rep_build_edges           (EtherRep *rep);
EAPI void                 ether_rep_sorting_set           (EtherRep *rep, Ether_Sorting_Type type);
EAPI Ether_Sorting_Type   ether_rep_sorting_get           (EtherRep *rep);
EAPI EtherScalar          ether_rep_size_get              (EtherRep *rep); 
EAPI int                  ether_rep_count_vertices        (EtherRep *rep); 
EAPI EtherVector         *ether_rep_vertex_get            (EtherRep *rep, int n, EtherVector vector); 
EAPI EtherVector         *ether_rep_vertex_set            (EtherRep *rep, int n, EtherVector vector); 
EAPI int                  ether_rep_has_normals           (EtherRep *rep);
EAPI EtherVector         *ether_rep_normals_get           (EtherRep *rep, int n, EtherVector vector);
EAPI EtherRep            *ether_rep_next_get              (EtherRep *rep);

#endif /* __ETHER_REP_H__ */
