#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "ether_allegro.h"
#include "ether_math.h"
#include "ether_palette.h"
#include "ether_private.h"
#include "ether_raster.h"
#include "ether_display.h"

#define _ETHER_ALLEGRO_SCREEN_UNITY                 (1L << _ETHER_SCREEN_FRACT_BITS)
#define _ETHER_ALLEGRO_SCREEN_HALF_UNITY            (1L << (_ETHER_SCREEN_FRACT_BITS-1))
#define _ETHER_ALLEGRO_SCREEN_HALF_MASK             (_ETHER_ALLEGRO_SCREEN_UNITY - 1)
#define _ETHER_ALLEGRO_NUMSPECS                     10

#define _ETHER_ALLEGRO_SETPIXEL(buff, x, y, color)  buff[_ether_allegro_line_table[(y)+_ether_allegro_top_border]+(x)+_ether_allegro_left_border] = (color)
#define _ETHER_ALLEGRO_S2I(a)                       ((a) >> _ETHER_SCREEN_FRACT_BITS)
#define _ETHER_ALLEGRO_HFILL(buff, x1, x2, y, color) \
	if (x1 <= x2) \
		memset(&buff[_ether_allegro_line_table[y] + (x1)], (color), (x2) - (x1) + 1)

/* #define FITTING 1 */
#ifdef FITTING
#define _ETHER_ALLEGRO_ROUND_DOWN(a) (_ETHER_ALLEGRO_S2I(a) - (((a) & _ETHER_ALLEGRO_SCREEN_HALF_MASK) ? 0 : 1))
#define _ETHER_ALLEGRO_ROUND_UP(a)    _ETHER_ALLEGRO_S2I((a) + _ETHER_ALLEGRO_SCREEN_UNITY - 1)
#else
#define _ETHER_ALLEGRO_ROUND_DOWN(a)  _ETHER_ALLEGRO_S2I(a)
#define _ETHER_ALLEGRO_ROUND_UP(a)   (_ETHER_ALLEGRO_S2I((a)+_ETHER_ALLEGRO_SCREEN_HALF_UNITY))
#endif

typedef struct {
	int startx;
	int endx;
	int startcolor;
	int endcolor;
} EtherDriversSpan;

static EtherRaster       *_ether_allegro_our_raster     = NULL;
static unsigned int      *_ether_allegro_line_table     = NULL;
static EtherFactor       *_ether_allegro_slope_table    = NULL;
static EtherDriversSpan   _ether_allegro_spans[500];
static unsigned char      _ether_allegro_specular_table[_ETHER_ALLEGRO_NUMSPECS][256];
static int                _ether_allegro_text_x         = 0;
static int                _ether_allegro_text_y         = 0;
static int                _ether_allegro_top_border;
static int                _ether_allegro_left_border;
static int                _ether_allegro_starty;          /* starting Y coordinate for the poly being processed */
static int                _ether_allegro_endy;            /* ending Y coordinate for the poly being processed */
static int                _ether_allegro_dither[4][4]   = {
	{((15-(0)) <<4),((15-(8)) << 4),((15-(2)) << 4),((15-(10))<< 4)},
	{((15-(12))<<4),((15-(4)) << 4),((15-(14))<< 4),((15-(6)) << 4)},
	{((15-(3)) <<4),((15-(11))<< 4),((15-(1)) << 4),((15-(9)) << 4)},
	{((15-(15))<<4),((15-(7)) << 4),((15-(13))<< 4),((15-(5)) << 4)},
};

static EtherRaster _ether_allegro_video_raster = {
  1024, /* width  */
   768, /* height */
     8, /* depth  */
     0, /* top */
     0, /* left */
  1023, /* right */
   767, /* bottom   */
  1024, /* rowbytes */ 
	NULL
};

ALLEGRO_DISPLAY *_allegro_display = NULL;
ALLEGRO_FONT *_allegro_font = NULL;

