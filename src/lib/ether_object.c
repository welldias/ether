#include <Ether.h>
#include "ether_private.h"

static EtherObject *_ether_obj_list;

/*
   this routine is based on TransBox, by Jim Arvo
   (published in the first volume of "Graphics Gems")
 */
static void  _ether_object_transform_bbox     (EtherObject *object);
static void  _ether_object_recursive_updater  (EtherObject *object, int flag);
    
EtherObject *
ether_object_init(EtherObject *obj)
{
	if(obj == NULL) 
		return NULL;
	obj->shape = NULL;  
	obj->surfmap = NULL;
	ether_matrix_identity(obj->localmat);
	ether_matrix_identity(obj->globalmat);
	obj->parent = NULL;
	obj->siblings = NULL;
	obj->children = NULL;
	obj->layer = 0;  
	obj->invisible = 0;  
	obj->highlight = 0;
	obj->moved = 1;  
	obj->fixed = 0;  
	obj->rotate_box = 1;
	obj->applic_data = NULL;  
	obj->function = NULL;
	ether_vector_copy(obj->minbound, Ether_VectorNULL);
	ether_vector_copy(obj->maxbound, Ether_VectorNULL);
	obj->contents = NULL;
	obj->next = NULL;
	obj->forced_rep = NULL;
	obj->name = "No Name";
	ether_world_add_abject(obj);
	return obj;
}

EtherObject *
ether_object_create(EtherShape *shape)
{
	EtherObject *obj = malloc(sizeof(EtherObject));
	if(obj == NULL) 
		return NULL;
	ether_object_init(obj);
	ether_object_shape_set(obj, shape);
	return obj;
}

EtherObject *
ether_object_copy(EtherObject *proto)
{
	EtherObject *obj = malloc(sizeof(EtherObject));
	if(obj == NULL) 
		return NULL;
	memcpy(obj, proto, sizeof(EtherObject));
	obj->next = NULL;
	obj->contents = NULL;
	obj->children = NULL;
	proto->siblings = obj;
	return obj;
}

void
ether_object_destroy(EtherObject *obj)
{
	if(obj)
    {
    	ether_object_detach(obj->children);
    	ether_object_detach(obj);
    	free(obj);
    }
}

void
ether_object_move(EtherObject *obj, EtherScalar x, EtherScalar y, EtherScalar z)
{
	EtherVector v;
	if(obj == NULL)
		return;
	if(ether_object_is_fixed(obj))
		return;
	ether_vector_create(v, x, y, z);
	ether_matrix_translation_set(obj->localmat, v);
	obj->moved = 1;
}

void
ether_object_rel_move(EtherObject *obj, EtherScalar x, EtherScalar y, EtherScalar z)
{
	EtherVector v;
	if(obj == NULL) 
		return;
	if(ether_object_is_fixed(obj)) 
		return;
	ether_vector_create(v, x, y, z);
	ether_matrix_translate(obj->localmat, v, 1);
	obj->moved = 1;
}

void
ether_object_rot_vector(EtherObject *obj, EtherAngle angle, EtherVector vector)
{
	if(obj == NULL) 
		return;
	if(ether_object_is_fixed(obj))
		return;
	ether_matrix_rot_vector(obj->localmat, angle, vector, 1);
	obj->moved = 1;
}

void
ether_object_rot_reset(EtherObject *obj)
{
	if(obj == NULL)
		return;
	if(ether_object_is_fixed(obj))
		return;
	ether_matrix_reset_rotations(obj->localmat);
	obj->moved = 1;
}

