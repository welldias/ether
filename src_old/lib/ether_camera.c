#include <Ether.h>

static const float cameraYaw         = -90.0f;
static const float cameraPitch       =   0.0f;
static const float cameraSpeed       =   2.5f;
static const float cameraSensitivity =   0.1f;
static const float cameraZoom        =  45.0f;

static void _ether_camera_gl_update_camera_vectors(EtherCamera* camera);

EtherCamera* ether_camera_gl_create(void)
{
	EtherCamera*camera = malloc(sizeof(EtherCamera));
    if(camera == NULL)
        return NULL;
    ether_camera_gl_init(camera);

	return camera;
}

void ether_camera_gl_init(EtherCamera* camera)
{
	if (camera == NULL) 
		return;

	glm_vec3_copy(GLM_VEC3_ZERO, camera->position);
	glm_vec3_copy(GLM_YUP, camera->worldUp);
	glm_vec3_copy(GLM_FORWARD, camera->front);

	camera->movementSpeed = cameraSpeed;
	camera->mouseSensitivity = cameraSensitivity;
	camera->zoom = cameraZoom;
	camera->yaw = cameraYaw;
	camera->pitch = cameraPitch;


	_ether_camera_gl_update_camera_vectors(camera);
}

void ether_camera_gl_configure(EtherCamera* camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	if (camera == NULL)
		return;

	camera->position[0] = posX; camera->position[1] = posY; camera->position[2] = posZ;
	camera->worldUp[0] = upX; camera->worldUp[1] = upY; camera->worldUp[2] = upZ;
	glm_vec3_copy(GLM_FORWARD, camera->front);

	camera->movementSpeed = cameraSpeed;
	camera->mouseSensitivity = cameraSensitivity;
	camera->zoom = cameraZoom;
	camera->yaw = yaw;
	camera->pitch = pitch;

	_ether_camera_gl_update_camera_vectors(camera);
}

void ether_camera_gl_destroy(EtherCamera* camera)
{
	if(camera == NULL) 
		return;
	free(camera);
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
void ether_camera_gl_view_matrix_get(EtherCamera* camera, mat4 dest)
{
	vec3 vecAux;

	if (camera == NULL)
		return;

	glm_vec3_add(camera->position, camera->front, vecAux);
	glm_lookat(camera->position, vecAux, camera->up, dest);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ether_camera_gl_process_keyboard(EtherCamera* camera, EtherCameraMovment direction, float deltaTime)
{
	vec3 vecAux;

	if (camera == NULL)
		return;

	float velocity = camera->movementSpeed * deltaTime;

	if (direction == ETHER_CAMERA_MOVMENT_FORWARD) {
		glm_vec3_scale(camera->front, velocity, vecAux);
		glm_vec3_add(camera->position, vecAux, camera->position);
	}
	if (direction == ETHER_CAMERA_MOVMENT_BACKWARD) {
		glm_vec3_scale(camera->front, velocity, vecAux);
		glm_vec3_sub(camera->position, vecAux, camera->position);
	}
	if (direction == ETHER_CAMERA_MOVMENT_LEFT) {
		glm_vec3_scale(camera->right, velocity, vecAux);
		glm_vec3_sub(camera->position, vecAux, camera->position);
	}
	if (direction == ETHER_CAMERA_MOVMENT_RIGHT) {
		glm_vec3_scale(camera->right, velocity, vecAux);
		glm_vec3_add(camera->position, vecAux, camera->position);
	}
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ether_camera_gl_process_mouse_movement(EtherCamera* camera, float xoffset, float yoffset, int constrainPitch)
{
	if (camera == NULL)
		return;

	xoffset *= camera->mouseSensitivity;
	yoffset *= camera->mouseSensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch == TRUE)
	{
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	_ether_camera_gl_update_camera_vectors(camera);
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void ether_camera_gl_process_mouse_scroll(EtherCamera* camera, float yoffset)
{
	if (camera == NULL)
		return;

	camera->zoom -= (float)yoffset;
	if (camera->zoom < 1.0f)
		camera->zoom = 1.0f;
	if (camera->zoom > 45.0f)
		camera->zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void _ether_camera_gl_update_camera_vectors(EtherCamera* camera)
{
	vec3 vecAux;

	// calculate the new Front vector
	vecAux[0] = (float)(cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)));
	vecAux[1] = (float)sin(glm_rad(camera->pitch));
	vecAux[2] = (float)(sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)));
	glm_normalize_to(vecAux, camera->front);

	// also re-calculate the Right and Up vector
	glm_vec3_cross(camera->front, camera->worldUp, vecAux);
	glm_normalize_to(vecAux, camera->right);

	// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	glm_vec3_cross(camera->right, camera->front, vecAux);
	glm_normalize_to(vecAux, camera->up);
}

#if FALSE
void ether_camera_zoom_set(EtherCameraOld *camera, float zoom)
{
	if(camera)
	{
		camera->zoom = zoom;
		camera->need_updating = 1;
	}
}

float
ether_camera_zoom_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->zoom;
	else
		return 0;
}

