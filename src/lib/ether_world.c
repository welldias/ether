#include "ether_world.h"
#include "ether_private.h"
#include "ether_palette.h"
#include "ether_object.h"
#include "ether_vector.h"
#include "ether_shape.h"
#include "ether_rep.h"

static int _ether_world_count;
static EtherObject *_ether_world_found_obj;
static char *_ether_world_find_name;
static EtherScalar _ether_world_minx, _ether_world_maxx, _ether_world_miny, _ether_world_maxy, _ether_world_minz, _ether_world_maxz;

static int _ether_world_count_facets(EtherObject *obj);
static int _ether_world_count_objs(EtherObject *obj);
static int _ether_world_find_obj(EtherObject *obj);
static int _ether_world_find_bounds(EtherObject *obj);

static EtherWorld _ether_world = {
    NULL,                        /* objects */
    NULL,                        /* lights */
    NULL,                        /* cameras */
    NULL,                        /* current camera */
    NULL,                        /* left-eye camera */
    NULL,                        /* right-eye camera */
    ETHER_FLOAT_TO_FACTOR(0.5),  /* ambient light */
    ETHER_FLOAT_TO_SCALAR(1),    /* world scale */
    ETHER_FLOAT_TO_SCALAR(100),  /* movestep */
    ETHER_FLOAT_TO_ANGLE(5),     /* turnstep */
    1,                           /* movement mode */
    1,                           /* screenclear */
    1,                           /* horizon */
    { 0x88, 3 },                 /* horizon colors */
    2,                           /* ncolors */
    {{{0}},{{0}},0}              /* palette */
};

EtherWorld *ether_world_current = &_ether_world;   /* the currently active world */

EtherWorld * ether_world_create(void) {
    EtherWorld *w = malloc(sizeof(EtherWorld));
    if(w)
        return ether_world_init(w);
    else
        return NULL;
}

EtherWorld * ether_world_init(EtherWorld *world) {
    if(world == NULL)
        return NULL;

    world->objects = NULL;
    world->lights = NULL;
    world->camera = NULL;
    world->left_camera = NULL;
    world->right_camera = NULL;
    world->ambient = ETHER_FLOAT_TO_FACTOR(0.5);
    world->screenclear = 1;
    world->horizon = 1;
    world->movement_mode = 1;
    world->movestep = ETHER_FLOAT_TO_FACTOR(100);
    world->rotstep = ETHER_FLOAT_TO_ANGLE(5);
    world->horizoncolors[0] = 0x88;  
    world->horizoncolors[1] = 0;
    world->nhorizoncolors = 2;
    world->scale = 1;
    ether_palette_init(&world->palette);

    if(ether_world_current == NULL) 
        ether_world_current = world;

    return world;
}

void ether_world_add_light(EtherLight *light) {

    if(ether_world_current == NULL || light == NULL)
        return;

    light->next = ether_world_current->lights;
    ether_world_current->lights = light;
}

void ether_world_remove_light(EtherLight *light) {
    EtherLight *p;

    if(ether_world_current == NULL || light == NULL) 
        return;

    if(ether_world_current->lights == light)
        ether_world_current->lights = light->next;
    else
        for(p = ether_world_current->lights; p; p = p->next)
            if (p->next == light)
            {
                p->next = light->next;
                break;
            }
    light->next = NULL;
}

EtherLight * ether_world_find_light(char *name) {
    EtherLight *p;
    
    for(p = ether_world_current->lights; p; p = p->next)
		if(!strcasecmp(p->name, name))
			return p;
    return NULL;    
}

void ether_world_add_camera(EtherCamera *camera) {
    if(ether_world_current == NULL || camera == NULL)
        return;
    
    camera->next = ether_world_current->cameras;
    ether_world_current->cameras = camera;
}

void ether_world_remove_camera(EtherCamera *camera) {
    EtherCamera *p;
    
    if(ether_world_current == NULL || camera == NULL)
        return;

    if(ether_world_current->cameras == camera)
        ether_world_current->cameras = camera->next;
    else
        for(p = ether_world_current->cameras; p; p = p->next)
            if (p->next == camera)
            {
                p->next = camera->next;
                break;
            }
    camera->next = NULL;
}

