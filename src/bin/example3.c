/* EXAMPLE3 -- surface maps */
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

void ether_blit(EtherRaster *raster);
void ether_key(int key_code);

int main(int argc, char **argv)
{
  FILE *infile;
  int i;

  EtherLight *light = NULL;
  EtherCamera *camera = NULL;
  EtherShape *colorthing = NULL;
  EtherSurfaceMap *map1, *map2;

  ether_system_startup();

  map1 = ether_surface_map_create(6);
  map2 = ether_surface_map_create(6);
  for (i = 0; i < 6; ++i) {
    ether_surface_map_surface_set(map1, i, ether_surface_create(i + 1));
    ether_surface_map_surface_set(map2, i, ether_surface_create(7 + i));
  }

  infile = fopen("./plg/colorful.plg", "r");
  if (infile) {
    colorthing = ether_plg_read(infile);
    fclose(infile);
  }

  light = ether_light_create();
  ether_light_rot_y(light, ETHER_FLOAT_TO_ANGLE(45));
  ether_light_rot_x(light, ETHER_FLOAT_TO_ANGLE(45));

  camera = ether_camera_create();
  ether_camera_move(camera, 0, 100, -50);

  for (i = 0; i < 10; ++i) {
    EtherObject *obj = ether_object_create(colorthing);
    if (i & 1)
      ether_object_surfacemap_set(obj, map1);
    else
      ether_object_surfacemap_set(obj, map2);
    ether_object_move(obj, rand() % 1000, rand() % 1000, rand() % 1000); 
  }

  ether_system_run();

  return 0;
}

/*
void ether_key(int key_code)
{
  EtherCamera* cam = ether_world_cameras_get();

  switch (key_code) {
    case ALLEGRO_KEY_PGUP:
    case ALLEGRO_KEY_PGDN:
    case ALLEGRO_KEY_LEFT:
    case ALLEGRO_KEY_RIGHT:
    case ALLEGRO_KEY_UP:
    case ALLEGRO_KEY_DOWN:
      break;
    case ALLEGRO_KEY_PAD_PLUS:
      ether_camera_zoom_set(cam, ether_camera_zoom_get(cam) * 1.1); 
      printf("PLUS");
      break;
    case ALLEGRO_KEY_PAD_MINUS: 
      ether_camera_zoom_set(cam, ether_camera_zoom_get(cam) * 0.9); 
      printf("MINUS");
      break;
    case ALLEGRO_KEY_EQUALS: 
      ether_camera_zoom_set(cam, 1.0); 
      printf("EQUALS");
      break;
    default: break;
  }

  ether_system_request_refresh();
}
*/
