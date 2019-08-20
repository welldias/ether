#ifndef __ETHER_LIGHT_H__
#define __ETHER_LIGHT_H__

/* Lights in Ether have a number of properties; they can be on or 
 * off, they can have an intensity, they can have a "type", and they 
 * can be associated with an object.  The on/off and intensity properties 
 * are similar to a household dimmer; rotating the knob on a dimmer alters 
 * the intensity, and pushing it in toggles the light on and off.
 * The current version of Ether only supports ambient lights and directional 
 * lights; point sources will be supported soon. Any light that is not 
 * associated with an object is considered ambient; this is in addition to 
 * the overall ambient light level for the world.  A directional light uses 
 * the orientation of the object it's associated with to determine which 
 * direction the light should come from. A point source light (once implemented) 
 * will use the location of the object it's associated with to determine where 
 * the light comes from.
 * As with worlds and objects, lights can be statically or dynamically created 
 * and destroyed.
 * 
 */
 
typedef enum _Ether_Lighting_Type
{
   ETHER_LIGHT_AMBIENT = 0, 
   ETHER_LIGHT_DIRECTIONAL, 
   ETHER_LIGHT_POINTSOURCE
} Ether_Lighting_Type;

struct _Ether_Light
{
   Ether_Lighting_Type type;                /**< type of light source */
   EtherFactor intensity;                  /**< how bright the light is */
   int on : 1;                              /**< set if the light is on */
   char *name;                              /**< name of this light */
   void *applic_data;                       /**< pointer to application-specific data */
   int (*function)(EtherLight *light);     /**< light function */
   EtherObject *object;                    /**< the object this light is associated with, if any */
   EtherLight *next;                       /**< lights are kept in a linked list */
};

EAPI EtherLight          *ether_light_create                 (void);
EAPI EtherLight          *ether_light_init                   (EtherLight *light);
EAPI void                  ether_light_destroy                (EtherLight *light);
EAPI void                  ether_light_type_set               (EtherLight *light, Ether_Lighting_Type type);
EAPI Ether_Lighting_Type   ether_light_type_get               (EtherLight *light);
EAPI void                  ether_light_on                     (EtherLight *light);
EAPI void                  ether_light_off                    (EtherLight *light);
EAPI void                  ether_light_toggle                 (EtherLight *light);
EAPI int                   ether_light_is_on                  (EtherLight *light);
EAPI void                  ether_light_intensity_set          (EtherLight *light, EtherFactor intensity);
EAPI EtherFactor          ether_light_intensity_get          (EtherLight *light);
EAPI void                  ether_light_associate              (EtherLight *light, EtherObject *obj);
EAPI void                  ether_light_disassociate           (EtherLight *light);
EAPI EtherObject         *ether_light_object_get             (EtherLight *light);
EAPI void                  ether_light_name_set               (EtherLight *light, char *name);
EAPI char                 *ether_light_name_get               (EtherLight *light);
EAPI EtherLight          *ether_light_next_get               (EtherLight *light);
EAPI void                  ether_light_application_data_set   (EtherLight *light, void *data);
EAPI void                 *ether_light_application_data_get   (EtherLight *light);
EAPI void                  ether_light_move                   (EtherLight *light, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                  ether_light_rel_move               (EtherLight *light, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                  ether_light_vector_move            (EtherLight *light, EtherVector v);
EAPI void                  ether_light_vector_rel_move        (EtherLight *light, EtherVector v);
EAPI void                  ether_light_rot_x                  (EtherLight *light, EtherAngle angle);
EAPI void                  ether_light_rot_y                  (EtherLight *light, EtherAngle angle);
EAPI void                  ether_light_rot_z                  (EtherLight *light, EtherAngle angle);
EAPI void                  ether_light_rot_vector             (EtherLight *light, EtherAngle angle, EtherVector v);
EAPI void                  ether_light_rot_reset              (EtherLight *light);
EAPI void                  ether_light_rotate                 (EtherLight *light, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_light_translate              (EtherLight *light, EtherVector v, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                  ether_light_look_at                (EtherLight *light, EtherVector forward, EtherVector up);
EAPI EtherObject         *ether_light_attach                 (EtherLight *light, EtherObject *newparent);
EAPI EtherObject         *ether_light_detach                 (EtherLight *light);
EAPI EtherVector         *ether_light_world_location_get     (EtherLight *light, EtherVector v);
EAPI void                  ether_light_world_rotations_get    (EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI EtherVector         *ether_light_relative_location_get  (EtherLight *light, EtherVector v);
EAPI void                  ether_light_relative_rotations_get (EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI EtherScalar          ether_light_world_x_get            (EtherLight *light);
EAPI EtherScalar          ether_light_world_y_get            (EtherLight *light);
EAPI EtherScalar          ether_light_world_z_get            (EtherLight *light);
EAPI EtherScalar          ether_light_relative_x_get         (EtherLight *light);
EAPI EtherScalar          ether_light_relative_y_get         (EtherLight *light);
EAPI EtherScalar          ether_light_relative_z_get         (EtherLight *light);


#endif /* __ETHER_LIGHT_H__ */
