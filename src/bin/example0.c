#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Ether.h>

#include <stdio.h>

#define WIDTH 650
#define HEIGHT 490


void ether_blit(EtherRaster *raster);
void draw_palette(void);

int main(int argc, char **argv)
{
  ether_system_startup();
  draw_palette();
  ether_system_run();

  return 0;
}

#define WIDTH_BOX 25
#define HEIGTH_BOX 25

void draw_palette(void)
{
  EtherPalette palette;
  EtherColor color = 0;
  char message[10];
  int x,y,l,c;
  int a,b;

  ether_palette_init(&palette);

  x = y = 0;
  for(l=0; l<16; l++) {
    for(c=0; c<16; c++) {
      snprintf(message, sizeof(message), "%d", color);
      a = (int)((WIDTH_BOX - ether_display_text_width_get(message))/2);
      b = (int)((HEIGTH_BOX - ether_display_text_height_get(message))/2);

      ether_display_box(x, y, x+WIDTH_BOX, y+HEIGTH_BOX, color);
      ether_display_text(x+a, y+b,15,message);

      color++;
      x += 2 + WIDTH_BOX;
    }
    x = 0;
    y += 2 + HEIGTH_BOX;
  }
}

