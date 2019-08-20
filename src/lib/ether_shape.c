#include <Ether.h>
#include "ether_private.h"

static EtherShape *_ether_shape_list = NULL;

static void _ether_shape_recursive_recompute_normals (EtherFacet *facet, EtherVector *vertices);
static void _ether_shape_linear_recompute_normals (EtherFacet *facet, EtherVector *vertices);

EtherShape *ether_shape_create()
{
  EtherShape *shape;
  shape = malloc(sizeof(EtherShape));
  if(shape == NULL)
    return NULL;

  return ether_shape_init(shape);
}

EtherShape *ether_shape_init(EtherShape *shape)
{
	if(shape == NULL)
		return NULL;

	ether_vector_zero(shape->center);
	ether_vector_zero(shape->minbound);
	ether_vector_zero(shape->maxbound);
	shape->radius = 0;
	shape->default_surfacemap = NULL;
	shape->replist = NULL;
	shape->name = "No Name";
	shape->next = _ether_shape_list;
	_ether_shape_list = shape;
	return shape;
}

void ether_shape_compute_bounds(EtherShape *shape)
{
	EtherScalar minx, maxx, miny, maxy, minz, maxz; /* bounding box */
	EtherVector farthest_point;  /* vector from center to farthest point */
	EtherRep *rep;
	int i;

	if(shape == NULL || shape->replist == NULL)
		return;

	minx = maxx = shape->replist->vertices[0][X];
	miny = maxy = shape->replist->vertices[0][Y];
	minz = maxz = shape->replist->vertices[0][Z];
	for(rep = shape->replist; rep; rep = rep->next)
	{
		for(i = 0; i < rep->nvertices; ++i)
		{
			if(rep->vertices[i][X] < minx) 
				minx = rep->vertices[i][X];
			if(rep->vertices[i][Y] < miny) 
				miny = rep->vertices[i][Y];
			if(rep->vertices[i][Z] < minz) 
				minz = rep->vertices[i][Z];
			if(rep->vertices[i][X] > maxx) 
				maxx = rep->vertices[i][X];
			if(rep->vertices[i][Y] > maxy) 
				maxy = rep->vertices[i][Y];
			if(rep->vertices[i][Z] > maxz) 
				maxz = rep->vertices[i][Z];
		}
	}

	/* store the box's bounds */
	shape->minbound[X] = minx;
	shape->minbound[Y] = miny;
	shape->minbound[Z] = minz;
	shape->maxbound[X] = maxx;
	shape->maxbound[Y] = maxy;
	shape->maxbound[Z] = maxz;
	
	/* center of bounding sphere is center of bounding box */
	shape->center[X] = (maxx - minx) / 2 + minx;
	shape->center[Y] = (maxy - miny) / 2 + miny;
	shape->center[Z] = (maxz - minz) / 2 + minz;
	
	/* not the smartest bounding sphere in the world, but it works */
	ether_vector_sub(farthest_point, shape->maxbound, shape->center);
	shape->radius = ether_vector_magnitude(farthest_point);
}

void ether_shape_rescale(EtherShape *shape, float sx, float sy, float sz)
{
	EtherRep *rep;
	int i;
	if(shape == NULL)
		return;
	for(rep = shape->replist; rep; rep = rep->next)
		for (i = 0; i < rep->nvertices; ++i)
		{
			rep->vertices[i][X] *= sx;
			rep->vertices[i][Y] *= sy;
			rep->vertices[i][Z] *= sz;
		}
	ether_shape_update(shape);
}

void ether_shape_offset(EtherShape *shape, EtherScalar tx, EtherScalar ty, EtherScalar tz)
{
	EtherRep *rep;
	int i;
	if(shape == NULL)
		return;
	for(rep = shape->replist; rep; rep = rep->next)
		for (i = 0; i < rep->nvertices; ++i)
		{
			rep->vertices[i][X] += tx;
			rep->vertices[i][Y] += ty;
			rep->vertices[i][Z] += tz;
		}
	ether_shape_update(shape);
}

void ether_shape_update(EtherShape *shape)
{
	EtherRep *rep;
	if(shape == NULL)
		return;
	for(rep = shape->replist; rep; rep = rep->next)
	{
		if (rep->sorttype == ETHER_SORT_OTHER)  /* probably a BSP tree */
			_ether_shape_recursive_recompute_normals(rep->facets, rep->vertices);
		else
			_ether_shape_linear_recompute_normals(rep->facets, rep->vertices);
	}
	ether_shape_compute_bounds(shape);
}