void
ether_object_rotate(EtherObject *obj, EtherAngle angle, int axis, EtherCoordFrame frame, EtherObject *relative_to)
{
	int left = 0;  /* local coordinates */
	if(obj == NULL)
		return;
	if(ether_object_is_fixed(obj))
		return;
	if(axis > 2)
		axis -= 3; /* map XROT to X, etc */
	switch(frame)
	{
	case ETHER_COORD_OBJREL:
		{
			EtherVector basis;
			if (relative_to == NULL) return;
			ether_matrix_basis_get(basis, relative_to->globalmat, axis);
			if (obj->parent)
			{
				EtherMatrix m;
				EtherVector v;
				ether_matrix_inverse(m, obj->parent->globalmat);
				_ether_transform(v, m, basis);
				ether_object_rot_vector(obj, angle, v);
			}
			else
				ether_object_rot_vector(obj, angle, basis);
		}
		break;
	case ETHER_COORD_WORLD:
		if(obj->parent)
		{
			EtherVector *v = &obj->parent->globalmat[axis];
			ether_object_rot_vector(obj, angle, *v);
			break;
		}
		/* else fall through */
	case ETHER_COORD_PARENT:
		left = 1;  /* fall through */
	case ETHER_COORD_LOCAL:
		switch(axis)
		{
			case X:
			case XROT: 
				ether_matrix_rot_x(obj->localmat, angle, left); 
				break;
			case Y:
			case YROT: 
				ether_matrix_rot_y(obj->localmat, angle, left); 
				break;
			case Z:
			case ZROT: 
				ether_matrix_rot_z(obj->localmat, angle, left); 
				break;
			default: break;
		}
	default:
		break;
	}
	obj->moved = 1;
}

void
ether_object_translate(EtherObject *obj, EtherVector v, EtherCoordFrame frame, EtherObject *relative_to)
{
    EtherVector our_v;
    if(obj == NULL) 
        return;
    if(ether_object_is_fixed(obj)) 
        return;
    ether_vector_copy(our_v, v);
    switch(frame)
    {
    case ETHER_COORD_OBJREL:
        {
            EtherMatrix m;
            if(relative_to == NULL)
                return;
            ether_matrix_copy(m, relative_to->globalmat);
            ether_vector_zero(our_v);
            ether_matrix_translation_set(m, our_v);
            _ether_transform(our_v, m, v);
        }
    /* fall through */
    case ETHER_COORD_WORLD:
        if (obj->parent)
        {
            EtherMatrix m;
            EtherVector vtmp;
            ether_matrix_inverse(m, obj->parent->globalmat);
            ether_vector_zero(vtmp);
            ether_matrix_translation_set(m, vtmp);
            _ether_transform(vtmp, m, our_v);
            ether_matrix_translate(obj->localmat, vtmp, 1);
            break;
        }
    /* else fall through */
    case ETHER_COORD_PARENT:
        ether_matrix_translate(obj->localmat, our_v, 1); 
        break;
    case ETHER_COORD_LOCAL: 
        ether_matrix_translate(obj->localmat, our_v, 0); 
        break;
    default:
        break;
    }
    obj->moved = 1;
}

void
ether_object_look_at(EtherObject *obj, EtherVector forward, EtherVector up)
{
    EtherVector x, y;
    EtherMatrix mat;
    ether_vector_crossproduct(x, up, forward);
    ether_vector_crossproduct(y, forward, x);
    ether_matrix_basis_set(mat, x, X);  
    ether_matrix_basis_set(mat, y, Y);  
    ether_matrix_basis_set(mat, forward, Z);    
    ether_vector_copy(mat[3], obj->globalmat[3]);
    if(obj->parent)
    {
        /*
           Since our parent matrix times our local matrix gives our global
           matrix, we premultiply by the inverse of the parent matrix to
           get the local matrix that will produce our newly-computed global
           matrix.  Whew!
        */
        EtherMatrix tmp;
        ether_matrix_inverse(tmp, obj->parent->globalmat);
        ether_matrix_multiply(obj->localmat, tmp, mat);
    }
    else
        ether_matrix_copy(obj->localmat, mat);
    obj->moved = 1;
}

EtherObject *
ether_object_attach(EtherObject *obj, EtherObject *newparent)
{
    EtherMatrix tmp;
    EtherObject *oldparent;
    if(obj == NULL)
        return NULL;
    oldparent = obj->parent;
    if(ether_object_is_fixed(obj))
        return oldparent;   /* fixed objects can't be attached */
    if(newparent == NULL)
        return oldparent;        /* no parent to attach to */
    if(ether_object_is_fixed(newparent))
        return oldparent;   /* can't attach to a fixed object */
    if(oldparent)
        ether_object_detach(obj);
    ether_world_remove_abject(obj);
    obj->siblings = newparent->children;
    newparent->children = obj;
    obj->parent = newparent;
    /*
       Since our parent matrix times our local matrix gives our global
       matrix, we premultiply by the inverse of the parent matrix to
       get the local matrix that will produce our existing global matrix.
       Whew!
    */
    ether_matrix_inverse(tmp, newparent->globalmat);
    ether_matrix_multiply(obj->localmat, tmp, obj->globalmat);
    obj->moved = 1;
    return oldparent;
}

