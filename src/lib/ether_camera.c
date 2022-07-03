#include "ether_camera.h"
#include "ether_object.h"
#include "ether_world.h"

EtherCamera *
ether_camera_create(void)
{
    EtherCamera *camera = malloc(sizeof(EtherCamera));
    EtherObject *obj = ether_object_create(NULL);
    if(camera == NULL || obj == NULL)
        return NULL;
    ether_camera_init(camera);
    ether_camera_obj_set(camera, obj);
    if(ether_world_camera_get() == NULL)
        ether_world_camera_set(camera);
    return camera;
}

EtherCamera *
ether_camera_init(EtherCamera *camera)
{
	if (camera == NULL) 
		return NULL;
	camera->hither = ETHER_FLOAT_TO_SCALAR(10);
	camera->yon = ETHER_FLOAT_TO_SCALAR(1000000000L);
	camera->zoom = 4;  
	camera->aspect = 1.33f;
	camera->ortho = 0;  
	camera->orthodist = 0;
	camera->object = NULL;
	camera->name = "No Name";
	camera->need_updating = 1;
	return camera;
}

void
ether_camera_destroy(EtherCamera *camera)
{
	if(camera == NULL) 
		return;
	ether_world_remove_camera(camera);
	free(camera);
}

void ether_camera_zoom_set(EtherCamera *camera, float zoom)
{
	if(camera)
	{
		camera->zoom = zoom;
		camera->need_updating = 1;
	}
}

float
ether_camera_zoom_get(EtherCamera *camera)
{
	if(camera)
		return camera->zoom;
	else
		return 0;
}

void
ether_camera_aspect_set (EtherCamera *camera, float aspect)
{
	if(camera)
	{
		camera->aspect = aspect;
		camera->need_updating = 1;
	}
}

float
ether_camera_aspect_get(EtherCamera *camera)
{
	if(camera)
		return camera->aspect;
	else
		return 0;
}

void
ether_camera_hiter_set(EtherCamera *camera, EtherScalar hiter)
{
	if(camera)
		camera->hither = hiter;
}

EtherScalar
ether_camera_hiter_get(EtherCamera *camera)
{
	if(camera)
		return camera->hither;
	else
		return 0;
}

void
ether_camera_yon_set(EtherCamera *camera, EtherScalar yon)
{
	if(camera)
		camera->yon = yon;
}

EtherScalar
ether_camera_yon_get(EtherCamera *camera)
{
	if(camera)
		return camera->yon;
	else
		return 0;
}

void
ether_camera_obj_set(EtherCamera *camera, EtherObject *obj)
{
	if(camera)
		camera->object = obj;
}

EtherObject *
ether_camera_obj_get(EtherCamera *camera)
{
	if(camera)
		return camera->object;
	else
		return NULL;
}


void
ether_camera_name_set(EtherCamera *camera, char *name)
{
	if(camera)
		camera->name = name;
}

char *
ether_camera_name_get(EtherCamera *camera)
{
	if(camera)
		return camera->name;
	else
		return NULL;
}

EtherCamera *
ether_camera_next_get(EtherCamera *camera)
{
	if(camera)
		return camera->next;
	else
		return NULL;
}

void
ether_camera_application_data_set(EtherCamera *camera, void *data)
{
	if(camera)
		camera->applic_data = data;
}

void *
ether_camera_application_data_get(EtherCamera *camera)
{
	if(camera)
		return camera->applic_data;
	else
		return NULL;
}

void
ether_camera_move(EtherCamera *camera, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(camera)
	   ether_object_move(camera->object, x, y, z);
}

void
ether_camera_rel_move(EtherCamera *camera, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(camera)
	   ether_object_rel_move(camera->object, x, y, z);
}

void
ether_camera_vector_move(EtherCamera *camera, EtherVector vector)
{
	if(camera)
	   ether_object_vector_move(camera->object, vector);
}

void 
ether_camera_vector_rel_move(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_vector_rel_move(camera->object, vector);
}

void
ether_camera_rot_x(EtherCamera *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_x(camera->object, angle);
}

void 
ether_camera_rot_y(EtherCamera *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_y(camera->object, angle);
}

void 
ether_camera_rot_z(EtherCamera *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_z(camera->object, angle);
}

void
ether_camera_rot_vector(EtherCamera *camera, EtherAngle angle, EtherVector vector)
{
	if(camera)
		ether_object_rot_vector(camera->object, angle, vector);
}

void
ether_camera_rot_reset(EtherCamera *camera)
{
	if(camera)
	   ether_object_rot_reset(camera->object);
}

void
ether_camera_rotate(EtherCamera *camera, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(camera)
		ether_object_rotate(camera->object, angle, axis, frame, relative_to);
}

void
ether_camera_translate(EtherCamera *camera, EtherVector vector, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(camera)
		ether_object_translate(camera->object, vector, frame, relative_to);
}

void 
ether_camera_look_at(EtherCamera *camera, EtherVector forward, EtherVector up)
{
	if(camera)
		ether_object_look_at(camera->object, forward, up);
}

void 
ether_camera_attach(EtherCamera *camera, EtherObject *newparent)
{
	if(camera)
		ether_object_attach(camera->object, newparent);
}

void 
ether_camera_detach(EtherCamera *camera)
{
	if(camera)
		ether_object_detach(camera->object);
}

EtherScalar
ether_camera_world_x_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][X];
	else
		return 0;
}

EtherScalar
ether_camera_world_y_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_camera_world_z_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][Z];
	else
		return 0;
}

EtherScalar
ether_camera_relative_x_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][X];
	else
		return 0;
}

EtherScalar
ether_camera_relative_y_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_camera_relative_z_get(EtherCamera *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][Z];
	else
		return 0;
}

void
ether_camera_world_location_get(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_world_location_get(camera->object, vector);
}

void
ether_camera_world_rotations_get(EtherCamera *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(camera)
		ether_object_world_rotations_get(camera->object, rx, ry, rz);
}

void 
ether_camera_relative_location_get(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_relative_location_get(camera->object, vector);
}

void 
ether_camera_relative_rotations_get(EtherCamera *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(camera)
		ether_object_relative_rotations_get(camera->object, rx, ry, rz);
}

void
ether_camera_forward_vector_get(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_forward_vector_get(camera->object, vector);
}

void
ether_camera_right_vector_get(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_right_vector_get(camera->object, vector);
}

void
ether_camera_up_vector_get(EtherCamera *camera, EtherVector vector)
{
	if(camera)
		ether_object_up_vector_get(camera->object, vector);
}