static void   _ether_allegro_render_gouraud_poly       (void);
static void   _ether_allegro_render_dithered_poly      (void);
static void   _ether_allegro_render_fast_dithered_poly (void);
static void   _ether_allegro_specular_update           (EtherPalette *palette);
static int    _ether_allegro_compute_line_table        (void);
static int    _ether_allegro_compute_slope_table       (void);
static int    _ether_allegro_render_lines              (EtherOutputvertex *list, int closed);
static int    _ether_allegro_render_text               (int x, int y, char* text, unsigned char color);
static int    _ether_allegro_scan_outline              (EtherOutputvertex *list, EtherColor color);
static void (*_ether_allegro_shadefunc)                (void) = _ether_allegro_render_dithered_poly;

int 
ether_allegro_display_default(EtherDisplayCommand cmd, int lparm1, void *pparm1)
{
  switch (cmd)
  {
    case ETHER_DISPLAY_GET_VERSION: 
      return al_get_allegro_version();
    case ETHER_DISPLAY_GET_DESCRIPTION: 
      {
        uint32_t version = al_get_allegro_version();
        snprintf((char *) pparm1, lparm1, 
            "Allegro  v%d (%d:%d %d:%d)",
            version,               /* version  */
            version >> 24,         /* major    */
            (version >> 16) & 255, /* minor    */
            (version >> 8) & 255,  /* revision */
            (version) & 255);        /* release  */
        return 0;
      }
    case ETHER_DISPLAY_INIT:
      {
        /* if could not init Allegro */
        if (!al_init()) {
          return -1;
        }

        if (!al_init_primitives_addon()) {
            return -1;
        }
        al_init_font_addon();
        al_install_mouse();
        al_install_keyboard();

        if (pparm1)
        {
          _ether_allegro_our_raster = pparm1;
        }
        else
        {
          EtherRaster *hardware_raster = ether_video_raster_get();
          _ether_allegro_our_raster = ether_raster_create(
              ether_raster_width_get(hardware_raster),
              ether_raster_height_get(hardware_raster),
              ether_raster_depth_get(hardware_raster));
          if(_ether_allegro_our_raster == NULL)
            return -1;
        }
        _allegro_display = al_create_display(
            ether_raster_width_get(_ether_allegro_our_raster),
            ether_raster_height_get(_ether_allegro_our_raster));
        if (!_allegro_display) {
          return -1;
        }

        _allegro_font = al_create_builtin_font();

        if(_ether_allegro_compute_slope_table())
          return -2;
        if(_ether_allegro_compute_line_table())
          return -3;

      }
    case ETHER_DISPLAY_CLEAR: 
      {
        /* TODO: precisa ajustar a cor (usar lparm1)*/
        al_clear_to_color(al_map_rgb(0,0,0));
        break;
      }
    case ETHER_DISPLAY_QUIT:
      al_destroy_font(_allegro_font);
      al_destroy_display(_allegro_display);
      break;
    case ETHER_DISPLAY_POINT:
      {
        EtherOutputvertex *vertlist = (EtherOutputvertex *)pparm1;
        EtherOutputvertex *v        = (EtherOutputvertex *)pparm1;
        do
        {
          al_put_pixel(
              (int)_ETHER_ALLEGRO_S2I(v->x),
              (int)_ETHER_ALLEGRO_S2I(v->y),
              al_map_rgb(v->red, v->green, v->blue));
          v = v->next;
        } while(v != vertlist);
        break;
      }
    case ETHER_DISPLAY_LINE: 
      _ether_allegro_render_lines((EtherOutputvertex*)pparm1, 0); 
      break;
    case ETHER_DISPLAY_CLOSED_LINE: 
      _ether_allegro_render_lines((EtherOutputvertex*)pparm1, 1); break;
    case ETHER_DISPLAY_POLY:
      {
        EtherOutputfacet *f = (EtherOutputfacet*)pparm1;
        _ether_allegro_scan_outline(f->points, f->color);
        break;
      }
    case ETHER_DISPLAY_BOX:
      {
        EtherOutputvertex *v = pparm1;
        int row    = (int)_ETHER_ALLEGRO_S2I(v->y);
        int endrow = (int)_ETHER_ALLEGRO_S2I(v->next->y);
        int xstart = (int)_ETHER_ALLEGRO_S2I(v->x);
        int xend   = (int)_ETHER_ALLEGRO_S2I(v->next->x);
        
           unsigned char *data = ether_raster_data_get(_ether_allegro_our_raster);
           if(!data)
           break;
           for(row = (int)_ETHER_ALLEGRO_S2I(v->y); row <= endrow; ++row)
           _ETHER_ALLEGRO_HFILL(data, xstart, xend, row, v->red);
/*
        al_draw_rectangle(
            xstart, 
            row, 
            xend, 
            endrow, 
            al_map_rgb(v->red, v->green, v->blue), 
            1);
*/
        break;
      }
    case ETHER_DISPLAY_BEGIN_FRAME:
      {
        /* TODO: precisa ajustar a cor (usar lparm1)*/
        al_clear_to_color(al_map_rgb(0,0,0));
        al_lock_bitmap(al_get_backbuffer(_allegro_display), ALLEGRO_PIXEL_FORMAT_ANY, 0);

        ether_raster_window_get(_ether_allegro_our_raster, &_ether_allegro_left_border, &_ether_allegro_top_border, NULL, NULL);
        break;
      }
    case ETHER_DISPLAY_END_FRAME:
      {
        al_unlock_bitmap(al_get_backbuffer(_allegro_display));
        break;
      }
    case ETHER_DISPLAY_TEXT:
      {
        /* TODO: precisa ajustar a cor (usar lparm1)*/
        _ether_allegro_render_text(
            _ether_allegro_text_x,
            _ether_allegro_text_y,
            (char*)pparm1,
            (unsigned char)lparm1);

        _ether_allegro_text_x += strlen((char*)pparm1);
        break;
      }
    case ETHER_DISPLAY_TEXT_POSITION: 
      _ether_allegro_text_x =  lparm1 >> 16; 
      _ether_allegro_text_y = (short)lparm1;
      break;
    case ETHER_DISPLAY_GET_TEXTWIDTH: 
      return (int)strlen(pparm1) * 8;
    case ETHER_DISPLAY_GET_TEXTHEIGHT: 
      return 8;
    case ETHER_DISPLAY_SET_RASTER: 
      _ether_allegro_our_raster = pparm1; 
      break;
    case ETHER_DISPLAY_GET_RASTER: 
      *((EtherRaster **) pparm1) = _ether_allegro_our_raster; 
      break;
    case ETHER_DISPLAY_CAN_GOURAUD: 
      return 1;
    case ETHER_DISPLAY_UPDATE_PALETTE: 
      _ether_allegro_specular_update(pparm1); 
      break;
    case ETHER_DISPLAY_SET_SHADING:
      switch (lparm1)
      {
        case 0: _ether_allegro_shadefunc = NULL; break;
        case 1: _ether_allegro_shadefunc = _ether_allegro_render_gouraud_poly; break;
        case 2: _ether_allegro_shadefunc = _ether_allegro_render_fast_dithered_poly; break;
        case 3: _ether_allegro_shadefunc = _ether_allegro_render_fast_dithered_poly; break;
        default: break;
      }
      break;
    default: break;
  }
  return 0;
}

