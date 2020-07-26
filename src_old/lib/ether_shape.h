#ifndef __ETHER_SHAPE_H__
#define __ETHER_SHAPE_H__

struct _Ether_Shape
{
  EtherVector center;                  /**< center of bounding sphere */
  EtherScalar radius;                  /**< radius of bounding sphere */
  EtherVector minbound;                /**< bounding box */
  EtherVector maxbound;
  EtherSurfaceMap *default_surfacemap; /**< default surface map for this shape */
  EtherRep *replist;                   /**< linked list of representations */
  char *name;                          /**< name of this shape */
  EtherShape *next;                    /**< shapes are kept in a linked list */
};

EAPI EtherShape      *ether_shape_create                  (void);
EAPI EtherShape      *ether_shape_init                    (EtherShape *shape);
EAPI void             ether_shape_compute_bounds          (EtherShape *shape);
EAPI void             ether_shape_rescale                 (EtherShape *shape, float sx, float sy, float sz);
EAPI void             ether_shape_offset                  (EtherShape *shape, EtherScalar tx, EtherScalar ty, EtherScalar tz);
EAPI void             ether_shape_update                  (EtherShape *shape);
EAPI void             ether_shape_transform               (EtherMatrix matrix, EtherShape *shape);
EAPI EtherRep        *ether_shape_rep_get                 (EtherShape *shape, EtherScalar size);
EAPI void             ether_shape_add_rep                 (EtherShape *shape, EtherRep *rep, EtherScalar size);
EAPI void             ether_shape_traverse_reps           (EtherShape *shape, int (*function)(EtherRep *rep));
EAPI int              ether_shape_count_reps              (EtherShape *shape);
EAPI EtherShape      *ether_shape_list_get                (void);
EAPI EtherShape      *ether_shape_find                    (char *name);
EAPI void             ether_shape_compute_vertex_normals  (EtherShape *shape);
EAPI EtherScalar      ether_shape_radius_get              (EtherShape *shape);
EAPI EtherVector     *ether_shape_center_get              (EtherShape *shape, EtherVector vector);
EAPI EtherVector     *ether_shape_min_bounds_get          (EtherShape *shape, EtherVector vector);
EAPI EtherVector     *ether_shape_max_bounds_get          (EtherShape *shape, EtherVector vector);
EAPI void             ether_shape_surface_map_set         (EtherShape *shape, EtherSurfaceMap *map);
EAPI EtherSurfaceMap *ether_shape_surface_map_get         (EtherShape *shape);
EAPI void             ether_shape_name_set                (EtherShape *shape, char *name);
EAPI char            *ether_shape_name_get                (EtherShape *shape);
EAPI EtherShape      *ether_shape_next_get                (EtherShape *shape);
EAPI EtherRep        *ether_shape_first_rep_get           (EtherShape *shape);

#endif /* __ETHER_SHAPE_H__ */
