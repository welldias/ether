#ifndef __ETHER_CAMERA_H__
#define __ETHER_CAMERA_H__

typedef enum _Ether_Camera_Movment
{
	ETHER_CAMERA_MOVMENT_FORWARD,
	ETHER_CAMERA_MOVMENT_BACKWARD,
	ETHER_CAMERA_MOVMENT_LEFT,
	ETHER_CAMERA_MOVMENT_RIGHT
} EtherCameraMovment;

struct _Ether_Camera
{
    // camera Attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    
    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
};

EAPI EtherCamera*       ether_camera_gl_create                  (void);
EAPI void               ether_camera_gl_init                    (EtherCamera *camera);
EAPI void               ether_camera_gl_configure               (EtherCamera *camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
EAPI void               ether_camera_gl_destroy                 (EtherCamera *camera);
EAPI void               ether_camera_gl_view_matrix_get         (EtherCamera* camera, mat4 dest);
EAPI void               ether_camera_gl_process_keyboard        (EtherCamera* camera, EtherCameraMovment direction, float deltaTime);
EAPI void               ether_camera_gl_process_mouse_movement  (EtherCamera* camera, float xoffset, float yoffset, int constrainPitch);
EAPI void               ether_camera_gl_process_mouse_scroll    (EtherCamera* camera, float yoffset);

#if FALSE
EAPI void                  ether_camera_zoom_set                (EtherCameraOld *camera, float zoom);
EAPI float                 ether_camera_zoom_get                (EtherCameraOld *camera);
EAPI void                  ether_camera_aspect_set              (EtherCameraOld *camera, float aspect);
EAPI float                 ether_camera_aspect_get              (EtherCameraOld *camera);
EAPI void                  ether_camera_hiter_set               (EtherCameraOld *camera, EtherScalar hiter);
EAPI EtherScalar           ether_camera_hiter_get               (EtherCameraOld *camera);
EAPI void                  ether_camera_yon_set                 (EtherCameraOld *camera, EtherScalar yon);
EAPI EtherScalar           ether_camera_yon_get                 (EtherCameraOld *camera);
EAPI void                  ether_camera_obj_set                 (EtherCameraOld *camera, EtherObject *obj);
EAPI EtherObject          *ether_camera_obj_get                 (EtherCameraOld *camera);
EAPI void                  ether_camera_name_set                (EtherCameraOld *camera, char *name);
EAPI char                 *ether_camera_name_get                (EtherCameraOld *camera);
EAPI EtherCameraOld       *ether_camera_next_get                (EtherCameraOld *camera);
EAPI void                  ether_camera_application_data_set    (EtherCameraOld *camera, void *data);
EAPI void                 *ether_camera_application_data_get    (EtherCameraOld *camera);
EAPI void                  ether_camera_move                    (EtherCameraOld *camera, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                  ether_camera_rel_move                (EtherCameraOld *camera, EtherScalar x, EtherScalar y, EtherScalar z); 
EAPI void                  ether_camera_vector_move             (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_vector_rel_move         (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_rot_x                   (EtherCameraOld *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_y                   (EtherCameraOld *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_z                   (EtherCameraOld *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_vector              (EtherCameraOld *camera, EtherAngle angle, EtherVector vector);
EAPI void                  ether_camera_rot_reset               (EtherCameraOld *camera);
EAPI void                  ether_camera_rotate                  (EtherCameraOld *camera, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_camera_translate               (EtherCameraOld *camera, EtherVector vector, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_camera_look_at                 (EtherCameraOld *camera, EtherVector forward, EtherVector up);
EAPI void                  ether_camera_attach                  (EtherCameraOld *camera, EtherObject *newparent);
EAPI void                  ether_camera_detach                  (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_world_x_get             (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_world_y_get             (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_world_z_get             (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_relative_x_get          (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_relative_y_get          (EtherCameraOld *camera);
EAPI EtherScalar           ether_camera_relative_z_get          (EtherCameraOld *camera);
EAPI void                  ether_camera_world_location_get      (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_world_rotations_get     (EtherCameraOld *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                  ether_camera_relative_location_get   (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_relative_rotations_get  (EtherCameraOld *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                  ether_camera_forward_vector_get      (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_right_vector_get        (EtherCameraOld *camera, EtherVector vector);
EAPI void                  ether_camera_up_vector_get           (EtherCameraOld *camera, EtherVector vector);
#endif

#endif /* __ETHER_CAMERA_H__ */
