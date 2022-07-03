#ifndef __ETHER_WORLD_H__
#define __ETHER_WORLD_H__

#include "ether_defines.h"

EAPI EtherWorld     *ether_world_create               (void);
EAPI EtherWorld     *ether_world_init                 (EtherWorld *world);
EAPI void            ether_world_add_light            (EtherLight *light);
EAPI void            ether_world_remove_light         (EtherLight *light);
EAPI EtherLight     *ether_world_find_light           (char *name);
EAPI void            ether_world_add_camera           (EtherCamera *camera);
EAPI void            ether_world_remove_camera        (EtherCamera *camera);
EAPI EtherCamera    *ether_world_find_camera          (char *name);
EAPI void            ether_world_add_abject           (EtherObject *obj);
EAPI void            ether_world_remove_abject        (EtherObject *obj);
EAPI EtherObject    *ether_world_find_object          (char *name);
EAPI int             ether_world_count_objects        (void);
EAPI int             ether_world_count_facets         (void);
EAPI int             ether_world_count_lights         (void);
EAPI int             ether_world_count_cameras        (void);
EAPI void            ether_world_bounds_get           (EtherVector v1, EtherVector v2);
EAPI void            ether_world_center_get           (EtherVector v);
EAPI EtherScalar     ether_world_size_get             (void);
EAPI void            ether_world_screenclear_set      (int n);
EAPI int             ether_world_screenclear_get      (void);
EAPI void            ether_world_toggle_screenclear   (void);
EAPI void            ether_world_horizon_set          (int n);
EAPI int             ether_world_horizon_get          (void);
EAPI void            ether_world_toggle_horizon       (void);
EAPI void            ether_world_movement_mode_set    (int n);
EAPI int             ether_world_movement_mode_get    (void);
EAPI void            ether_world_toggle_movement_mode (void);
EAPI void            ether_world_movestep_set         (EtherScalar distance);
EAPI EtherScalar     ether_world_movestep_get         (void);
EAPI void            ether_world_turnstep_set         (EtherAngle angle);
EAPI EtherAngle      ether_world_turnstep_get         (void);
EAPI void            ether_world_scale_set            (EtherScalar s);
EAPI EtherScalar     ether_world_scale_get            (void);
EAPI void            ether_world_ambient_set          (EtherFactor amb);
EAPI EtherFactor     ether_world_ambient_get          (void);
EAPI void            ether_world_ground_color_set     (EtherColor color);
EAPI EtherColor      ether_world_ground_color_get     (void);
EAPI void            ether_world_sky_color_set        (EtherColor color);
EAPI EtherColor      ether_world_sky_color_get        (void);
EAPI EtherPalette   *ether_world_palette_get          (void);
EAPI EtherLight     *ether_world_lights_get           (void);
EAPI EtherCamera    *ether_world_cameras_get          (void);
EAPI EtherObject    *ether_world_object_tree_get      (void);
EAPI void            ether_world_camera_set           (EtherCamera *cam);
EAPI EtherCamera    *ether_world_camera_get           (void);
EAPI void            ether_world_left_camera_set      (EtherCamera *cam);
EAPI EtherCamera    *ether_world_left_camera_get      (void);
EAPI void            ether_world_right_camera_set     (EtherCamera *cam);
EAPI EtherCamera    *ether_world_right_camera_get     (void);
EAPI EtherObject    *ether_world_update               (void);
EAPI void            ether_world_current_set          (EtherWorld *world);
EAPI EtherWorld     *ether_world_current_get          (void);

#endif /* __ETHER_WORLD_H__ */