void
ether_camera_aspect_set (EtherCameraOld *camera, float aspect)
{
	if(camera)
	{
		camera->aspect = aspect;
		camera->need_updating = 1;
	}
}

float
ether_camera_aspect_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->aspect;
	else
		return 0;
}

void
ether_camera_hiter_set(EtherCameraOld *camera, EtherScalar hiter)
{
	if(camera)
		camera->hither = hiter;
}

EtherScalar
ether_camera_hiter_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->hither;
	else
		return 0;
}

void
ether_camera_yon_set(EtherCameraOld *camera, EtherScalar yon)
{
	if(camera)
		camera->yon = yon;
}

EtherScalar
ether_camera_yon_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->yon;
	else
		return 0;
}

void
ether_camera_obj_set(EtherCameraOld *camera, EtherObject *obj)
{
	if(camera)
		camera->object = obj;
}

EtherObject *
ether_camera_obj_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->object;
	else
		return NULL;
}


void
ether_camera_name_set(EtherCameraOld *camera, char *name)
{
	if(camera)
		camera->name = name;
}

char *
ether_camera_name_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->name;
	else
		return NULL;
}

EtherCameraOld *
ether_camera_next_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->next;
	else
		return NULL;
}

void
ether_camera_application_data_set(EtherCameraOld *camera, void *data)
{
	if(camera)
		camera->applic_data = data;
}

void *
ether_camera_application_data_get(EtherCameraOld *camera)
{
	if(camera)
		return camera->applic_data;
	else
		return NULL;
}

void
ether_camera_move(EtherCameraOld *camera, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(camera)
	   ether_object_move(camera->object, x, y, z);
}

void
ether_camera_rel_move(EtherCameraOld *camera, EtherScalar x, EtherScalar y, EtherScalar z)
{
	if(camera)
	   ether_object_rel_move(camera->object, x, y, z);
}

void
ether_camera_vector_move(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
	   ether_object_vector_move(camera->object, vector);
}

void 
ether_camera_vector_rel_move(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_vector_rel_move(camera->object, vector);
}

void
ether_camera_rot_x(EtherCameraOld *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_x(camera->object, angle);
}

void 
ether_camera_rot_y(EtherCameraOld *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_y(camera->object, angle);
}

void 
ether_camera_rot_z(EtherCameraOld *camera, EtherAngle angle)
{
	if(camera)
		ether_object_rot_z(camera->object, angle);
}

void
ether_camera_rot_vector(EtherCameraOld *camera, EtherAngle angle, EtherVector vector)
{
	if(camera)
		ether_object_rot_vector(camera->object, angle, vector);
}

void
ether_camera_rot_reset(EtherCameraOld *camera)
{
	if(camera)
	   ether_object_rot_reset(camera->object);
}

void
ether_camera_rotate(EtherCameraOld *camera, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(camera)
		ether_object_rotate(camera->object, angle, axis, frame, relative_to);
}

void
ether_camera_translate(EtherCameraOld *camera, EtherVector vector, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	if(camera)
		ether_object_translate(camera->object, vector, frame, relative_to);
}

void 
ether_camera_look_at(EtherCameraOld *camera, EtherVector forward, EtherVector up)
{
	if(camera)
		ether_object_look_at(camera->object, forward, up);
}

void 
ether_camera_attach(EtherCameraOld *camera, EtherObject *newparent)
{
	if(camera)
		ether_object_attach(camera->object, newparent);
}

void 
ether_camera_detach(EtherCameraOld *camera)
{
	if(camera)
		ether_object_detach(camera->object);
}

EtherScalar
ether_camera_world_x_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][X];
	else
		return 0;
}

EtherScalar
ether_camera_world_y_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_camera_world_z_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->globalmat[3][Z];
	else
		return 0;
}

EtherScalar
ether_camera_relative_x_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][X];
	else
		return 0;
}

EtherScalar
ether_camera_relative_y_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][Y];
	else
		return 0;
}

EtherScalar
ether_camera_relative_z_get(EtherCameraOld *camera)
{
	if(camera && camera->object)
		return camera->object->localmat[3][Z];
	else
		return 0;
}

void
ether_camera_world_location_get(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_world_location_get(camera->object, vector);
}

void
ether_camera_world_rotations_get(EtherCameraOld *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(camera)
		ether_object_world_rotations_get(camera->object, rx, ry, rz);
}

void 
ether_camera_relative_location_get(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_relative_location_get(camera->object, vector);
}

void 
ether_camera_relative_rotations_get(EtherCameraOld *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
	if(camera)
		ether_object_relative_rotations_get(camera->object, rx, ry, rz);
}

void
ether_camera_forward_vector_get(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_forward_vector_get(camera->object, vector);
}

void
ether_camera_right_vector_get(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_right_vector_get(camera->object, vector);
}

void
ether_camera_up_vector_get(EtherCameraOld *camera, EtherVector vector)
{
	if(camera)
		ether_object_up_vector_get(camera->object, vector);
}
#endif