EtherObject *
ether_object_detach(EtherObject *obj)
{
    EtherObject *oldparent;
    if(obj == NULL)
        return NULL;
    oldparent = obj->parent;
    if(oldparent == NULL) 
        return oldparent;
    if(ether_object_is_fixed(obj))
    return oldparent;
    if(obj == oldparent->children)
        oldparent->children = obj->siblings;
    else
    {
        EtherObject *p;
        for(p = oldparent->children; p; p = p->next)
            if (p->siblings == obj)
            {
                p->siblings = obj->siblings;
                break;
            }
    }
    obj->parent = NULL;
    obj->siblings = NULL;
    ether_world_add_abject(obj);
    ether_matrix_copy(obj->localmat, obj->globalmat);
    obj->moved = 0;
    return oldparent;
}

EtherObject *
ether_object_update(EtherObject *obj)
{
    if(obj == NULL)
        return NULL;
    _ether_obj_list = NULL;
    _ether_object_recursive_updater(obj, 0);
    return _ether_obj_list;
}

void
ether_object_traverse(EtherObject *obj, int (*function)(EtherObject *obj))
{
    EtherObject *o;
    for(o = obj; o; o = o->siblings)
    {
        if((*function)(o))
            return;
        if(o->children)
            ether_object_traverse(o->children, function);
    }
}

void
ether_object_make_fixed(EtherObject *obj)
{
    if(obj == NULL)
        return;
    if(ether_object_is_fixed(obj))
        return;
    ether_object_detach(obj);
    if(obj->shape)
        ether_shape_transform(obj->globalmat, obj->shape);
    ether_matrix_identity(obj->globalmat);
    obj->moved = 0;
    obj->fixed = 1;
}

void
ether_object_make_movable(EtherObject *obj)
{
    EtherShape *shape;
    if(obj == NULL)
        return;
    shape = ether_object_shape_get(obj);
    ether_matrix_identity(obj->localmat);
    if(shape)
    {
        EtherRep *rep = ether_shape_rep_get(shape, 0);
        EtherVector v;
        ether_rep_vertex_get(rep, 0, v);
        ether_vector_negative(v);
        ether_matrix_translation_set(obj->localmat, v);
        ether_shape_transform(obj->localmat, shape);
        ether_vector_negative(v);
        ether_matrix_translation_set(obj->localmat, v);
    }
    obj->moved = 1;
    obj->fixed = 0;
}

EtherObject *
ether_object_find_root(EtherObject *obj)
{
    if(obj == NULL)
        return NULL;
	while(obj->parent)
		obj = obj->parent;
	return obj;
}

EtherScalar
ether_object_compute_distance (EtherObject *obj1, EtherObject *obj2)
{
    EtherVector tmp1, tmp2, tmp3;
    if(obj1 == NULL || obj2 == NULL)
        return 0;
    ether_vector_add(tmp1, obj1->minbound, obj1->maxbound);
    tmp1[X] /= 2;
    tmp1[Y] /= 2;
    tmp1[Z] /= 2;
    ether_vector_add(tmp1, obj1->minbound, obj1->maxbound);
    tmp1[X] /= 2;
    tmp1[Y] /= 2;
    tmp1[Z] /= 2;
    ether_vector_sub(tmp3, tmp1, tmp2);
    return ether_vector_magnitude(tmp3);
}

void
ether_object_rot_x(EtherObject *obj, EtherAngle angle)
{
    if(obj)
        ether_object_rotate(obj, angle, X, ETHER_COORD_PARENT, NULL);
}

void
ether_object_rot_y(EtherObject *obj, EtherAngle angle)
{
    if(obj)
        ether_object_rotate(obj, angle, Y, ETHER_COORD_PARENT, NULL);
}

void
ether_object_rot_z(EtherObject *obj, EtherAngle angle)
{
    if(obj)
        ether_object_rotate(obj, angle, Z, ETHER_COORD_PARENT, NULL);
}

