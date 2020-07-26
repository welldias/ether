#ifndef __ETHER_OBJECT_H__
#define __ETHER_OBJECT_H__

/* Objects are the most important entities in a virtual world. 
 * All objects have a location and orientation, and they can be 
 * attached to each other in a tree-structured hierarchy. 
 * Each object can have a shape (i.e. geometric description) and 
 * a surface map. You can create an object statically (by declaring 
 * a variable of type Ether_Object) or dynamically (either by using 
 * malloc() to allocate the space and ether_object_init() to initialize 
 * it, or by simply calling ether_object_create()). If you use 
 * ether_object_create(), you can optionally specify a shape for the 
 * object to use; if you don't want to assign a shape, use NULL. You 
 * can also destroy objects using ether_object_destroy().
 * 
 */

typedef enum _Ether_Coord_Frame
{
   ETHER_COORD_LOCAL = 0,
   ETHER_COORD_PARENT,
   ETHER_COORD_WORLD,
   ETHER_COORD_OBJREL
} EtherCoordFrame;

typedef int (*EtherObjectFunction)(EtherObject *obj);

struct _Ether_Object
{
  EtherShape *shape;             /**< geometry information */
  EtherSurfaceMap *surfmap;      /**< array of pointers to surface descriptors */
  EtherMatrix localmat;          /**< transformation matrix relative to our parent */
  EtherMatrix globalmat;         /**< transformation matrix relative to the world */
  EtherObject *parent;           /**< pointer to our parent in the hierarchy */
  EtherObject *children;         /**< pointer to our children */
  EtherObject *siblings;         /**< pointers to our siblings */
  EtherVector minbound, maxbound;/**< bounding box (world coords) */
  int fixed : 1;                 /**< set if object is immobile */
  int moved : 1;                 /**< set when our local matrix has changed */
  int rotate_box : 1;            /**< set if bounding box should rotate */
  int highlight : 1;             /**< set if object is highlighted */
  int invisible : 1;             /**< set if object is invisible */
  char *name;                    /**< name of the object (may be NULL) */
  void *applic_data;             /**< pointer to application-specific data */
  unsigned char layer;           /**< the layer we're on (0 for all, 1-255) */
  EtherObject *contents;         /**< points to objects contained by this one (not used) */
  EtherRep *forced_rep;          /**< if not NULL, forces a rep to be used */
  EtherObjectFunction function;/**< object function */
  EtherObject *next;             /**< points to next object on a list */
};

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
