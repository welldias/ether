#include <Ether.h>

static unsigned int _ether_rep_find_id;
static int          _ether_rep_facet_count;
static int          _ether_rep_npoints_count;
static int          _ether_rep_nedges_added;
static EtherFacet  *_ether_rep_found_facet;
static EtherRep    *_ether_rep_this_rep;
static double       _ether_rep_normal_x;
static double       _ether_rep_normal_y;
static double       _ether_rep_normal_z;
static int          _ether_rep_vertex_num;

static void         _ether_rep_recursive_traversal    (EtherFacet *facet, int (*function)(EtherFacet *f));
static int          _ether_rep_find_facet             (EtherFacet *facet);
static int          _ether_rep_facet_counter          (EtherFacet *facet);
static int          _ether_rep_facet_npoints_counter  (EtherFacet *facet);
static void         _ether_rep_insert_edge            (int v1, int v2);
static int          _ether_rep_add_edge               (EtherFacet *facet);
static int          _ether_rep_add_normals            (EtherFacet *f);

EtherRep *
ether_rep_create(int nvertices, int has_normals)
{
	EtherRep *rep;
	rep = malloc(sizeof(EtherRep));
	if(rep == NULL)
		return NULL;
	return ether_rep_init(rep, nvertices, has_normals);
}

EtherRep *
ether_rep_init(EtherRep *rep, int nvertices, int has_normals)
{
	if(rep == NULL)
		return NULL;
	rep->size = 0;
	rep->sorttype = ETHER_SORT_FARTHEST; 
	rep->nvertices = nvertices;
	rep->vertices = calloc(nvertices, sizeof(EtherVector));
	if(rep->vertices == NULL)
		return NULL;
	rep->normals = NULL;
	if(has_normals)
		rep->normals = calloc(nvertices, sizeof(EtherVector));
	rep->facets = NULL;
	rep->next = NULL;
	rep->edges = NULL;
	return rep;
}

void
ether_rep_add_facet(EtherRep *rep, EtherFacet *facet)
{
	if(rep == NULL || facet == NULL)
		return;
	facet->farside = rep->facets;
	rep->facets = facet;
}

void
ether_rep_traverse_vertices(EtherRep *rep, int (*function)(EtherVector *vertex, EtherVector *normal))
{
	int i;
	if(rep == NULL)
		return;
	for(i = 0; i < rep->nvertices; ++i)
		if ((*function)(&rep->vertices[i], rep->normals ? &rep->normals[i] : NULL))
			return;
}

void
ether_rep_traverse_facets(EtherRep *rep, int (*function)(EtherFacet *facet))
{
	if(rep == NULL || rep->facets == NULL)
		return;
	if(rep->sorttype == ETHER_SORT_OTHER)
		_ether_rep_recursive_traversal(rep->facets, function);
	else  /* linear traversal */
	{
		EtherFacet *f;
		for(f = rep->facets; f; f = f->farside)
			if((*function)(f))
				return;
	}
}

EtherFacet *
ether_rep_facet_get(EtherRep *rep, int n)
{
	int i;
	EtherFacet *f;
	if(rep == NULL)
		return NULL;
	i = 0;
	for(f = rep->facets; f; f = f->farside)
		if (i++ == n)
			break;
	return f;
}

EtherFacet *
ether_rep_find_facet(EtherRep *rep, unsigned int id)
{
	if(rep == NULL)
		return NULL;
	_ether_rep_find_id = id;
	_ether_rep_found_facet = NULL;
	ether_rep_traverse_facets(rep, _ether_rep_find_facet);
	return _ether_rep_found_facet;
}

int
ether_rep_count_facets(EtherRep *rep)
{
	_ether_rep_facet_count = 0;
	ether_rep_traverse_facets(rep, _ether_rep_facet_counter);
	return _ether_rep_facet_count;
}

void
ether_rep_compute_vertex_normals(EtherRep *rep)
{
	float mag;
    if(rep == NULL)
        return;
	if(rep->normals == NULL)
		rep->normals = calloc(rep->nvertices, sizeof(EtherVector));
	for(_ether_rep_vertex_num = 0; _ether_rep_vertex_num < rep->nvertices; ++_ether_rep_vertex_num)
	{
		/* add up and count all the facet normals for facets sharing this vertex */
		_ether_rep_normal_x = _ether_rep_normal_y = _ether_rep_normal_z = 0;
		ether_rep_traverse_facets(rep, _ether_rep_add_normals);
		mag = (float)sqrt(_ether_rep_normal_x * _ether_rep_normal_x + 
			_ether_rep_normal_y * _ether_rep_normal_y + 
			_ether_rep_normal_z * _ether_rep_normal_z);
		if(mag)
		{
			rep->normals[_ether_rep_vertex_num][X] = ETHER_DOUBLE_TO_FACTOR(_ether_rep_normal_x / mag);
			rep->normals[_ether_rep_vertex_num][Y] = ETHER_DOUBLE_TO_FACTOR(_ether_rep_normal_y / mag);
			rep->normals[_ether_rep_vertex_num][Z] = ETHER_DOUBLE_TO_FACTOR(_ether_rep_normal_y / mag);
		}
		else
		{
			rep->normals[_ether_rep_vertex_num][X] = 0;
			rep->normals[_ether_rep_vertex_num][Y] = 0;
			rep->normals[_ether_rep_vertex_num][Z] = 0;
		}
	}
}

