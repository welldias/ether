#ifndef __ETHER_WORLD_H__
#define __ETHER_WORLD_H__

/* In Ether, a virtual world is a collection of objects, light sources, virtual 
 * cameras and miscellaneous attributes.  You can have any number of worlds within 
 * a single Ether application; they're distinct from each other, and you can switch 
 * between them whenever you like.
 */

extern EtherWorld *ether_world_current;   /**< the currently active world */

struct _Ether_World
{
    EtherObject *objects;           /**< tree of objects */
    EtherLight  *lights;            /**< linked list of lights */
    EtherCameraOld *cameras;           /**< linked list of cameras */
    EtherCameraOld *camera;            /**< current camera */
    EtherCameraOld *left_camera;       /**< left-eye camera */
    EtherCameraOld *right_camera;      /**< right-eye camera */
    EtherFactor  ambient;           /**< ambient light level */
    EtherScalar  scale;             /**< millimeters per unit of virtual space */
    EtherScalar  movestep;          /**< default movement step size */
    EtherAngle   rotstep;           /**< default rotation step size */
    int movement_mode : 1;          /**< non-zero if we can fly by looking up */
    int screenclear : 1;            /**< set if we should clear the screen */
    int horizon : 1;                /**< set if we should draw a horizon   */
    EtherColor horizoncolors[10];   /**< entry 0 is ground, entry n is sky */
    int nhorizoncolors;             /**< number of colors used in horizoncolors[] */
    EtherPalette palette;           /**< the palette, if one is used */
};

EAPI EtherWorld     *ether_world_create               (void);
EAPI EtherWorld     *ether_world_init                 (EtherWorld *world);
EAPI void            ether_world_add_light            (EtherLight *light);
EAPI void            ether_world_remove_light         (EtherLight *light);
EAPI EtherLight     *ether_world_find_light           (char *name);
EAPI void            ether_world_add_camera           (EtherCameraOld *camera);
EAPI void            ether_world_remove_camera        (EtherCameraOld *camera);
EAPI EtherCameraOld    *ether_world_find_camera          (char *name);
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
EAPI EtherCameraOld    *ether_world_cameras_get          (void);
EAPI EtherObject    *ether_world_object_tree_get      (void);
EAPI void            ether_world_camera_set           (EtherCameraOld *cam);
EAPI EtherCameraOld    *ether_world_camera_get           (void);
EAPI void            ether_world_left_camera_set      (EtherCameraOld *cam);
EAPI EtherCameraOld    *ether_world_left_camera_get      (void);
EAPI void            ether_world_right_camera_set     (EtherCameraOld *cam);
EAPI EtherCameraOld    *ether_world_right_camera_get     (void);
EAPI EtherObject    *ether_world_update               (void);
EAPI void            ether_world_current_set          (EtherWorld *world);
EAPI EtherWorld     *ether_world_current_get          (void);

#endif /* __ETHER_WORLD_H__ */
