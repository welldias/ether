/* EXAMPLE2 -- several asteroids, sharing the same geometry */
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
#include <stdlib.h>  /* needed for rand() */

void ether_rot_obj(void);

int main(int argc, char **argv)
{
  FILE *infile;
  EtherLight *light = NULL;
  EtherShape *plgshape = NULL;
  EtherCamera *camera = NULL;
  EtherObject *plg = NULL;

  ether_system_startup();

  ether_world_horizon_set(0);   /* turn off horizon */
  ether_world_sky_color_set(0); /* black sky */

  infile = fopen("./plg/balloon.plg", "r");
  if(infile) {
    plgshape = ether_plg_read(infile);
    fclose(infile);
  }

  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));
  ether_light_rot_x(light, ETHER_FLOAT_TO_ANGLE(45));
  ether_light_intensity_set(light, ETHER_FLOAT_TO_FACTOR(0.9));

  camera = ether_camera_create();
  ether_camera_move(camera, 0, 0, -1500);

  plg = ether_object_create(plgshape);
  //ether_object_move(plg, rand() % 100, rand() % 100, rand() % 100);

  ether_task_create(ether_rot_obj, plg, 0);
  ether_system_run();

  return 0;
}

void ether_rot_obj(void)
{
  EtherObject *obj = (EtherObject*)ether_task_data_get();
  ether_object_rot_y(obj, 0.1);
  ether_object_rot_x(obj, 0.1);
  ether_object_rot_z(obj, 0.1);
  ether_system_request_refresh();
}
