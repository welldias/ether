#include <stdlib.h>

#include "ether_facet.h"
#include "ether_vector.h"

EtherFacet * ether_facet_create(int npts)
{
    EtherFacet * facet = malloc(sizeof(EtherFacet));
    ether_face_init(facet, npts);
    return facet;
}

EtherFacet * ether_face_init(EtherFacet *facet, int npts)
{
    if (facet == NULL) 
        return NULL;

    facet->surface = 0;  
    facet->id = 0;
    facet->highlight = 0;  
    facet->interior = 0;
    facet->details = NULL;  
    facet->nearside = NULL;
    facet->farside = NULL;
    facet->npoints = npts;
    facet->edges = NULL;
    if (npts)
    {
        facet->points = calloc(npts, sizeof(int));
        if (facet->points == NULL) 
            return NULL;
    }

    return facet;
}

void ether_facet_traverse(EtherFacet *facet, int (*function)(EtherFacet *f))
{
    if(facet->nearside)
        ether_facet_traverse(facet->nearside, function);
    if((*function)(facet))
        return;
    if(facet->farside)
        ether_facet_traverse(facet->farside, function);
}

void ether_facet_compute_normal(EtherFacet *facet, EtherVector *vertices)
{
    EtherScalar maxmag = 0;
    int i;
    facet->normal[X] = ETHER_UNITY;  
    facet->normal[Y] = 0;
    facet->normal[Z] = 0;
    if(facet->npoints < 3)
        return;
    for(i = 0; i < facet->npoints; ++i)
    {
        EtherVector v1, v2, tmp;
        EtherScalar m;
        int first  = facet->points[i];
        int second = facet->points[(i+1) % facet->npoints];
        int third  = facet->points[(i+2) % facet->npoints];
        ether_vector_sub(v1, vertices[third], vertices[second]);
        ether_vector_sub(v2, vertices[first], vertices[second]);
        m = ether_vector_crossproduct(tmp, v1, v2);
        if (m > maxmag)
        {
            maxmag = m;
            ether_vector_copy(facet->normal, tmp);
        }
     }
}

void ether_facet_surf_num_set(EtherFacet *facet, int n)
{
    if(facet)
        facet->surface = n;
}

int ether_facet_surf_num_get(EtherFacet *facet)
{   
    if(facet)
        return facet->surface;
    else
        return 0;
}

int ether_facet_count_points(EtherFacet *facet)
{
    if(facet)
     return facet->npoints;
    else
        return 0;
}

void ether_facet_vertex_get(EtherRep *rep, EtherFacet *facet, int n, EtherVector v)
{
    if(rep && facet && facet->npoints > n)
        ether_vector_copy(v, rep->vertices[facet->points[n]]);
}

void ether_facet_highligth_set(EtherFacet *facet, int high)
{
    if(facet)
        facet->highlight = ((high) ? 1 : 0);
}

int ether_facet_highligth_get(EtherFacet *facet)
{
    if(facet)
        return facet->highlight;
    else
        return 0;
}

void ether_facet_toggle_highligth(EtherFacet *facet)
{
    if(facet)
        facet->highlight = !(facet->highlight);
}

void
ether_facet_interior_set(EtherFacet *facet, int inter)
{
    if(facet)
        facet->interior = ((inter) ? 1 : 0);
}

int
ether_facet_interior_get(EtherFacet *facet)
{
    if(facet)
        return facet->interior;
    else
        return 0;
}

void
ether_facet_toggle_interior(EtherFacet *facet)
{
    if(facet)
        facet->interior = !(facet->interior);
}

void
ether_facet_id_set(EtherFacet *facet, unsigned int n)
{
    if(facet)
        facet->id = n;
}

unsigned int
ether_facet_id_get(EtherFacet *facet)
{
    if(facet)
        return facet->id;
    else
        return 0;
}

void ether_facet_normal_get(EtherFacet *facet, EtherVector v)
{
    if(facet)
        ether_vector_copy(v, facet->normal);
}

void ether_facet_point_set(EtherFacet *facet, int n, int m)
{
    if(facet && facet->npoints > n)
        facet->points[n] = m;
}

int ether_facet_point_get(EtherFacet *facet, int n)
{
    if(facet && facet->npoints > n)
     return facet->points[n];
    else
        return 0;
}
