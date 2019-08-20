/* EXAMPLE4 -- simple object behaviours */
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
#include <stdlib.h>  /* needed for rand() */
#include <Ether.h>

static EtherAngle spinrate;
static EtherTime bounce_period;
static EtherScalar maxheight;
static EtherTime pulse_period;

static void spin(void);
static void bounce(void);
static void pulsate(void);

int main(int argc, char **argv)
{
  EtherLight *light = NULL;
  EtherCamera *camera = NULL;
  EtherShape *cube, *sphere, *cylinder;
  EtherSurfaceMap *cubemap, *pulsemap;
  int i;

  ether_system_startup();

  cube = ether_primitives_box(100, 100, 100, NULL);
  sphere = ether_primitives_sphere(100, 6, 6, NULL);
  cylinder = ether_primitives_cylinder(100, 50, 100, 8, NULL);

  cubemap = ether_surface_map_create(1);
  ether_surface_map_surface_set(cubemap, 0, ether_surface_create(5));
  pulsemap = ether_surface_map_create(1);
  ether_surface_map_surface_set(pulsemap, 0, ether_surface_create(14));

  spinrate = ETHER_FLOAT_TO_ANGLE(72.0 / ether_timer_tick_rate_get());  /* deg per tick */
  bounce_period = 4 * ether_timer_tick_rate_get();  /* four-second period */
  maxheight = ETHER_FLOAT_TO_SCALAR(400);    /* maximum height in units */
  pulse_period =  2 * ether_timer_tick_rate_get();  /* two-second period */

  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));
  ether_light_rot_x(light, ETHER_FLOAT_TO_ANGLE(45));

  camera = ether_camera_create();
  ether_camera_rot_y(camera, ETHER_FLOAT_TO_ANGLE(5));
  ether_camera_move(camera, 0, 200, -4400);

  for (i = 0; i < 10; ++i)
  {
    EtherObject *obj = ether_object_create(NULL);
    ether_object_move(obj, rand() % 1000, rand() % 1000, rand() % 1000);
    switch (i & 3)
    {
      case 0:
        ether_object_shape_set(obj, cube);
        break;
      case 1:
        ether_object_shape_set(obj, cube);
        ether_object_surfacemap_set(obj, cubemap);
        ether_task_create(spin, obj, 10);
        break;
      case 2:
        ether_object_shape_set(obj, sphere);
        ether_task_create(bounce, obj, 10);
        break;
      case 3:
        ether_object_shape_set(obj, cylinder);
        ether_object_surfacemap_set(obj, pulsemap);
        break;
    }
    ether_task_create(pulsate, pulsemap, 10);
  }

  ether_system_run();

  return 0;
}

static void spin(void)
{
  ether_object_rot_y(ether_task_data_get(), ether_task_elapsed_get() * spinrate);
  ether_system_request_refresh();
}

static void bounce(void)
{
  unsigned long off;
  EtherScalar height;
  EtherObject *obj = ether_task_data_get();

  off = (360 * (ether_task_time_now_get() % bounce_period)) / bounce_period;
  height = ether_math_factor_multiply(ether_math_sine(ETHER_FLOAT_TO_ANGLE(off)), maxheight);
  ether_object_move(obj, ether_object_world_x_get(obj), height, ether_object_world_z_get(obj));
  ether_system_request_refresh();
}

static void pulsate(void)
{
  unsigned long off;
  int brightness;
  EtherSurface *surf = ether_surface_map_surface_get((EtherSurfaceMap*) ether_task_data_get(), 0);

  off = (360 * (ether_task_time_now_get() % pulse_period)) / pulse_period;
  brightness = abs(ether_math_factor_multiply(ether_math_sine(ETHER_FLOAT_TO_ANGLE(off)), 255));
  ether_surface_brightness_set(surf, brightness);
  ether_system_request_refresh();
}