void
ether_rep_build_edges(EtherRep *rep)
{
	_ether_rep_npoints_count = 0;
	ether_rep_traverse_facets(rep, _ether_rep_facet_npoints_counter);
	rep->edges = calloc(_ether_rep_npoints_count / 2, sizeof(EtherEdge));
	if(rep->edges == NULL)
		return;
	_ether_rep_this_rep = rep;
	_ether_rep_nedges_added = 0;
	ether_rep_traverse_facets(rep, _ether_rep_add_edge);
}

void
ether_rep_sorting_set(EtherRep *rep, Ether_Sorting_Type type)
{
	if(rep == NULL)
		return;
	rep->sorttype = type;
}

Ether_Sorting_Type
ether_rep_sorting_get(EtherRep *rep)
{
	if(rep == NULL)
		return ETHER_SORT_UNDEF;
	return rep->sorttype;
}

EtherScalar
ether_rep_size_get(EtherRep *rep)
{
	if(rep == NULL)
		return 0;
	return rep->size;
}

int
ether_rep_count_vertices(EtherRep *rep)
{
	if(rep == NULL)
		return 0;
	return rep->nvertices;
}

EtherVector *                       
ether_rep_vertex_get(EtherRep *rep, int n, EtherVector vector)
{
	if(rep == NULL)
		return NULL;
	return ether_vector_copy(vector, rep->vertices[n]);
}

EtherVector *                        
ether_rep_vertex_set(EtherRep *rep, int n, EtherVector vector)
{
	if(rep == NULL)
		return NULL;
	return ether_vector_copy(rep->vertices[n], vector);
}

int 
ether_rep_has_normals(EtherRep *rep)
{
	if(rep == NULL || rep->normals == NULL)
		return 0;
	return 1;
}

EtherVector *  
ether_rep_normals_get(EtherRep *rep, int n, EtherVector vector)
{
	if(rep == NULL)
		return NULL;
	return ether_vector_copy(vector, (rep)->normals[n]);
}

EtherRep *
ether_rep_next_get(EtherRep *rep)
{
	if(rep == NULL)
		return NULL;
	return rep->next;
}

static void 
_ether_rep_recursive_traversal(EtherFacet *facet, int (*function)(EtherFacet *f))
{
	if(facet->nearside)
		_ether_rep_recursive_traversal(facet->nearside, function);
	(*function)(facet);
	if(facet->farside)
		_ether_rep_recursive_traversal(facet->farside, function);
}

static int 
_ether_rep_find_facet(EtherFacet *facet)
{
	if(facet && facet->id == _ether_rep_find_id)
	{
		_ether_rep_found_facet = facet;
		return 1;
	}
	return 0;
}

static int _ether_rep_facet_counter(EtherFacet *facet)
{
	if (facet)
		++_ether_rep_facet_count;
	return 0;
}

static int 
_ether_rep_facet_npoints_counter(EtherFacet *facet)
{
	if(facet)
		_ether_rep_npoints_count += facet->npoints;
	return 0;
}

static void 
_ether_rep_insert_edge(int v1, int v2)
{
	int i;
	for(i = 0; i < _ether_rep_nedges_added; ++i)
		if((v1 == _ether_rep_this_rep->edges[i].v1 && v2 == _ether_rep_this_rep->edges[i].v2)
			|| (v1 == _ether_rep_this_rep->edges[i].v2 && v2 == _ether_rep_this_rep->edges[i].v1))
			return;
	_ether_rep_this_rep->edges[_ether_rep_nedges_added].v1 = v1;
	_ether_rep_this_rep->edges[_ether_rep_nedges_added].v2 = v2;
	++_ether_rep_nedges_added;
}

static int 
_ether_rep_add_edge(EtherFacet *facet)
{
	int i;
	if(facet->edges == NULL)
	{
		facet->edges = calloc(facet->npoints, sizeof(int));
		if(facet->edges == NULL)
			return 1;
	}
	for(i = 0; i < facet->npoints - 1; ++i)
	{
		_ether_rep_insert_edge(facet->points[i], facet->points[i+1]);
		facet->edges[i] = _ether_rep_nedges_added-1;
	}
	_ether_rep_insert_edge(facet->points[i], facet->points[0]);
	facet->edges[i] = _ether_rep_nedges_added-1;
	return 0;
}

static int _ether_rep_add_normals(EtherFacet *f)
{
	EtherVector v;
	int n, i;
	n = ether_facet_count_points(f);
	for(i = 0; i < n; ++i)
		if(ether_facet_point_get(f, i) == _ether_rep_vertex_num)
			break;
	if(i >= n)
		return 0;
	ether_facet_normal_get(f, v);
	_ether_rep_normal_x += ETHER_FACTOR_TO_DOUBLE(v[X]);
	_ether_rep_normal_y += ETHER_FACTOR_TO_DOUBLE(v[Y]);
	_ether_rep_normal_z += ETHER_FACTOR_TO_DOUBLE(v[Z]);
	return 0;
}