void
ether_object_vector_move(EtherObject *obj, EtherVector v)
{
    if(obj)
        ether_object_move(obj, v[X], v[Y], v[Z]);
}

void
ether_object_vector_rel_move(EtherObject *obj, EtherVector v)
{
    if(obj)
        ether_object_translate(obj, v, ETHER_COORD_PARENT, NULL);
}

void
ether_object_layer_set(EtherObject *obj, unsigned char layer)
{
	if(obj)
		obj->layer = layer;
}

unsigned char
ether_object_layer_get(EtherObject *obj)
{
    if(obj)
        return obj->layer;
	return 0;
}

void
ether_object_shape_set(EtherObject *obj, EtherShape *shp)
{
    if(obj)
        obj->shape = shp;
}

EtherShape *
ether_object_shape_get(EtherObject *obj)
{
    if(obj)
        return obj->shape;
    else
        return NULL;
}

void
ether_object_surfacemap_set(EtherObject *obj, EtherSurfaceMap *map)
{
    if(obj)
        obj->surfmap = map;
}

EtherSurfaceMap *
ether_object_surfacemap_get(EtherObject *obj)
{
    if(obj)
        return obj->surfmap;
    else
        return NULL;
}

void
ether_object_highlight_set(EtherObject *obj, int high)
{
    if(obj)
        obj->highlight = high;
}

int
ether_object_highlight_get(EtherObject *obj)
{
    if(obj)
        return obj->highlight;
    else
        return 0;
}

void
ether_object_toggle_highlight(EtherObject *obj)
{
    if(obj)
        obj->highlight = !(obj->highlight);
}

void
ether_object_visibility_set(EtherObject *obj, int vis)
{
    if(obj)    
        obj->invisible = vis ? 1 : 0;
}

int
ether_object_visibility_get(EtherObject *obj)
{
    if(obj)
        return obj->invisible;
	return 0;
}

void
ether_object_toggle_visibility(EtherObject *obj)
{
    if(obj)
        obj->invisible = !(obj->invisible);
}

int
ether_object_is_fixed(EtherObject *obj)
{
    if(obj)
        return obj->fixed;
	return 0;
}

EtherVector  *
ether_object_minbounds_get(EtherObject *obj, EtherVector v)
{
    if(obj)
        return ether_vector_copy(v, obj->minbound);
    else
        return NULL;
}

EtherVector *
ether_object_maxbounds_get(EtherObject *obj, EtherVector v)
{
    if(obj)
        return ether_vector_copy(v, obj->maxbound);
    else
        return NULL;
}

void
ether_object_rep_set(EtherObject *obj, EtherRep *r)
{
    if(obj)
        obj->forced_rep = r;
}

EtherRep  *
ether_object_rep_get(EtherObject *obj)
{
    if(obj)
        return obj->forced_rep;
    else
        return NULL;
}

EtherScalar ether_object_world_x_get(EtherObject *obj)
{
    if(obj)
        return obj->globalmat[3][X];
	return 0;
}

EtherScalar 
ether_object_world_y_get(EtherObject *obj)
{
    if(obj)
        return obj->globalmat[3][Y];
	return 0;
}

EtherScalar  
ether_object_world_z_get(EtherObject *obj)
{
    if(obj)
        return obj->globalmat[3][Z];
	return 0;
}

EtherVector *
ether_object_world_location_get(EtherObject *obj, EtherVector v)
{
    if(obj)
        return ether_vector_copy(v, obj->globalmat[3]);
    else
        return NULL;
}

EtherScalar
ether_object_relative_x_get(EtherObject *obj)
{
    if(obj)
        return obj->localmat[3][X];
	return 0;
}

EtherScalar
ether_object_relative_y_get(EtherObject *obj)
{
    if(obj)
        return obj->localmat[3][Y];
	return 0;
}

EtherScalar
ether_object_relative_z_get(EtherObject *obj)
{
    if(obj)
        return obj->localmat[3][Z];
	return 0;
}

EtherVector *
ether_object_relative_location_get(EtherObject *obj, EtherVector v)
{
    if(obj)
        return ether_vector_copy(v, obj->localmat[3]);
    else
        return NULL;
}