int 
ether_allegro_video_default(EtherVideoCommand cmd, int lparm1, void *pparm1)
{
	switch (cmd)
	{
		case ETHER_VIDEO_GET_VERSION:
			return 1;
		case ETHER_VIDEO_GET_DESCRIPTION: 
			strncpy((char *) pparm1, "Default video Version 0.1", lparm1);
			return 0;
		case ETHER_VIDEO_SETUP:
			return 0;
		case ETHER_VIDEO_SHUTDOWN:
			break;
		case ETHER_VIDEO_GET_MODE:
			return 0;
		case ETHER_VIDEO_SET_DRAW_PAGE:
			break;
		case ETHER_VIDEO_SET_VIEW_PAGE:
			break;
		case ETHER_VIDEO_GET_NPAGES:
			return 1;
		case ETHER_VIDEO_HAS_PALETTE:
			return 1;
		case ETHER_VIDEO_SET_PALETTE:
			return 0;
		case ETHER_VIDEO_SET_NTSC:
			break;
		case ETHER_VIDEO_CHECK_RETRACE:
			break;
		case ETHER_VIDEO_GET_RASTER:
		{
			*((EtherRaster **) pparm1) = &_ether_allegro_video_raster;
			break;
		}
		case ETHER_VIDEO_BLIT:
		{
            al_flip_display();
			break;
		}
		case ETHER_VIDEO_CURSOR_HIDE:
			break;
		case ETHER_VIDEO_CURSOR_SHOW:
			break;
		case ETHER_VIDEO_CURSOR_RESET:
			break;
		case ETHER_VIDEO_CURSOR_MOVE:
			break;
		case ETHER_VIDEO_CURSOR_SET_APPEARANCE:
			break;
		default:
			break;
	}

	return 0;
}

