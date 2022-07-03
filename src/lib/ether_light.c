#include "ether_light.h"
#include "ether_object.h"
#include "ether_world.h"
#include "ether_camera.h"

EtherLight *
ether_light_create(void)
{
    EtherLight *light = malloc(sizeof(EtherLight));
    EtherObject *obj  = ether_object_create(NULL);
    if(light == NULL || obj == NULL)
		return NULL;
    ether_light_init(light);
    ether_light_associate(light, obj);
    return light;
}

EtherLight *
ether_light_init(EtherLight *light)
{
    if (light == NULL)
		return NULL;
    light->type = ETHER_LIGHT_DIRECTIONAL;
    light->on = 1;
    light->intensity = ETHER_FLOAT_TO_ANGLE(0.5);
    light->object = NULL;
    light->name = "No Name";
    light->applic_data = NULL;
    ether_world_add_light(light);
    return light;
}

void
ether_light_destroy(EtherLight *light)
{
    ether_world_remove_light(light);
    free(light);
}

void
ether_light_type_set(EtherLight *light, EtherLightingType type)
{
    if(light)
        light->type = type;
}

EtherLightingType
ether_light_type_get(EtherLight *light)
{
    if(light)
        return light->type;
    else
        return 0;
}

void
ether_light_on(EtherLight *light)
{
    if(light)
        light->on = 1;
}

void
ether_light_off(EtherLight *light)
{
    if(light)
        light->on = 0;
}

void
ether_light_toggle(EtherLight *light)
{
    if(light)
        light->on = !(light->on);
}

int
ether_light_is_on(EtherLight *light)
{
    if(light)
        return light->on;
    else
        return 0;
}

void
ether_light_intensity_set(EtherLight *light, EtherFactor intensity)
{
    if(light)
        light->intensity = intensity;
}

EtherFactor
ether_light_intensity_get(EtherLight *light)
{
    if(light)
        return light->intensity;
    else
        return 0;
}

void 
ether_light_associate(EtherLight *light, EtherObject *obj)
{
    if(light)
        light->object = obj;
}

void
ether_light_disassociate(EtherLight *light)
{
    if(light)
        light->object = NULL;
}

EtherObject *
ether_light_object_get(EtherLight *light)
{
    if(light)
        return light->object;
    else
        return NULL;
}

void
ether_light_name_set(EtherLight *light, char *name)
{
    if(light)
        light->name = name;
}

char *
ether_light_name_get(EtherLight *light)
{
    if(light)
        return light->name;
    else
        return NULL;
}

EtherLight *
ether_light_next_get(EtherLight *light)
{
    if(light)
        return light->next;
    else
        return NULL;
}

void
ether_light_application_data_set(EtherLight *light, void *data)
{
    if(light)
        light->applic_data = data;
}

void *
ether_light_application_data_get(EtherLight *light)
{
    if(light)
        return light->applic_data;
    else
        return NULL;
}

void
ether_light_move(EtherLight *light, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(light)
		ether_object_move(light->object, x, y, z);
}
void
ether_light_rel_move(EtherLight *light, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(light)
		ether_object_rel_move(light->object, x, y, z);
}
void
ether_light_vector_move(EtherLight *light, EtherVector v)
{
	if(light)
		ether_object_vector_move(light->object, v);
}

void
ether_light_vector_rel_move(EtherLight *light, EtherVector v)
{
	if(light)
		ether_object_vector_rel_move(light->object, v);
}

void
ether_light_rot_x(EtherLight *light, EtherAngle angle)
{
	if(light)
		ether_object_rot_x(light->object, angle);
}

void
ether_light_rot_y(EtherLight *light, EtherAngle angle)
{
	if(light)
		ether_object_rot_y(light->object, angle);
}

void
ether_light_rot_z(EtherLight *light, EtherAngle angle)
{
	if(light)
		ether_object_rot_z(light->object, angle);
}

void
ether_light_rot_vector(EtherLight *light, EtherAngle angle, EtherVector v)
{
	if(light)
		ether_object_rot_vector(light->object, angle, v);
}

void
ether_light_rot_reset(EtherLight *light)
{
	if(light)
		ether_object_rot_reset(light->object);
}

void
ether_light_rotate(EtherLight *light, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(light)
		ether_object_rotate(light->object, angle, axis, frame, relative_to);
}

void
ether_light_translate(EtherLight *light, EtherVector v, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(light)
		ether_object_translate(light->object, v, frame, relative_to);
}

void
ether_light_look_at(EtherLight *light, EtherVector forward, EtherVector up)
{
	if(light)
		ether_object_look_at(light->object, forward, up);
}

EtherObject *
ether_light_attach(EtherLight *light, EtherObject *newparent)
{
	if(light)
		return ether_object_attach(light->object, newparent);
	else 
		return NULL;
}

EtherObject *
ether_light_detach(EtherLight *light)
{
	if(light)
		return ether_object_detach(light->object);
	else 
		return NULL;
}

EtherVector *
ether_light_world_location_get(EtherLight *light, EtherVector v)
{
	if(light)
		return ether_object_world_location_get(light->object, (v));
	else
		return NULL;
}

void
ether_light_world_rotations_get(EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(light)
		ether_object_world_rotations_get(light->object, rx, ry, rz);
}

EtherVector *
ether_light_relative_location_get(EtherLight *light, EtherVector v)
{
	if(light)
		return ether_object_relative_location_get(light->object, v);
	else
		return NULL;
}

void
ether_light_relative_rotations_get(EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(light)
		ether_object_relative_rotations_get(light->object, rx, ry, rz);
}

EtherScalar
ether_light_world_x_get(EtherLight *light)
{
	if(light)
		return light->object->globalmat[3][X];
	else
		return 0;
}

EtherScalar
ether_light_world_y_get(EtherLight *light)
{
	if(light)
		return light->object->globalmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_light_world_z_get(EtherLight *light)
{
	if(light)
		return light->object->globalmat[3][Z];
	else
		return 0;
}

EtherScalar
ether_light_relative_x_get(EtherLight *light)
{
	if(light)
		return light->object->localmat[3][X];
	else
		return 0;
}

EtherScalar
ether_light_relative_y_get(EtherLight *light)
{
	if(light)
		return light->object->localmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_light_relative_z_get(EtherLight *light)
{
	if(light)
		return light->object->localmat[3][Z];
	else
		return 0;
}

