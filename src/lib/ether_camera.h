#ifndef __ETHER_CAMERA_H__
#define __ETHER_CAMERA_H__

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

struct _Ether_Camera
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
   EtherCamera *next;                      /**< cameras are kept in a linked list */
};

EAPI EtherCamera          *ether_camera_init                    (EtherCamera *camera);
EAPI EtherCamera          *ether_camera_create                  (void);
EAPI void                  ether_camera_destroy                 (EtherCamera *camera);
EAPI void                  ether_camera_zoom_set                (EtherCamera *camera, float zoom);
EAPI float                 ether_camera_zoom_get                (EtherCamera *camera);
EAPI void                  ether_camera_aspect_set              (EtherCamera *camera, float aspect);
EAPI float                 ether_camera_aspect_get              (EtherCamera *camera);
EAPI void                  ether_camera_hiter_set               (EtherCamera *camera, EtherScalar hiter);
EAPI EtherScalar           ether_camera_hiter_get               (EtherCamera *camera);
EAPI void                  ether_camera_yon_set                 (EtherCamera *camera, EtherScalar yon);
EAPI EtherScalar           ether_camera_yon_get                 (EtherCamera *camera);
EAPI void                  ether_camera_obj_set                 (EtherCamera *camera, EtherObject *obj);
EAPI EtherObject          *ether_camera_obj_get                 (EtherCamera *camera);
EAPI void                  ether_camera_name_set                (EtherCamera *camera, char *name);
EAPI char                 *ether_camera_name_get                (EtherCamera *camera);
EAPI EtherCamera          *ether_camera_next_get                (EtherCamera *camera);
EAPI void                  ether_camera_application_data_set    (EtherCamera *camera, void *data);
EAPI void                 *ether_camera_application_data_get    (EtherCamera *camera);
EAPI void                  ether_camera_move                    (EtherCamera *camera, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                  ether_camera_rel_move                (EtherCamera *camera, EtherScalar x, EtherScalar y, EtherScalar z); 
EAPI void                  ether_camera_vector_move             (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_vector_rel_move         (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_rot_x                   (EtherCamera *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_y                   (EtherCamera *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_z                   (EtherCamera *camera, EtherAngle angle);
EAPI void                  ether_camera_rot_vector              (EtherCamera *camera, EtherAngle angle, EtherVector vector);
EAPI void                  ether_camera_rot_reset               (EtherCamera *camera);
EAPI void                  ether_camera_rotate                  (EtherCamera *camera, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_camera_translate               (EtherCamera *camera, EtherVector vector, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_camera_look_at                 (EtherCamera *camera, EtherVector forward, EtherVector up);
EAPI void                  ether_camera_attach                  (EtherCamera *camera, EtherObject *newparent);
EAPI void                  ether_camera_detach                  (EtherCamera *camera);
EAPI EtherScalar           ether_camera_world_x_get             (EtherCamera *camera);
EAPI EtherScalar           ether_camera_world_y_get             (EtherCamera *camera);
EAPI EtherScalar           ether_camera_world_z_get             (EtherCamera *camera);
EAPI EtherScalar           ether_camera_relative_x_get          (EtherCamera *camera);
EAPI EtherScalar           ether_camera_relative_y_get          (EtherCamera *camera);
EAPI EtherScalar           ether_camera_relative_z_get          (EtherCamera *camera);
EAPI void                  ether_camera_world_location_get      (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_world_rotations_get     (EtherCamera *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                  ether_camera_relative_location_get   (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_relative_rotations_get  (EtherCamera *camera, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                  ether_camera_forward_vector_get      (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_right_vector_get        (EtherCamera *camera, EtherVector vector);
EAPI void                  ether_camera_up_vector_get           (EtherCamera *camera, EtherVector vector);

#endif /* __ETHER_CAMERA_H__ */
