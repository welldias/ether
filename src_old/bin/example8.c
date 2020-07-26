/* EXAMPLE8 -- Hierarchical objects */
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

EtherAngle sun_speed;

void ether_orbit(void);
void ether_rot(void);

typedef struct
{
  EtherObject object;
  EtherAngle speed;
} PlanetData;

int main(int argc, char **argv)
{
  EtherShape *planet_shape;
  EtherObject *sun;
  EtherLight *light;
  EtherCamera *camera;
  EtherSurface *surf;
  EtherSurfaceMap *surfmap;
  EtherVector v;
  PlanetData planet1, planet2, moon;

  /* startup the system, create a light and a camera */
  ether_system_startup();
  ether_world_ambient_set(0);

  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));

  camera = ether_camera_create();
  ether_camera_rot_x(camera, ETHER_FLOAT_TO_ANGLE(90));
  ether_camera_move(camera, 0, 3000, 0);

  /* create the geometry that all the celestial bodies will use */	
  planet_shape = ether_primitives_sphere(30, 8, 8, NULL);

  /* create the sun */
  sun = ether_object_create(planet_shape);

  /* create a surfacemap for the sun */
  surfmap = ether_surface_map_create(1);
  surf =  ether_surface_create(5);
  ether_surface_map_surface_set(surfmap, 0, surf);
  ether_object_surfacemap_set(sun, surfmap);
  sun_speed = ETHER_FLOAT_TO_ANGLE(8.0 / ether_timer_tick_rate_get());
  ether_task_create(ether_rot, sun, 0);

  /* create a planet */
  ether_object_init(&planet1.object);
  ether_object_shape_set(&planet1.object, planet_shape);

  /* give it a surfacemap */
  surfmap = ether_surface_map_create(1);
  surf = ether_surface_create(7);
  ether_surface_map_surface_set(surfmap, 0, surf);
  ether_object_surfacemap_set(&planet1.object, surfmap);

  /* attach it to the sun and displace it */
  ether_object_attach(&planet1.object, sun);
  ether_vector_create(v, 0, 0, -250);
  ether_object_translate(&planet1.object, v, ETHER_COORD_PARENT, NULL);
  
  /* make it orbit */
  planet1.speed = ETHER_FLOAT_TO_ANGLE(36.0 / ether_timer_tick_rate_get());
  ether_task_create(ether_orbit, &planet1, 0);

  /* create a second planet */
  ether_object_init(&planet2.object);
  ether_object_shape_set(&planet2.object, planet_shape);

  /* give it a surfacemap */
  surfmap = ether_surface_map_create(1);
  surf = ether_surface_create(9);
  ether_surface_map_surface_set(surfmap, 0, surf);
  ether_object_surfacemap_set(&planet2.object, surfmap);

  /* attach it to the sun and displace it */
  ether_object_attach(&planet2.object, sun);
  ether_vector_create(v, 0, 0, -500);
  ether_object_translate(&planet2.object, v, ETHER_COORD_PARENT, NULL);

  /* make it orbit */
  planet2.speed = ETHER_FLOAT_TO_ANGLE(18.0 / ether_timer_tick_rate_get());
  ether_task_create(ether_orbit, &planet2, 0);

  /* create a moon for the second planet */
  ether_object_init(&moon.object);
  ether_object_shape_set(&moon.object, planet_shape);

  /* give it a surfacemap */
  surfmap = ether_surface_map_create(1);
  surf = ether_surface_create(11);
  ether_surface_map_surface_set(surfmap, 0, surf);
  ether_object_surfacemap_set(&moon.object, surfmap);

  /* attach it to the sun and displace it */
  ether_object_attach(&moon.object, &planet2.object);
  ether_vector_create(v, 0, 0, -80);
  ether_object_translate(&moon.object, v, ETHER_COORD_PARENT, NULL);

  /* make it orbit */
  moon.speed = ETHER_FLOAT_TO_ANGLE(72.0 / ether_timer_tick_rate_get());
  ether_task_create(ether_orbit, &moon, 0);

  ether_system_run();

  return 0;
}

void ether_orbit(void)
{
  PlanetData *data = (PlanetData *)ether_task_data_get();
  ether_object_rotate(&data->object, 
      ether_task_elapsed_get() * data->speed,
      Y, 
      ETHER_COORD_PARENT, 
      NULL);
  ether_system_request_refresh();
}

void ether_rot(void)
{
  EtherObject *obj = (EtherObject*)ether_task_data_get();
  ether_object_rotate(obj, 
      ether_task_elapsed_get() * sun_speed,
      Y, 
      ETHER_COORD_PARENT, 
      NULL);
  ether_system_request_refresh();
}
