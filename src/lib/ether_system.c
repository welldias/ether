#include <allegro5/allegro.h>
#include <Ether.h>
#include "ether_private.h"

static int        _ether_system_running           = 0; /* non-zero while system is initialized and running */
static int        _ether_system_need_to_redraw    = 0; /* non-zero if the screen needs updating */
static EtherTime _ether_system_last_render_ticks;
static your_blit_especial_func  *_your_blit = NULL;

void ether_system_blit_func_set(your_blit_especial_func  *your_blit)
{
  _your_blit = your_blit;
}

int
ether_system_startup(void)
{
	ether_math_init();
	ether_world_init(ether_world_current_get());
	
	/*
	if(ether_video_setup(0))
	{
		fprintf(stderr, "Could not enter graphics mode!\n");
		return -1;
	}
	atexit(ether_video_shutdown);
	*/

	if(ether_display_init(NULL))
		return -1;
	atexit(ether_display_quit);
	/* ether_mouse_init(); */
	/* atexit(ether_mouse_quit); */
	if(ether_timer_init())
		return -2;
	atexit(ether_timer_quit);
	if(ether_render_init(800, 800, 500, 5, 65000))
		return -3;
	atexit(ether_render_quit);
	/* make sure that exit() [and therefore the atexit() functions] get
	   called if there are any fatal errors */
#ifndef _WIN32_WCE
	signal(SIGABRT, exit);
	signal(SIGFPE,  exit);
	signal(SIGILL,  exit);
	signal(SIGINT,  exit);
	signal(SIGSEGV, exit);
	signal(SIGTERM, exit);
#endif

	ether_system_start_running();
	ether_system_request_refresh();
	ether_system_render(NULL);
	return 0;	
}

void
ether_system_run(void)
{
  ALLEGRO_KEYBOARD_STATE key_state;
	EtherObject *list;

	ether_application_init();
	ether_system_request_refresh();
	while(ether_system_is_running())
  {
    /*
    if(ether_keyboard_check())
    ether_application_key(ether_keyboard_read());
    check_mouse();
    */
    ether_task_run();
    /* ether_device_pollall(); */

    list = ether_world_update();
    if(ether_system_query_refresh())
      ether_system_render(list);
    al_rest(0.001);

    al_get_keyboard_state(&key_state);
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
      ether_system_stop_running();
      break;
    }
  }	
}

EtherRenderStatus *
ether_system_render(EtherObject *list)
{
	static EtherObject *lastlist = NULL;
	EtherPalette *pal;
	EtherRenderStatus *stat;
	int pagenum;
	EtherTime render_start = ether_timer_read();
	if(list == NULL)
		list = lastlist;
	else
		lastlist = list;
	pal = ether_world_palette_get();
	if(ether_palette_has_changed(pal))
	{
		ether_video_palette_set(0, 255, pal);
		ether_palette_changed_set(pal, 0);
	}
	pagenum = ether_video_draw_page_get();
	if(++pagenum >= ether_video_npages_get())
		pagenum = 0;
	ether_video_draw_page_set(pagenum);
	ether_render_ambient_set(ether_world_ambient_get());
	if(ether_world_screenclear_get())
	{
		ether_display_begin_frame();
		if(ether_world_horizon_get() && !ether_render_draw_mode_get())
			ether_render_horizon();
		else
			ether_display_clear(ether_world_sky_color_get());
		ether_display_end_frame();
	}
	//ether_application_draw_under();

	ether_render_horizontal_shift_set(0);
	ether_display_begin_frame();
	ether_render_begin(ether_world_camera_get(), ether_world_lights_get());
	stat = ether_render_objlist(list);
	ether_display_end_frame();
	ether_render_horizontal_shift_set(0);
	ether_application_draw_over(stat);
	ether_video_cursor_hide();
  if(_your_blit)
    _your_blit(ether_display_raster_get());
  else
	  ether_display_update();
	ether_video_view_page_set(pagenum);
	ether_video_cursor_show();
	_ether_system_last_render_ticks = ether_timer_read() - render_start;
	_ether_system_need_to_redraw = 0;
	return stat;	
}

EtherTime
ether_system_render_time_get(void)
{
	return _ether_system_last_render_ticks;
}

EtherTime
ether_system_frame_rate_get(void)
{
	if(_ether_system_last_render_ticks == 0) 
		_ether_system_last_render_ticks = 1;
	return ether_timer_tick_rate_get() / _ether_system_last_render_ticks;
}

void
ether_system_command_line(int argc, char *argv[])
{
	/*
	int i;
	Ether_Camera *cam;
	for(i = 1; i < argc; ++i)
	{
		FILE *in = fopen(argv[i], "r");
		if (in == NULL) continue;
		if (strstr(argv[i], ".wld"))
			ether_wld_read(in);
		else if (strstr(argv[i], ".fig"))
			ether_fig_read(in, NULL, NULL);
		else if (strstr(argv[i], ".plg"))
			ether_objectplg_read(in);
		fclose(in);
	}
	if(!ether_world_camera_get())
		ether_camera_create();
	ether_world_update();
	*/
}

void
ether_system_request_refresh(void)
{
	_ether_system_need_to_redraw = 1;
}

int
ether_system_query_refresh(void)
{
	return _ether_system_need_to_redraw;
}

void
ether_system_start_running(void)
{
	_ether_system_running = 1;
}

void
ether_system_stop_running(void)
{
	_ether_system_running = 0;
}

int
ether_system_is_running(void)
{
	return _ether_system_running;
}
