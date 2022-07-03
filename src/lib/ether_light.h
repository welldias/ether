#ifndef __ETHER_LIGHT_H__
#define __ETHER_LIGHT_H__

#include "ether_defines.h"

EAPI EtherLight           *ether_light_create                 (void);
EAPI EtherLight           *ether_light_init                   (EtherLight *light);
EAPI void                  ether_light_destroy                (EtherLight *light);
EAPI void                  ether_light_type_set               (EtherLight *light, EtherLightingType type);
EAPI EtherLightingType     ether_light_type_get               (EtherLight *light);
EAPI void                  ether_light_on                     (EtherLight *light);
EAPI void                  ether_light_off                    (EtherLight *light);
EAPI void                  ether_light_toggle                 (EtherLight *light);
EAPI int                   ether_light_is_on                  (EtherLight *light);
EAPI void                  ether_light_intensity_set          (EtherLight *light, EtherFactor intensity);
EAPI EtherFactor           ether_light_intensity_get          (EtherLight *light);
EAPI void                  ether_light_associate              (EtherLight *light, EtherObject *obj);
EAPI void                  ether_light_disassociate           (EtherLight *light);
EAPI EtherObject          *ether_light_object_get             (EtherLight *light);
EAPI void                  ether_light_name_set               (EtherLight *light, char *name);
EAPI char                 *ether_light_name_get               (EtherLight *light);
EAPI EtherLight           *ether_light_next_get               (EtherLight *light);
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
EAPI EtherObject          *ether_light_attach                 (EtherLight *light, EtherObject *newparent);
EAPI EtherObject          *ether_light_detach                 (EtherLight *light);
EAPI EtherVector          *ether_light_world_location_get     (EtherLight *light, EtherVector v);
EAPI void                  ether_light_world_rotations_get    (EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI EtherVector          *ether_light_relative_location_get  (EtherLight *light, EtherVector v);
EAPI void                  ether_light_relative_rotations_get (EtherLight *light, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI EtherScalar           ether_light_world_x_get            (EtherLight *light);
EAPI EtherScalar           ether_light_world_y_get            (EtherLight *light);
EAPI EtherScalar           ether_light_world_z_get            (EtherLight *light);
EAPI EtherScalar           ether_light_relative_x_get         (EtherLight *light);
EAPI EtherScalar           ether_light_relative_y_get         (EtherLight *light);
EAPI EtherScalar           ether_light_relative_z_get         (EtherLight *light);


#endif /* __ETHER_LIGHT_H__ */
