/* EXAMPLE7 -- Gouraud shading */
/* 

   Copyright 1994, 1995, 1996, 1997, 1998, 1999  by Bernie Roehl 

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA,
   02111-1307, USA.

   For more information, contact Bernie Roehl <broehl@uwaterloo.ca> 
   or at Bernie Roehl, 68 Margaret Avenue North, Waterloo, Ontario, 
   N2J 3P7, Canada

*/

#include <Ether.h>
#include <stdio.h>

static EtherAngle tumblerate;

static void load_palette(char *filename);
void tumbler(void);

int main(int argc, char **argv)
{
  EtherShape *smooth_shape;
  EtherObject *thing;
  EtherLight *light;
  EtherCamera *camera;
  EtherSurface *surf;

  ether_system_startup();

  load_palette("shade32.pal");

  smooth_shape = ether_primitives_cylinder(100, 25, 200, 16, NULL);
  ether_shape_compute_vertex_normals(smooth_shape);

  surf = ether_surface_map_surface_get(ether_shape_surface_map_get(smooth_shape), 0);
  ether_surface_type_set(surf, ETHER_SURF_GOURAUD);
  ether_surface_hue_set(surf, 4);
  ether_surface_brightness_set(surf, 243);

  thing = ether_object_create(smooth_shape);
  ether_object_rel_move(thing, 0, -100, 0);

  ether_world_ambient_set(0);
  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));

  camera = ether_camera_create();
  ether_camera_move(camera, 0, 0, -1400);

  tumblerate = ETHER_FLOAT_TO_ANGLE(72.0 / ether_timer_tick_rate_get());
  ether_task_create(tumbler, thing, 0);

  ether_system_run();

  return 0;
}

static void load_palette(char *filename)
{
  FILE *infile = fopen(filename, "rb");
  if (infile) {
    ether_palette_read(ether_world_palette_get(), infile);
    fclose(infile);
  }
}

void tumbler(void)
{
  EtherObject *obj = (EtherObject*)ether_task_data_get();
  EtherAngle amount = ether_task_elapsed_get() * tumblerate;
  ether_object_rot_y(obj, amount);
  ether_object_rot_x(obj, amount);
  ether_system_request_refresh();
}