static void 
_ether_allegro_render_gouraud_poly(void)
{
	int x;
	int y;
	int color;
	int cincrement;
	unsigned char *data = ether_raster_data_get(_ether_allegro_our_raster);
	if(!data)
		return;
	y = (_ether_allegro_endy + _ether_allegro_starty) >> 1;
	cincrement = (int)ether_math_factor_multiply(
		_ether_allegro_slope_table[_ether_allegro_spans[y].endx - _ether_allegro_spans[y].startx], 
		(EtherScalar)(_ether_allegro_spans[y].endcolor - _ether_allegro_spans[y].startcolor));
	for(y = _ether_allegro_starty; y <= _ether_allegro_endy; ++y)
	{
		unsigned char *ptr = &data[_ether_allegro_line_table[y]];
		color = _ether_allegro_spans[y].startcolor;
		for (x = _ether_allegro_spans[y].startx; x <= _ether_allegro_spans[y].endx; ++x)
		{
			ptr[x] = color >> 8;
			color += cincrement;
		}
	}
}

static void 
_ether_allegro_render_dithered_poly(void)
{
	int x;
	int y;
	int color;
	int cincrement;
	unsigned char *data = ether_raster_data_get(_ether_allegro_our_raster);
    
	if(!data)
		return;
	y = (_ether_allegro_endy + _ether_allegro_starty) >> 1;
	cincrement = (int)ether_math_factor_multiply(
		_ether_allegro_slope_table[_ether_allegro_spans[y].endx - _ether_allegro_spans[y].startx],
		(EtherScalar)(_ether_allegro_spans[y].endcolor - _ether_allegro_spans[y].startcolor));
	for(y = _ether_allegro_starty; y <= _ether_allegro_endy; ++y)
	{
		unsigned char *ptr = &data[_ether_allegro_line_table[y]];
		int *dtable = _ether_allegro_dither[y & 3];
		color = _ether_allegro_spans[y].startcolor;
		for(x = _ether_allegro_spans[y].startx; x <= _ether_allegro_spans[y].endx; ++x)
		{
			ptr[x] = ((unsigned int)((color + dtable[x&3]))) >> 8;
			color += cincrement;
		}
	}
}

static void 
_ether_allegro_render_fast_dithered_poly(void)
{
	int x;
	int y;
	int color;
	int cincrement;
	unsigned char *data = ether_raster_data_get(_ether_allegro_our_raster);
	if(!data)
		return;
	y = (_ether_allegro_endy + _ether_allegro_starty) >> 1;
	cincrement = (int)ether_math_factor_multiply(
		_ether_allegro_slope_table[_ether_allegro_spans[y].endx - _ether_allegro_spans[y].startx],
		(EtherScalar)(_ether_allegro_spans[y].endcolor - _ether_allegro_spans[y].startcolor));
	for(y = _ether_allegro_starty; y <= _ether_allegro_endy; ++y)
	{
		unsigned char *ptr = &data[_ether_allegro_line_table[y]];
		int a, b;
		if(y & 1)
		{ 
			a = 1 << 6;
			b = 3 << 6;
		}
		else
		{ 
			a = 2 << 6;
			b = 0 << 6;
		}
		color = _ether_allegro_spans[y].startcolor;
		for(x = _ether_allegro_spans[y].startx; x <= _ether_allegro_spans[y].endx; ++x)
		{
			ptr[x] = ((unsigned int) ((color + ((x&1) ? a : b)))) >> 8;
			color += cincrement;
		}
	}
}