EtherCamera  * ether_world_find_camera(char *name) {
    EtherCamera *p;
    
    for (p = ether_world_current->cameras; p; p = p->next)
        if (!strcasecmp(p->name, name))
            return p;
    return NULL;    
}

void ether_world_add_abject(EtherObject *obj) {
    if(ether_world_current == NULL || obj == NULL)
        return;

    ether_world_remove_abject(obj);
    obj->siblings = ether_world_current->objects;
    ether_world_current->objects = obj;    
}

void ether_world_remove_abject(EtherObject *obj) {
    EtherObject *p;
    
    if(ether_world_current == NULL || obj == NULL)
        return;

    ether_object_detach(obj);
    if(ether_world_current->objects == obj) {
        ether_world_current->objects = obj->siblings;
    } 
    else {
        for(p = ether_world_current->objects; p; p = p->siblings) {
            if (p->siblings == obj)
            {
                p->siblings = obj->siblings;
                break;
            }
        }
    }
    obj->siblings = NULL;
}

EtherObject * ether_world_find_object(char *name) {
    _ether_world_found_obj = NULL;
    _ether_world_find_name = name;
    ether_object_traverse(ether_world_current->objects, _ether_world_find_obj);
    return _ether_world_found_obj;
}

int ether_world_count_objects(void) {
    _ether_world_count = 0;
    ether_object_traverse(ether_world_current->objects, _ether_world_count_objs);
    return _ether_world_count;
}

int ether_world_count_facets(void) {
    _ether_world_count = 0;
    ether_object_traverse(ether_world_current->objects, _ether_world_count_facets);
    return _ether_world_count;
}

int ether_world_count_lights(void) {
    EtherLight *light;
    int n = 0;

    for (light = ether_world_current->lights; light; light = light->next)
        ++n;
    return n;
}

int ether_world_count_cameras(void) {
    EtherCamera *camera;
    int n = 0;
    for (camera = ether_world_current->cameras; camera; camera = camera->next)
        ++n;
    return n;
}

void ether_world_bounds_get(EtherVector v1, EtherVector v2) {
    EtherObject *obj = ether_world_object_tree_get();
    EtherVector v;

    ether_object_minbounds_get(obj, v);
    _ether_world_minx = _ether_world_maxx = v[X];
    _ether_world_miny = _ether_world_maxy = v[Y];
    _ether_world_minz = _ether_world_maxz = v[Z];

    ether_object_traverse(obj, _ether_world_find_bounds);
    v1[X] = _ether_world_minx;
	v1[Y] = _ether_world_miny;
	v1[Z] = _ether_world_minz;
    v2[X] = _ether_world_maxx;
	v2[Y] = _ether_world_maxy;
	v2[Z] = _ether_world_maxz;
}

void ether_world_center_get(EtherVector v) {
    EtherVector v1, v2;
    ether_world_bounds_get(v1, v2);
    v[X] = (v1[X] + v2[X]) / 2;
    v[Y] = (v1[Y] + v2[Y]) / 2;
    v[Z] = (v1[Z] + v2[Z]) / 2;
}

EtherScalar ether_world_size_get(void) {
    EtherVector v1, v2, vc;
    ether_world_bounds_get(v1, v2);
    ether_world_center_get(vc);

    return max(ether_vector_distance(v1, vc), ether_vector_distance(v2, vc));
}

void ether_world_screenclear_set(int n) {
    ether_world_current->screenclear = (n) ? 1 : 0;
}

int ether_world_screenclear_get(void) {
    return ether_world_current->screenclear;
}

void ether_world_toggle_screenclear(void) {
    ether_world_current->screenclear = !(ether_world_current->screenclear);
}

void ether_world_horizon_set(int n) {
    ether_world_current->horizon = (n) ? 1 : 0;
}

int ether_world_horizon_get(void) {
    return ether_world_current->horizon;
}

void ether_world_toggle_horizon(void) {
    ether_world_current->horizon = !(ether_world_current->horizon);
}

void ether_world_movement_mode_set(int n) {
    ether_world_current->movement_mode = (n) ? 1 : 0;
}

int ether_world_movement_mode_get(void) {
    return ether_world_current->movement_mode;
}

void ether_world_toggle_movement_mode(void) {
    ether_world_current->movement_mode = !(ether_world_current->movement_mode);
}

