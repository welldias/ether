#ifndef __ETHER_DISPLAY_H__
#define __ETHER_DISPLAY_H__

/*
 * The display subsystem is the "back end" of the rendering pipeline; 
 * it's responsible for actually drawing polygons (and lines, and text) 
 * into a raster. 
 *
 * Internally, the display driver is just a function (much like the 
 * video driver, or any of the input device drivers); 
 * The application-visible functions that access that driver are described here.
 */

typedef enum _Ether_Display_Command
{
	ETHER_DISPLAY_GET_VERSION = 0, 
	ETHER_DISPLAY_GET_DESCRIPTION,
	ETHER_DISPLAY_INIT, 
	ETHER_DISPLAY_QUIT,
	ETHER_DISPLAY_CLEAR, 
	ETHER_DISPLAY_POINT, 
	ETHER_DISPLAY_LINE,
	ETHER_DISPLAY_CLOSED_LINE, 
	ETHER_DISPLAY_POLY, 
	ETHER_DISPLAY_BOX,
	ETHER_DISPLAY_TEXT, 
	ETHER_DISPLAY_TEXT_POSITION,
	ETHER_DISPLAY_GET_TEXTWIDTH, 
	ETHER_DISPLAY_GET_TEXTHEIGHT,
	ETHER_DISPLAY_GET_SORTORDER, 
	ETHER_DISPLAY_CAN_GOURAUD,
	ETHER_DISPLAY_UPDATE_PALETTE, 
	ETHER_DISPLAY_CAN_XY_CLIP,
	ETHER_DISPLAY_BEGIN_FRAME, 
	ETHER_DISPLAY_END_FRAME,
	ETHER_DISPLAY_SET_RASTER, 
	ETHER_DISPLAY_GET_RASTER,
	ETHER_DISPLAY_SET_Z_BUFFER, 
	ETHER_DISPLAY_GET_Z_BUFFER,
	ETHER_DISPLAY_USE_Z_BUFFER, 
	ETHER_DISPLAY_CLEAR_Z_BUFFER,
	ETHER_DISPLAY_SET_SHADING,
} EtherDisplayCommand;

typedef enum _Ether_Video_Command
{
	ETHER_VIDEO_GET_VERSION = 0,
	ETHER_VIDEO_GET_DESCRIPTION,
	ETHER_VIDEO_SETUP, 
	ETHER_VIDEO_SHUTDOWN, 
	ETHER_VIDEO_GET_ERROR,
	ETHER_VIDEO_GET_MODE,
	ETHER_VIDEO_SET_DRAW_PAGE, 
	ETHER_VIDEO_SET_VIEW_PAGE, 
	ETHER_VIDEO_GET_NPAGES,
	ETHER_VIDEO_HAS_PALETTE, 
	ETHER_VIDEO_SET_PALETTE,
	ETHER_VIDEO_SET_NTSC, 
	ETHER_VIDEO_CHECK_RETRACE,
	ETHER_VIDEO_GET_RASTER, 
	ETHER_VIDEO_BLIT,
	ETHER_VIDEO_CURSOR_HIDE, 
	ETHER_VIDEO_CURSOR_SHOW, 
	ETHER_VIDEO_CURSOR_RESET,
	ETHER_VIDEO_CURSOR_MOVE, 
	ETHER_VIDEO_GET_STATUS,
	ETHER_VIDEO_CURSOR_SET_APPEARANCE
} EtherVideoCommand;

typedef enum _Ether_Display_Status
{
	ETHER_DISPLAY_STATUS_UNKNOW = 0,
	ETHER_DISPLAY_CONFIGURATED,
	ETHER_DISPLAY_INITIATED,
	ETHER_DISPLAY_STOPPED,
	ETHER_DISPLAY_RUNNING
} EtherDisplayStatus;

typedef int EtherDisplayDriverFunction(EtherDisplayCommand cmd, int lparm, void *pparm1);
typedef int EtherVideoDriverFunction  (EtherVideoCommand cmd, int lparm, void *pparm1);

extern EtherDisplayDriverFunction  *_ether_display_current_driver;
extern EtherVideoDriverFunction    *_ether_video_current_driver;

struct _Ether_Outputvertex
{
	long int x,y,z;           /**< X, Y screen coordinates and Z-depth */
	EtherColor red;           /**< components of the color */
	EtherColor green;		  
	EtherColor blue; 		  
	EtherOutputvertex *next;  /**< doubly-linked circular list */
	EtherOutputvertex *prev;  

	/* don't rely on anything below here staying the same */
	int u, v;                 /**< texture map coordinates */
	EtherFactor intensity;    /**< intensity of light at this vertex */
	int  outcode;             /**< used for XY clipping */
};

struct _Ether_Outputfacet
{
	EtherOutputvertex *points; /**< linked list of vertices for this facet */
	EtherSurface *surface;     /**< surface properties */
	EtherColor color;          /**< color of this facet (flat shading only) */
	
	/* don't rely on anything below here staying the same */
	EtherFactor intensity;     /**< amount of light falling on this facet */
	int xclip;                 /**< this facet needs X clipping */
	int yclip;                 /**< this facet needs Y clipping */
	int highlight : 1;         /**< this facet is highlighted */
	EtherFacet *original;      /**< points back at the facet we came from */
	void *outobj;              /**< points back at the outobject we belong to */
	long int minbound[3];      /**< bounding box for smarter sorting */
	long int maxbound[3];  
	EtherOutputfacet *details; /**< linked list of detail facets */
	EtherOutputfacet *next;    /**< only used to link detail facets */
};


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

EAPI EtherDisplayStatus ether_display_status_get      ();

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
EAPI int           ether_video_error_get              (const char** description);

/* XY clipping functions */
EAPI void               ether_display_xyclip_set      (int left, int top, int right, int bottom);
EAPI EtherOutputvertex *ether_display_xyclip_point    (EtherOutputvertex *vertices, unsigned int flags);
EAPI EtherOutputvertex *ether_display_xyclip_line     (EtherOutputvertex *vertices, unsigned int flags);
EAPI EtherOutputvertex *ether_display_xyclip_poly     (EtherOutputvertex *vertices, unsigned int flags);

#endif /* __ETHER_DISPLAY_H__ */