static void 
_ether_allegro_specular_update(EtherPalette *palette)
{
	unsigned int hue;
	unsigned int shade;
	unsigned int specnum;
	unsigned char value;
	double exponent;
	EtherHue *huemap = ether_palette_huemap_get(palette);
	for(specnum = 0; specnum < _ETHER_ALLEGRO_NUMSPECS; ++specnum) {
		exponent = 1.0 + 7 * specnum / 100.0;
		for(shade = 0; shade <= huemap[0].maxshade; ++shade)
			_ether_allegro_specular_table[specnum][shade] = shade;

		for(hue = 1; hue < 256 && huemap[hue].maxshade; ++hue) {
			for(shade = 0; shade <= huemap[hue].maxshade; ++shade) {
				value = (unsigned char)pow(shade, exponent);
				if(value > huemap[hue].maxshade)
					value = huemap[hue].maxshade;
				_ether_allegro_specular_table[specnum][huemap[hue].start + shade] = huemap[hue].start + value;
			}
		}
	}
}

static int 
_ether_allegro_compute_line_table(void)
{
	int i;
	if(_ether_allegro_line_table)
		free(_ether_allegro_line_table);
	_ether_allegro_line_table = calloc(ether_raster_height_get(_ether_allegro_our_raster), sizeof(unsigned int));
	if(_ether_allegro_line_table == NULL)
		return -1;
	for(i = 0; i < ether_raster_height_get(_ether_allegro_our_raster); ++i)
		_ether_allegro_line_table[i] = ether_raster_rowbytes_get(_ether_allegro_our_raster) * i;
	return 0;
}

static int 
_ether_allegro_compute_slope_table(void)
{
	int i;
	int n = max(ether_raster_height_get(_ether_allegro_our_raster), ether_raster_width_get(_ether_allegro_our_raster));
	if(_ether_allegro_slope_table)
		free(_ether_allegro_slope_table);
	_ether_allegro_slope_table = calloc(n, sizeof(EtherFactor));
	if(_ether_allegro_slope_table == NULL)
		return -1;
	_ether_allegro_slope_table[0] = 0;
	for (i = 1; i < n; ++i)
		_ether_allegro_slope_table[i] = ETHER_FLOAT_TO_FACTOR(1.0 / i);
	return 0;
}

static int 
_ether_allegro_render_lines(EtherOutputvertex *list, int closed)
{
	EtherOutputvertex *v    = list;
	EtherOutputvertex *last = closed ? list : list->prev;
  int x,y;
    
  do {
    x = (int)v->x;
    y = (int)v->y;
    al_draw_line(
        x, 
        y, 
        v->next->x, 
        v->next->y, 
        al_map_rgb(v->red, v->green, v->blue), 
        0);
    v = v->next;
  } while (v != last);

	return 0;
}

static int
_ether_allegro_render_text(int x, int y, char* text, unsigned char color)
{
  al_draw_text(_allegro_font, al_map_rgb(255, 255, 0), x, y, ALLEGRO_ALIGN_LEFT, text);
	return 0;
}

static int 
_ether_allegro_scan_outline(EtherOutputvertex *list, EtherColor color)
{
  typedef struct {
    float x;
    float y;
  } Vertex;

  int count = 0;
  Vertex *vertex;
  EtherOutputvertex *v = list;

  //ALLEGRO_COLOR col = al_map_rgba(255, 100, 100, 255);
  EtherPalette* pal = ether_palette_default_palette_256_get();
  ALLEGRO_COLOR col = al_map_rgba(pal->data[color][0], pal->data[color][1], pal->data[color][2], 255);

  //ALLEGRO_COLOR col = al_map_rgba(
  //      (color >> (8*1)) & 0xff,
  //      (color >> (8*2)) & 0xff,
  //      (color >> (8*3)) & 0xff,
  //      255);

  do {
    count++;
    v = v->next;
  } while (v != list);

  vertex = malloc(count * sizeof(Vertex));
  count = 0;

  do
  {
    vertex[count].x = (float)_ETHER_ALLEGRO_S2I(v->x);
    vertex[count].y = (float)_ETHER_ALLEGRO_S2I(v->y);
    count++;
    v = v->next;
  } while (v != list);

  al_draw_polygon(
          (float *)vertex,
          count,
          ALLEGRO_LINE_JOIN_ROUND,
          col,
          1,
          1);

  return 0;
}
