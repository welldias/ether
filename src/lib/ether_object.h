#ifndef __ETHER_OBJECT_H__
#define __ETHER_OBJECT_H__

#include "ether_defines.h"

EAPI EtherObject           *ether_object_init                    (EtherObject *obj);
EAPI EtherObject           *ether_object_create                  (EtherShape *shape);
EAPI EtherObject           *ether_object_copy                    (EtherObject *proto);
EAPI void                   ether_object_destroy                 (EtherObject *obj);
EAPI void                   ether_object_move                    (EtherObject *obj, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                   ether_object_rel_move                (EtherObject *obj, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void                   ether_object_rot_vector              (EtherObject *obj, EtherAngle angle, EtherVector vector);
EAPI void                   ether_object_rot_reset               (EtherObject *obj);
EAPI void                   ether_object_rotate                  (EtherObject *obj, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                   ether_object_translate               (EtherObject *obj, EtherVector v, EtherCoordFrame frame, EtherObject *relative_to);
EAPI void                   ether_object_look_at                 (EtherObject *obj, EtherVector forward, EtherVector up);
EAPI EtherObject           *ether_object_attach                  (EtherObject *obj, EtherObject *newparent);
EAPI EtherObject           *ether_object_detach                  (EtherObject *obj);
EAPI EtherObject           *ether_object_update                  (EtherObject *obj);
EAPI void                   ether_object_traverse                (EtherObject *obj, int (*function)(EtherObject *obj));
EAPI void                   ether_object_make_fixed              (EtherObject *obj);
EAPI void                   ether_object_make_movable            (EtherObject *obj);
EAPI EtherObject           *ether_object_find_root               (EtherObject *obj);
EAPI EtherScalar            ether_object_compute_distance        (EtherObject *obj1, EtherObject *obj2);
EAPI void                   ether_object_rot_x                   (EtherObject *obj, EtherAngle angle);
EAPI void                   ether_object_rot_y                   (EtherObject *obj, EtherAngle angle);
EAPI void                   ether_object_rot_z                   (EtherObject *obj, EtherAngle angle);      
EAPI void                   ether_object_vector_move             (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_vector_rel_move         (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_layer_set               (EtherObject *obj, unsigned char layer);
EAPI unsigned char          ether_object_layer_get               (EtherObject *obj);
EAPI void                   ether_object_shape_set               (EtherObject *obj, EtherShape *shp);
EAPI EtherShape            *ether_object_shape_get               (EtherObject *obj);
EAPI void                   ether_object_surfacemap_set          (EtherObject *obj, EtherSurfaceMap *map);
EAPI EtherSurfaceMap       *ether_object_surfacemap_get          (EtherObject *obj);
EAPI void                   ether_object_highlight_set           (EtherObject *obj, int high);
EAPI int                    ether_object_highlight_get           (EtherObject *obj);
EAPI void                   ether_object_toggle_highlight        (EtherObject *obj);
EAPI void                   ether_object_visibility_set          (EtherObject *obj, int vis);
EAPI int                    ether_object_visibility_get          (EtherObject *obj);
EAPI void                   ether_object_toggle_visibility       (EtherObject *obj);
EAPI int                    ether_object_is_fixed                (EtherObject *obj);
EAPI EtherVector           *ether_object_minbounds_get           (EtherObject *obj, EtherVector v);
EAPI EtherVector           *ether_object_maxbounds_get           (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_rep_set                 (EtherObject *obj, EtherRep *r);
EAPI EtherRep              *ether_object_rep_get                 (EtherObject *obj);
EAPI EtherScalar            ether_object_world_x_get             (EtherObject *obj);
EAPI EtherScalar            ether_object_world_y_get             (EtherObject *obj);
EAPI EtherScalar            ether_object_world_z_get             (EtherObject *obj);
EAPI EtherVector           *ether_object_world_location_get      (EtherObject *obj, EtherVector v);
EAPI EtherScalar            ether_object_relative_x_get          (EtherObject *obj);
EAPI EtherScalar            ether_object_relative_y_get          (EtherObject *obj);
EAPI EtherScalar            ether_object_relative_z_get          (EtherObject *obj);
EAPI EtherVector           *ether_object_relative_location_get   (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_world_rotations_get     (EtherObject *obj, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                   ether_object_relative_rotations_get  (EtherObject *obj, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void                   ether_object_name_set                (EtherObject *obj, char *str);
EAPI char                  *ether_object_name_get                (EtherObject *obj);
EAPI void                   ether_object_application_data_set    (EtherObject *obj, void *data);
EAPI void                  *ether_object_application_data_get    (EtherObject *obj);
EAPI void                   ether_object_forward_vector_get      (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_right_vector_get        (EtherObject *obj, EtherVector v);
EAPI void                   ether_object_up_vector_get           (EtherObject *obj, EtherVector v);
EAPI EtherObject           *ether_object_parent_get              (EtherObject *obj);
EAPI void                   ether_object_function_set            (EtherObject *obj, EtherObjectFunction function);
EAPI EtherObjectFunction    ether_object_function_get            (EtherObject *obj);
EAPI EtherObject           *ether_object_load_from_plg           (FILE *in);

#endif /* __ETHER_OBJECT_H__ */
