#ifndef __ETHER_CAMERA_OLD_H__
#define __ETHER_CAMERA_OLD_H__

/*
 * Ether allows you to have any number of virtual cameras. Each camera 
 * is associated with an object, much as lights are. However, unlike 
 * lights, cameras must be associated with an object; there's no such 
 * thing as an "ambient" camera.  Cameras are initialized and destroyed 
 * just like objects or light sources.
 * 
 * Cameras have only a few properties that are important; in particular, 
 * a zoom factor, an aspect ratio, and hither and yon clipping plane distances.
 */

struct _Ether_Camera_Old
{
   EtherScalar hither;                     /**< distance to near clipping plane */
   EtherScalar yon;                        /**< distance to far culling plane */
   float zoom;                             /**< zoom factor (1/tan(FOV/2)) */
   float aspect;                           /**< aspect ratio */
   int ortho : 1;                          /**< set if we want orthographic projection (not used) */
   char *name;                             /**< name of this camera */
   void *applic_data;                      /**< pointer to application-specific data */
   unsigned char need_updating;            /**< set when zoom or aspect is changed */
   EtherScalar orthodist;                  /**< apparent "distance" for orthographic projection (not used) */
   EtherObject *object;                    /**< the object this camera is attached to */
   EtherFactor aright, cright, btop, ctop; /**< only used internally, for object culling */
   EtherCameraOld *next;                      /**< cameras are kept in a linked list */
};

EAPI EtherCameraOld       *ether_camera_init                    (EtherCameraOld *camera);
EAPI EtherCameraOld       *ether_camera_create                  (void);
EAPI void                  ether_camera_destroy                 (EtherCameraOld *camera);
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
EAPI EtherCameraOld          *ether_camera_next_get                (EtherCameraOld *camera);
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

#endif /* __ETHER_CAMERA_OLD_H__ */
