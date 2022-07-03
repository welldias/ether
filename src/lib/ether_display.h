#ifndef __ETHER_DISPLAY_H__
#define __ETHER_DISPLAY_H__

#include "ether_defines.h"

/* Display functions */
EAPI void          ether_display_driver_set           (EtherDisplayDriverFunction *driver);
EAPI int           ether_display_init                 (EtherRaster *raster);
EAPI void          ether_display_quit                 (void);
EAPI void          ether_display_clear                (EtherColor color);
EAPI void          ether_display_point                (int x, int y, EtherColor color);
EAPI void          ether_display_line                 (int x1, int y1, int x2, int y2, EtherColor color);
EAPI void          ether_display_box                  (int x1, int y1, int x2, int y2, EtherColor color);
EAPI void          ether_display_text                 (int x, int y, EtherColor color, char *message);
EAPI int           ether_display_text_width_get       (char *string);
EAPI int           ether_display_text_height_get      (char *string);
EAPI int           ether_display_sort_order_get       (void);
EAPI int           ether_display_can_gouraud          (void);
EAPI int           ether_display_can_xyclip           (void);
EAPI int           ether_display_version_get          (void);
EAPI char         *ether_display_description_get      (void);
EAPI int           ether_display_begin_frame          (void);
EAPI int           ether_display_end_frame            (void);
EAPI int           ether_display_raster_set           (EtherRaster *raster);
EAPI EtherRaster *ether_display_raster_get            (void);
EAPI int           ether_display_zbuffer_set          (EtherRaster *raster);
EAPI int           ether_display_zbuffer_get          (void);
EAPI int           ether_display_clear_zbuffer        (int depth);
EAPI int           ether_display_use_zbuffer          (int flag);
EAPI int           ether_display_update_palette       (EtherPalette *palette);
EAPI int           ether_display_shading_set          (int value);
EAPI EtherColor    ether_display_compute_color        (EtherSurface *surf, EtherFactor intensity, EtherFactor ambient, EtherScalar depth);
EAPI void          ether_display_compute_vertex_color (EtherOutputvertex *v, EtherSurface *surf, EtherFactor intensity, EtherFactor ambient, EtherScalar depth);
EAPI void          ether_display_window_set           (int x1, int y1, int x2, int y2);
EAPI void          ether_display_window_get           (int *x1, int *y1, int *x2, int *y2);
EAPI int           ether_display_width_get            (void);
EAPI int           ether_display_height_get           (void);
EAPI int           ether_display_depth_get            (void);
EAPI int           ether_display_update               (void);
EAPI int           ether_display_send_command         (EtherDisplayCommand cmd, int lparm, void *pparm1);

/* Video functions */
EAPI void          ether_video_driver_set             (EtherVideoDriverFunction *driver);
EAPI int           ether_video_mode_get               (void);
EAPI void          ether_video_draw_page_set          (int page);
EAPI int           ether_video_draw_page_get          (void);
EAPI void          ether_video_view_page_set          (int page);
EAPI int           ether_video_view_page_get          (void);
EAPI int           ether_video_npages_get             (void);
EAPI int           ether_video_has_palette            (void);
EAPI void          ether_video_palette_set            (int start, int end, EtherPalette *palette);  /* updates the palette */
EAPI void          ether_video_palette_get            (int start, int end, EtherPalette *palette);
EAPI int           ether_video_ntsc_set               (int flag);
EAPI int           ether_video_check_retrace          (void);
EAPI EtherRaster  *ether_video_raster_get             (void);
EAPI int           ether_video_blit                   (EtherRaster *buffer);
EAPI int           ether_video_version_get            (void);
EAPI char         *ether_video_description_get        (void);
EAPI int           ether_video_cursor_hide            (void);
EAPI int           ether_video_cursor_show            (void);
EAPI int           ether_video_cursor_reset           (void);
EAPI int           ether_video_cursor_move            (int x, int y);

/* XY clipping functions */
EAPI void               ether_display_xyclip_set      (int left, int top, int right, int bottom);
EAPI EtherOutputvertex *ether_display_xyclip_point    (EtherOutputvertex *vertices, unsigned int flags);
EAPI EtherOutputvertex *ether_display_xyclip_line     (EtherOutputvertex *vertices, unsigned int flags);
EAPI EtherOutputvertex *ether_display_xyclip_poly     (EtherOutputvertex *vertices, unsigned int flags);

#endif /* __ETHER_DISPLAY_H__ */
