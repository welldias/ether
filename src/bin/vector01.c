#include <allegro5/allegro.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
   float x;
   float y;
} Point;

typedef struct {
  Point p0;
  Point p1;
  int vx;
  int vy;
  long time;
} Object;

int main(int argc, char **argv)
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_KEYBOARD_STATE key_state;
  Object obj;

  if (!al_init()) {
    printf("Could not init Allegro.\n");
    return -1;
  }

  obj.p0.x = 100;
  obj.p0.y = 150;
  obj.vx   = 3;
  obj.vy   = 1;
  obj.time = al_get_time();

  al_install_keyboard();

  display = al_create_display(WIDTH, HEIGHT);
  if (!display) {
    printf("Could not create display.\n");
    return -1;
  }

  while (1) {
    
    obj.time = al_get_time();
    obj.p1.x = obj.p0.x + obj.vx ;
    obj.p1.y = obj.p0.y + obj.vy ;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_lock_bitmap(al_get_backbuffer(display), ALLEGRO_PIXEL_FORMAT_ANY, 0);

    al_put_pixel(obj.p1.x, obj.p1.y, al_map_rgba(255, 255, 255, 255));

    al_unlock_bitmap(al_get_backbuffer(display));
    al_flip_display();

    al_rest(0.101);

    al_get_keyboard_state(&key_state);
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE))
      break;
    if (al_key_down(&key_state, ALLEGRO_KEY_UP))
      obj.vy--;
    if (al_key_down(&key_state, ALLEGRO_KEY_DOWN))
      obj.vy++;
    if (al_key_down(&key_state, ALLEGRO_KEY_LEFT))
      obj.vx--;
    if (al_key_down(&key_state, ALLEGRO_KEY_RIGHT))
      obj.vx++;

    if(obj.p1.x > WIDTH)
      obj.p1.x = 0;
    else if(obj.p1.x < 0)
      obj.p1.x = WIDTH;

    if(obj.p1.y > HEIGHT)
      obj.p1.y = 0;
    else if(obj.p1.y < 0)
      obj.p1.y = HEIGHT;

    obj.p0.x = obj.p1.x;
    obj.p0.y = obj.p1.y;
  }

  al_destroy_display(display);

  return 0;
}
