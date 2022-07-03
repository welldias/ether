#include <Ether.h>
#include <stdio.h>

void ether_rot_obj(void);
void ether_blit(EtherRaster *raster);
void ether_coloring(EtherObject* obj);

int main(int argc, char **argv)
{
  EtherObject *cube = NULL;
  EtherCamera *camera = NULL;
  EtherLight  *light = NULL;

  //ether_system_blit_func_set(ether_blit);
  ether_system_startup();
  //ether_render_draw_mode_set(1);

  cube = ether_object_create(ether_primitives_box(100, 100, 100, NULL));
  ether_object_rot_y(cube, ETHER_FLOAT_TO_ANGLE(45));
  ether_coloring(cube);

  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));
  ether_light_rot_x(light, ETHER_FLOAT_TO_ANGLE(45));

  camera = ether_camera_create();
  ether_camera_rot_x(camera, ETHER_FLOAT_TO_ANGLE(45));
  ether_camera_move(camera, 0, 500, -500);

  ether_task_create(ether_rot_obj, cube, 0);
  ether_system_run();

  return 0;
}

void ether_rot_obj(void)
{
  EtherObject *obj = (EtherObject*)ether_task_data_get();
  ether_object_rot_y(obj, 0.1);
  ether_system_request_refresh();
}

void ether_coloring(EtherObject* obj)
{
  int x;
  EtherSurface *surf;
  EtherSurfaceMap *map = ether_object_surfacemap_get(obj);
  int count = ether_surface_map_count_entries(map);

  for(x=0;x<count;x++)
  {
    surf = ether_surface_map_surface_get(map,x);
    ether_surface_hue_set(surf, x+1);
  }
}
