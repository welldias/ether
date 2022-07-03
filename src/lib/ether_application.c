#include "ether_application.h"
#include "ether_object.h"
#include "ether_camera.h"
#include "ether_world.h"

#define _ETHER_APP_BLACKCOLOR 0x00000000L
#define _ETHER_APP_WHITECOLOR 0xFFFFFFFFL

static int _ether_application_object_move_locally(EtherObject *obj);

void ether_application_init(void) {
	EtherObject *torso;
	EtherObject *head;

	/* find the head (the object the camera is associated with) and
     * set it up to track the head device 
	 */
	head = ether_camera_obj_get(ether_world_camera_get());
	ether_object_function_set(head, _ether_application_object_move_locally);
	ether_object_application_data_set(head, NULL);

	/* if the application hasn't already attached the head to a torso,
	 * then make a copy of the head to form the torso and attach the head
	 * to it; make sure that the torso thus created has no shape (we don't
	 * want a head on top of a another head to be the default)
	 */
	torso = ether_object_find_root(head);
	/* no torso yet */
	if(torso == head) {
		torso = ether_object_copy(head);      /* create one */
		ether_object_attach(head, torso);     /* and attach the head to it */
		ether_object_shape_set(torso, NULL);  /* doesn't look like the head */
	}
}

void ether_application_draw_under(void) {
}

void ether_application_draw_over(EtherRenderStatus *stat) {
#if 0
	int wleft, wtop, wright, wbottom; /* window coordinates */
	int wwidth, wheight;              /* window dimensions */
	char buff[100];

	Ether_Camera *cam = ether_world_camera_get();

	ether_display_window_get(&wleft, &wtop, &wright, &wbottom);
	wwidth = wright - wleft + 1;
	wheight = wbottom - wtop + 1;

	if(ether_config_position_display_get()) {
		sprintf(buff, "Position: %ld,%ld",
			(long)floor(ETHER_SCALAR_TO_FLOAT(ether_camera_world_x_get(cam))),
			(long)floor(ETHER_SCALAR_TO_FLOAT(ether_camera_world_z_get(cam))));
		if(ether_display_text_width_get(buff) + 10 < wwidth-1)
			ether_ui_drop_text(10, 10, _ETHER_APP_WHITECOLOR, buff);
	}

	if(ether_config_frame_rate_display_get()) {
		sprintf(buff, "Frames/sec: %ld", ether_system_frame_rate_get());
		if(ether_display_text_width_get(buff) + 5 < wwidth-1)
			ether_ui_drop_text(5, wheight - ether_display_text_height_get(buff) - 10, _ETHER_APP_WHITECOLOR, buff);
	}
	
	if(ether_config_compass_display_get())
		ether_ui_draw_compass(cam, wwidth - 70, 40, 35);

	if(showhud) {
		sprintf(buff, "%c%c%c",
			stat->memory ?  'M' : ' ',
			stat->objects ? 'O' : ' ',
			stat->facets ?  'F' : ' ');
		if(ether_display_text_width_get(buff) + 10 < wwidth-1)
			ether_ui_drop_text(10, 20, _ETHER_APP_WHITECOLOR, buff);
	}

	if(ether_mouse_usage_get()) 	{
		Ether_Mouse *mouse = ether_mouse_pointer();
		if(mouse) {
			int x = ether_mouse_center_get(mouse, X);
			int y = ether_mouse_center_get(mouse, Y);
			int deadx = ether_mouse_dead_zone(dev, X);
			int deady = ether_mouse_dead_zone(dev, Y);

			/* white inner box */
			ether_display_line(x - deadx, y - deady, x + deadx, y - deady, _ETHER_APP_WHITECOLOR);
			ether_display_line(x - deadx, y + deady, x + deadx, y + deady, _ETHER_APP_WHITECOLOR);
			ether_display_line(x - deadx, y - deady, x - deadx, y + deady, _ETHER_APP_WHITECOLOR);
			ether_display_line(x + deadx, y - deady, x + deadx, y + deady, _ETHER_APP_WHITECOLOR);

			/* black outer box */
			ether_display_line(x-deadx-1, y-deady-1, x+deadx+1, y-deady-1, _ETHER_APP_BLACKCOLOR);
			ether_display_line(x-deadx-1, y+deady+1, x+deadx+1, y+deady+1, _ETHER_APP_BLACKCOLOR);
			ether_display_line(x-deadx-1, y-deady-1, x-deadx-1, y+deady+1, _ETHER_APP_BLACKCOLOR);
			ether_display_line(x+deadx+1, y-deady-1, x+deadx+1, y+deady+1, _ETHER_APP_BLACKCOLOR);
		}
	}
#endif
}

void ether_application_key(unsigned int key) {
}

void ether_application_mouse_up(int x, int y, unsigned int buttons) {
}

static int _ether_application_object_move_locally(EtherObject *obj) {
	return -1;
}