void
ether_object_world_rotations_get(EtherObject *obj, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
    if(obj)
        ether_matrix_rotations_get(obj->globalmat, rx, ry, rz);
	else
		*rx = 0; *ry = 0; *rz = 0;
}

void
ether_object_relative_rotations_get(EtherObject *obj, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
   if(obj)
        ether_matrix_rotations_get(obj->localmat, rx, ry, rz);
   else
		*rx = 0; *ry = 0; *rz = 0;
}

void 
ether_object_name_set(EtherObject *obj, char *str)
{
    if(obj)
        obj->name = str;
}

char *
ether_object_name_get(EtherObject *obj)
{
    if(obj)
        return obj->name;
    else
        return NULL;
}

void 
ether_object_application_data_set(EtherObject *obj, void *data)
{
    if(obj)
        obj->applic_data = data;
}

void *
ether_object_application_data_get(EtherObject *obj)
{
    if(obj)
        return obj->applic_data;
    else
        return NULL;
}

void 
ether_object_forward_vector_get(EtherObject *obj, EtherVector v)
{
     if(obj)
        ether_matrix_basis_get(v, obj->globalmat, Z);
}

void 
ether_object_right_vector_get(EtherObject *obj, EtherVector v)
{
  if(obj)    
    ether_matrix_basis_get(v, obj->globalmat, X);
}

void 
ether_object_up_vector_get(EtherObject *obj, EtherVector v)
{
  if(obj)    
    ether_matrix_basis_get(v, obj->globalmat, Y);
}

EtherObject *
ether_object_parent_get(EtherObject *obj)
{
  if(obj)
    return obj->parent;
  else
    return NULL;
}

void
ether_object_function_set(EtherObject *obj, EtherObjectFunction function)
{
  if(obj)
    obj->function = function;
}

EtherObjectFunction 
ether_object_function_get(EtherObject *obj)
{
  if(obj)
    return obj->function;
  else
    return NULL;
}

EtherObject *
ether_object_load_from_plg(FILE *in)
{
	EtherShape *shape = ether_plg_read(in);
	if(shape == NULL)
		return NULL;
	return ether_object_create(shape);
}

static void
 _ether_object_transform_bbox(EtherObject *object)
{
  int i, j;
  EtherVector *minb = &object->shape->minbound;
  EtherVector *maxb = &object->shape->maxbound;
  if(!object->rotate_box && object->shape)
  {
    ether_vector_add(object->minbound, object->shape->minbound, object->globalmat[_ETHER_TRANS]);
    ether_vector_add(object->maxbound, object->shape->maxbound, object->globalmat[_ETHER_TRANS]);
    return;
  }
  ether_vector_copy(object->minbound, object->globalmat[_ETHER_TRANS]);
  ether_vector_copy(object->maxbound, object->globalmat[_ETHER_TRANS]);
  if(object->shape == NULL)
    return;
  for(i = 0; i < 3; ++i)
    for(j = 0; j < 3; ++j)
    {
      EtherScalar a = ether_math_factor_multiply(object->globalmat[i][j], (*minb)[j]);
      EtherScalar b = ether_math_factor_multiply(object->globalmat[i][j], (*maxb)[j]);
      if (a < b)
      {
        object->minbound[i] += a;
        object->maxbound[i] += b;
      }
      else
      {
        object->minbound[i] += b;
        object->maxbound[i] += a;
      }
    }
}
    
static void
_ether_object_recursive_updater(EtherObject *object, int flag)
{
  while(object)
  {
    if(object->function)
      (*object->function)(object);
    if((flag || object->moved) && ! ether_object_is_fixed(object))
    {
      if (object->parent == NULL)  /* no parent; our localmat *is* our globalmat */
        ether_matrix_copy(object->globalmat, object->localmat);
      else if (ether_object_is_fixed(object->parent))  /* same if our parent is fixed */
        ether_matrix_copy(object->globalmat, object->localmat);
      else /* normal case; multiply our parent's globalmat by our localmat to get our globalmat */
        ether_matrix_multiply(object->globalmat, object->parent->globalmat, object->localmat);
      _ether_object_transform_bbox(object);
    }
    if(object->children)
      _ether_object_recursive_updater(object->children, flag || object->moved);
    object->moved = 0;
    object->next = _ether_obj_list;
    _ether_obj_list = object;
    object = object->siblings;
  }
}