void ether_shape_transform(EtherMatrix matrix, EtherShape *shape)
{
	EtherRep *rep;
	int i;
	EtherVector v;
	for(rep = shape->replist; rep; rep = rep->next)
	{
		for(i = 0; i < rep->nvertices; ++i)
		{
			_ether_transform(v, matrix, rep->vertices[i]);
			v[X] = ether_math_scalar_round(v[X]);
			v[Y] = ether_math_scalar_round(v[Y]);
			v[Z] = ether_math_scalar_round(v[Z]);
			ether_vector_copy(rep->vertices[i], v);
		}
	}
	ether_shape_update(shape);
}

EtherRep *ether_shape_rep_get(EtherShape *shape, EtherScalar size)
{
	EtherRep *rep;
	if(shape == NULL)
		return NULL;
	for(rep = shape->replist; rep; rep = rep->next)
		if (size > rep->size)
			break;
	return rep;
}

void ether_shape_add_rep(EtherShape *shape, EtherRep *rep, EtherScalar size)
{
	EtherRep *p;
	if(shape == NULL || rep == NULL)
		return;
	p = shape->replist;
	rep->size = size;
	if(p == NULL)  /* rep is the only one */
	{
		rep->next = NULL;
		shape->replist = rep;
		return;
	}
	if (rep->size > p->size)  /* rep is bigger than biggest one */
	{
		rep->next = p;
		shape->replist = rep;
		return;
	}
	for (p = shape->replist; p->next; p = p->next)
		if (rep->size > p->next->size)  /* we're bigger than p->next */
		{
			rep->next = p->next;
			p->next = rep;
			return;
		}
	/* if we make it this far, we're the smallest rep so far */
	rep->next = NULL;
	p->next = rep;
}

void ether_shape_traverse_reps(EtherShape *shape, int (*function)(EtherRep *rep))
{
	EtherRep *r;
	if(shape == NULL)
		return;
	for(r = shape->replist; r; r = r->next)
		if((*function)(r))
			return;
}

int ether_shape_count_reps(EtherShape *shape)
{
	int n = 0;
	EtherRep *rep;
	if(shape == NULL)
		return 0;
	for (rep = shape->replist; rep; rep = rep->next)
		++n;
	return n;
}

EtherShape *ether_shape_list_get(void)
{
	return _ether_shape_list;
}

EtherShape *ether_shape_find(char *name)
{
	EtherShape *p;
	for(p = _ether_shape_list; p; p = p->next)
		if(!strcasecmp(name, p->name))
			return p;
	return NULL;
}

void ether_shape_compute_vertex_normals(EtherShape *shape)
{
	EtherRep *rep;
	for(rep = shape->replist; rep; rep = rep->next)
		ether_rep_compute_vertex_normals(rep);
}

EtherScalar ether_shape_radius_get(EtherShape *shape)
{
	if(shape)
		return shape->radius;
	return 0;
}

EtherVector *ether_shape_center_get(EtherShape *shape, EtherVector vector)
{
	if(shape)
		return ether_vector_copy(vector, shape->center);
	return NULL;
}

EtherVector *ether_shape_min_bounds_get(EtherShape *shape, EtherVector vector)
{
	if(shape)
		return ether_vector_copy(vector, shape->minbound);
	return NULL;
}

EtherVector *ether_shape_max_bounds_get(EtherShape *shape, EtherVector vector)
{
	if(shape)
		return ether_vector_copy(vector, shape->maxbound);
	return NULL;
}

void ether_shape_surface_map_set(EtherShape *shape, EtherSurfaceMap *map)
{
	if(shape)
		shape->default_surfacemap = map;
}

EtherSurfaceMap *ether_shape_surface_map_get(EtherShape *shape)
{
	if(shape)
		return shape->default_surfacemap;
	return NULL;
}

void ether_shape_name_set(EtherShape *shape, char *name)
{
	if(shape)
		shape->name = name;
}

char *ether_shape_name_get(EtherShape *shape)
{
	if(shape)
		return shape->name;
	return NULL;
}

EtherShape *ether_shape_next_get(EtherShape *shape)
{
	if(shape)
		return shape->next;
	return NULL;
}

EtherRep *ether_shape_first_rep_get(EtherShape *shape)
{
	if(shape)
		return shape->replist;
	return NULL;
}

static void _ether_shape_recursive_recompute_normals(EtherFacet *facet, EtherVector *vertices)
{
	if(facet->nearside) 
		_ether_shape_recursive_recompute_normals(facet->nearside, vertices);
	if(facet->farside)
		_ether_shape_recursive_recompute_normals(facet->farside, vertices);
	ether_facet_compute_normal(facet, vertices);
}

static void _ether_shape_linear_recompute_normals(EtherFacet *facet, EtherVector *vertices)
{
	EtherFacet *f;
	for(f = facet; f; f = f->farside)
		ether_facet_compute_normal(f, vertices);
}
