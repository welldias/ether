#include <Ether.h>

static EtherSurface _ether_primivites_defsurf = 
{ 
	ETHER_SURF_FLAT, 
	1, 
	255 
};

static int _ether_primivites_cube_points[6][4] =
{
	{ 0, 4, 5, 1 },
	{ 4, 7, 6, 5 }, 
	{ 2, 6, 7, 3 },
	{ 1, 5, 6, 2 },
	{ 3, 7, 4, 0 }, 
	{ 0, 1, 2, 3 }
};

static int _ether_primivites_prism_points[5][4] =
{
	{ 5, 4, 3, 2 }, 
	{ 2, 3, 0, 1 }, 
	{ 4, 5, 1, 0 },
	{ 1, 5, 2, 0 }, 
	{ 4, 0, 3, 0 }
};

static int _ether_primivites_triangle_points[3] = { 0, 1, 2 };

static EtherFacet *_ether_primitives_new_facet(int n, int *pts);

EtherShape *
ether_primitives_box(EtherScalar width, EtherScalar height, EtherScalar depth, EtherSurfaceMap *map)
{
	int i;
	EtherVector *verts;
	EtherShape *shape;
	EtherRep *rep;
	shape = ether_shape_create();
	rep   = ether_rep_create(8, 0);
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if(map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	verts[0][X] = verts[3][X] = verts[4][X] = verts[7][X] = -width /2;
	verts[1][X] = verts[2][X] = verts[5][X] = verts[6][X] =  width /2;
	verts[0][Y] = verts[1][Y] = verts[2][Y] = verts[3][Y] = -height/2;
	verts[4][Y] = verts[5][Y] = verts[6][Y] = verts[7][Y] =  height/2;
	verts[0][Z] = verts[1][Z] = verts[4][Z] = verts[5][Z] = -depth /2;
	verts[2][Z] = verts[3][Z] = verts[6][Z] = verts[7][Z] =  depth /2;
	rep->facets = NULL;
	for(i = 0; i < 6; ++i)
	{
		EtherFacet *facet = _ether_primitives_new_facet(4, _ether_primivites_cube_points[i]);
		if(facet == NULL)
			return NULL;
		facet->farside = rep->facets;
		rep->facets = facet;
	}
	ether_shape_update(shape);
	return shape;
}

EtherShape *
ether_primitives_triangle(EtherScalar width, EtherScalar height, EtherSurfaceMap *map)
{
	EtherVector *verts;
	EtherShape *shape;
	EtherRep *rep;
    EtherFacet *facet;
    
	shape = ether_shape_create();
	rep   = ether_rep_create(3, 0);
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if(map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	verts[1][X] = -width /2;
    verts[2][X] = width /2;
	verts[0][Y] = height;
	rep->facets = NULL;

    facet = _ether_primitives_new_facet(3, _ether_primivites_triangle_points);
    if(facet == NULL)
        return NULL;
    facet->farside = rep->facets;
    rep->facets = facet;

	ether_shape_update(shape);
	return shape;
}

EtherShape *
ether_primitives_cone(EtherScalar radius, EtherScalar height, int nsides, EtherSurfaceMap *map)
{
	int i;
	EtherVector *verts, *norms;
	EtherShape *shape;
	EtherRep *rep;
	EtherFacet *facet;
	shape = ether_shape_create();
	rep = ether_rep_create(nsides + 1, 1);
	if(nsides < 3)
		nsides = 3;
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if (map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	norms = rep->normals;
	verts[0][X] = verts[0][Z] = 0;
	verts[0][Y] = height;
	norms[0][X] = 0;
	norms[0][Z] = 0;
	norms[0][Y] = ETHER_UNITY;
	for (i = 1; i <= nsides; ++i)
	{
		verts[i][X] = radius * (EtherScalar)cos((i*2*PI)/nsides);
		verts[i][Y] = 0;
		verts[i][Z] = radius * (EtherScalar)sin((i*2*PI)/nsides);
		norms[i][X] = ETHER_DOUBLE_TO_FACTOR(cos((i*2*PI)/nsides));
		norms[i][Y] = 0;
		norms[i][Z] = ETHER_DOUBLE_TO_FACTOR(sin((i*2*PI)/nsides));
	}
	rep->facets = NULL;
	for(i = 1; i <= nsides; ++i)
	{
		int pts[3];
		pts[0] = i;  pts[1] = 0;  pts[2] = (i == nsides) ? 1 : (i + 1);
		facet = _ether_primitives_new_facet(3, pts);
		if(facet == NULL)
			return NULL;
		facet->farside = rep->facets;
		rep->facets = facet;
	}
	facet = malloc(sizeof(EtherFacet));
	if(facet == NULL)
		return NULL;
	facet->farside = rep->facets;
	rep->facets = facet;
	facet->nearside = NULL;
	facet->details = NULL;
	facet->interior = facet->highlight = 0;
	facet->surface = 0;
	facet->id = 0;
	facet->npoints = nsides;
	facet->points = calloc(nsides, sizeof(int));
	if(facet->points == NULL)
		return NULL;
	facet->edges = NULL;
	for(i = 0; i < nsides; ++i)
		facet->points[i] = i+1;
	ether_shape_update(shape);
	return shape;
}

EtherShape	*
ether_primitives_cylinder(EtherScalar bottom_radius, EtherScalar top_radius, EtherScalar height, int nsides, EtherSurfaceMap *map)
{
	int i;
	EtherVector *verts, *norms;
	EtherShape *shape;
	EtherRep *rep;
	EtherFacet *facet;
    
	shape = ether_shape_create();
	if(top_radius == 0)
		return ether_primitives_cone(bottom_radius, height, nsides, map);
	if(nsides < 3)
		nsides = 3;
	rep = ether_rep_create(nsides * 2, 1);
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if(map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	norms = rep->normals;
	for(i = 0; i < nsides; ++i)
	{
		verts[i][X] = bottom_radius * (EtherScalar)cos((i*2*PI)/nsides);
		verts[i][Y] = 0;
		verts[i][Z] = bottom_radius * (EtherScalar)sin((i*2*PI)/nsides);
		norms[i][X] = ETHER_DOUBLE_TO_FACTOR(cos((i*2*PI)/nsides));
		norms[i][Y] = 0;
		norms[i][Z] = ETHER_DOUBLE_TO_FACTOR(sin((i*2*PI)/nsides));
		verts[i+nsides][X] = top_radius * (EtherScalar)cos((i*2*PI)/nsides);
		verts[i+nsides][Y] = height;
		verts[i+nsides][Z] = top_radius * (EtherScalar)sin((i*2*PI)/nsides);
		norms[i+nsides][X] = ETHER_DOUBLE_TO_FACTOR(cos((i*2*PI)/nsides));
		norms[i+nsides][Y] = 0;
		norms[i+nsides][Z] = ETHER_DOUBLE_TO_FACTOR(sin((i*2*PI)/nsides));
	}
	rep->facets = NULL;
	for(i = 0; i < nsides; ++i)
	{
		int pts[4];
		pts[0] = i;  pts[1] = nsides+i;
		if (i == (nsides-1))
		{
			pts[2] = nsides;
			pts[3] = 0;
		}
		else
		{
			pts[2] = nsides + i + 1;
			pts[3] = i + 1;
		}
		facet = _ether_primitives_new_facet(4, pts);
		if(facet == NULL)
			return NULL;
		facet->farside = rep->facets;
		rep->facets = facet;
	}
	facet = malloc(sizeof(EtherFacet));
	if(facet == NULL)
		return NULL;
	facet->farside = rep->facets;
	rep->facets = facet;
	facet->nearside = NULL;
	facet->details = NULL;
	facet->interior = facet->highlight = 0;
	facet->surface = 0;
	facet->id = 0;
	facet->npoints = nsides;
	facet->points = calloc(nsides, sizeof(int));
	if(facet->points == NULL)
		return NULL;
	facet->edges = NULL;
	for(i = 0; i < nsides; ++i)
		facet->points[i] = i;
	facet = malloc(sizeof(EtherFacet));
	if(facet == NULL)
		return NULL;
	facet->farside = rep->facets;
	rep->facets = facet;
	facet->nearside = NULL;
	facet->details = NULL;
	facet->interior =  0;
	facet->highlight = 0;
	facet->surface = 0;
	facet->id = 0;
	facet->npoints = nsides;
	facet->points = calloc(nsides, sizeof(int));
	if(facet->points == NULL)
		return NULL;
	facet->edges = NULL;
	for(i = 0; i < nsides; ++i)
		facet->points[i] = nsides - i - 1 + nsides;
	ether_shape_update(shape);
	return shape;
}

EtherShape	*
ether_primitives_prism(EtherScalar width, EtherScalar height, EtherScalar depth, EtherSurfaceMap *map)
{
	int i;
	EtherVector *verts;
	EtherShape *shape;
	EtherRep *rep ;
	shape = ether_shape_create();
	rep = ether_rep_create(6, 0);
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if (map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	verts[0][X] = verts[3][X] = verts[4][X] = 0;
	verts[1][X] = verts[2][X] = verts[5][X] = width;
	verts[0][Y] = verts[1][Y] = verts[2][Y] = verts[3][Y] = 0;
	verts[4][Y] = verts[5][Y] = height;
	verts[0][Z] = verts[1][Z] = verts[4][Z] = verts[5][Z] = 0;
	verts[2][Z] = verts[3][Z] = depth;
	rep->facets = NULL;
	for (i = 0; i < 5; ++i)
	{
		EtherFacet *facet = _ether_primitives_new_facet((i < 3) ? 4 : 3, _ether_primivites_prism_points[i]);
		if(facet == NULL)
			return NULL;
		facet->farside = rep->facets;
		rep->facets = facet;
	}
	ether_shape_update(shape);
	return shape;
}

EtherShape	*
ether_primitives_sphere(EtherScalar radius, int vsides, int hsides, EtherSurfaceMap *map)
{
	int i, j;
	EtherVector *verts, *norms;
	EtherShape *shape;
	EtherRep *rep;
	EtherFacet *facet;
	shape = ether_shape_create();
	if(vsides < 3)
		vsides = 3;
	if(hsides < 3)
		hsides = 3;
	rep = ether_rep_create((vsides - 1) * hsides + 2, 1);
	if(shape == NULL || rep == NULL)
		return NULL;
	shape->replist = rep;
	if(map)
		shape->default_surfacemap = map;
	else
	{
		shape->default_surfacemap = ether_surface_map_create(1);
		ether_surface_map_surface_set(shape->default_surfacemap, 0, &_ether_primivites_defsurf);
	}
	ether_rep_sorting_set(rep, ETHER_SORT_NONE);
	verts = rep->vertices;
	norms = rep->normals;
	/* create poles */
	verts[0][X] = verts[0][Z] = verts[1][X] = verts[1][Z] = 0;
	norms[0][X] = norms[0][Z] = norms[1][X] = norms[1][Z] = 0;
	verts[0][Y] = radius;
	verts[1][Y] = -radius;
	norms[0][Y] = ETHER_UNITY;
	norms[1][Y] = -ETHER_UNITY;
	/* create latitudes and longitudes */
	for(i = 1; i < vsides; ++i)
	{
		EtherScalar r =  radius * (EtherScalar)sin((i * PI) / vsides);
		EtherScalar y = -radius * (EtherScalar)cos((i * PI) / vsides);
		for(j = 0; j < hsides; ++j)
		{
			verts[2+(i-1)*hsides+j][X] = r * (EtherScalar)cos((j*2*PI)/hsides);
			verts[2+(i-1)*hsides+j][Y] = y;
			verts[2+(i-1)*hsides+j][Z] = r * (EtherScalar)sin((j*2*PI)/hsides);
			norms[2+(i-1)*hsides+j][X] = r * (EtherScalar)cos((j*2*PI)/hsides);
			norms[2+(i-1)*hsides+j][Y] = y;
			norms[2+(i-1)*hsides+j][Z] = r * (EtherScalar)sin((j*2*PI)/hsides);
			ether_vector_normalize(norms[2+(i-1)*hsides+j]);
		}
	}
	rep->facets = NULL;
	for(i = 1; i < vsides-1; ++i)
		for(j = 0; j < hsides; ++j)
		{
			int pts[4];
			int n = (i-1) * hsides + j + 2;
			pts[0] = n;  pts[1] = n + hsides;
			if(j == (hsides - 1))
			{
				pts[2] = (i) * hsides + 2;
				pts[3] = (i-1) * hsides + 2;
			}
			else
			{
				pts[2] = n + 1 + hsides;
				pts[3] = n + 1;
			}
			facet = _ether_primitives_new_facet(4, pts);
			facet->farside = rep->facets;
			rep->facets = facet;
		}
	/* Triangular polar regions */
	for(i = 0; i < hsides; ++i)
	{
		int pts[3];
		/* north */
		pts[0] = 1;  pts[1] = i + 2;
		pts[2] = ((i == (hsides - 1)) ? 0 : (i + 1)) + 2;
		facet = _ether_primitives_new_facet(3, pts);
		facet->farside = rep->facets;
		rep->facets = facet;
		/* south */
		pts[0] = rep->nvertices - i - 1;  pts[1] = 0;
		pts[2] = (i == 0) ? (rep->nvertices - hsides) : (pts[0] + 1);
		facet = _ether_primitives_new_facet(3, pts);
		facet->farside = rep->facets;
		rep->facets = facet;
	}
	ether_shape_update(shape);
	return shape;
}

static
EtherFacet *_ether_primitives_new_facet(int n, int *pts)
{
	EtherFacet *facet;
	facet = ether_facet_create(n);
	if(facet == NULL)
		return NULL;
	memcpy(facet->points, pts, n * sizeof(int));
	return facet;
}