void ether_world_movestep_set(EtherScalar distance) {
    ether_world_current->movestep = distance;
}

EtherScalar ether_world_movestep_get(void) {
    return ether_world_current->movestep;
}

void ether_world_turnstep_set(EtherAngle angle) {
    ether_world_current->rotstep = angle;
}

EtherAngle ether_world_turnstep_get(void) {
    return ether_world_current->rotstep;
}

void ether_world_scale_set(EtherScalar s) {
    ether_world_current->scale = s;
}

EtherScalar ether_world_scale_get(void) {
    return ether_world_current->scale;
}

void ether_world_ambient_set(EtherFactor amb) {
    ether_world_current->ambient = amb;
}

EtherFactor ether_world_ambient_get(void) {
    return ether_world_current->ambient;
}

void ether_world_ground_color_set(EtherColor color) {
    ether_world_current->horizoncolors[0] = color;
}

EtherColor ether_world_ground_color_get(void) {
	return ether_world_current->horizoncolors[0];
}

void ether_world_sky_color_set(EtherColor color) {
    ether_world_current->horizoncolors[ether_world_current->nhorizoncolors-1] = color;
}

EtherColor ether_world_sky_color_get(void) {
    return ether_world_current->horizoncolors[ether_world_current->nhorizoncolors-1];
}

EtherPalette * ether_world_palette_get(void) {
    return &ether_world_current->palette;
}

EtherLight * ether_world_lights_get(void) {
    return ether_world_current->lights;
}

EtherCamera * ether_world_cameras_get(void) {
    return ether_world_current->cameras;
}

EtherObject * ether_world_object_tree_get(void) {
    return ether_world_current->objects;
}

void ether_world_camera_set(EtherCamera *cam) {
    ether_world_current->camera = cam;
}

EtherCamera * ether_world_camera_get(void) {
    return ether_world_current->camera;
}

void ether_world_left_camera_set(EtherCamera *cam) {
    ether_world_current->left_camera = cam;
}

EtherCamera * ether_world_left_camera_get(void) {
    return ether_world_current->left_camera;
}

void ether_world_right_camera_set(EtherCamera *cam) {
    ether_world_current->right_camera = cam;
}

EtherCamera * ether_world_right_camera_get(void) {
    return ether_world_current->right_camera;
}

EtherObject * ether_world_update(void) {
    return ether_object_update(ether_world_object_tree_get());
}

void ether_world_current_set(EtherWorld *world) {
    ether_world_current = world;
}

EtherWorld * ether_world_current_get(void) {
    return ether_world_current;
}

static int _ether_world_count_objs(EtherObject *obj) {
    if (obj)
        ++_ether_world_count;
    return 0;
}
    
static int _ether_world_count_facets(EtherObject *obj) {
    EtherRep *rep;
    EtherShape *shape;

    if(obj == NULL)
        return 0; /* no object */

    shape = ether_object_shape_get(obj);
    if(shape == NULL)
        return 0; /* object has no shape */

    rep = ether_object_rep_get(obj);
    /* object has a "current" rep -- use that */
    if(rep) {
        _ether_world_count += ether_rep_count_facets(rep);
        return 0;
    }
    
    rep = ether_shape_first_rep_get(shape); /* otherwise use the first (most detailed) rep */
    if (rep)
        _ether_world_count += ether_rep_count_facets(rep);
    
    return 0;
}

static int _ether_world_find_obj(EtherObject *obj) {
    if(obj == NULL)
        return 0;
    if(!strcasecmp(obj->name, _ether_world_find_name)) {
        _ether_world_found_obj = obj;
        return 1;
    }

    return 0;
}

static int _ether_world_find_bounds(EtherObject *obj) {
    EtherVector v;
    ether_object_minbounds_get(obj, v);

    if(v[X] < _ether_world_minx)
        _ether_world_minx = v[X];
    else if(v[X] > _ether_world_maxx)
        _ether_world_maxx = v[X];
    if(v[Y] < _ether_world_miny)
        _ether_world_miny = v[Y];
    else if(v[Y] > _ether_world_maxy)
        _ether_world_maxy = v[Y];
    if(v[Z] < _ether_world_minz)
        _ether_world_minz = v[Z];
    else if(v[Z] > _ether_world_maxz)
        _ether_world_maxz = v[Z];
    return 0;
}
