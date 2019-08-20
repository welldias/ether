#ifndef __ETHER_RENDER_H__
#define __ETHER_RENDER_H__

typedef struct _Ether_Render_Status EtherRenderStatus;

struct _Ether_Render_Status
{
	int objects : 1;     /**< set if there were too many objects */
	int facets  : 1;     /**< set if there were too many facets */
};

/* Initialize the rendering engine. Parameters are:
 * - maxf       : maximum number of renderable facets per frame
 * - maxvert    : maximum number of vertices per object (after Z-clipping)
 * - maxobjs    : maximum number of objects for sorting
 * - maxlgts    : maximum number of lights per scene 
 * - mempoolsize: amount of memory to allocate for per-frame data
 */
int                     ether_render_init                   (int maxvert, int maxf, int maxobjs, int maxlights, unsigned int mempoolsize);
void                    ether_render_quit                   (void);
void                    ether_render_begin                  (EtherCamera *camera, EtherLight *lights);
void                    ether_render_ambient_set            (EtherFactor amb);
void                    ether_render_horizon                (void);
EtherRenderStatus    *ether_render_objlist                (EtherObject *objects);
void                    ether_render_horizontal_shift_set   (int n);
void                    ether_render_draw_mode_set          (int mode);
int                     ether_render_draw_mode_get          (void);
void                    ether_render_wireframe_color_set    (EtherColor color);
EtherColor             ether_render_wireframe_color_get    (void);
void                    ether_render_highlight_color_set    (EtherColor color);
EtherColor             ether_render_highlight_color_get    (void);
void                    ether_render_monitor_init           (int x, int y);
int                     ether_render_monitor_read           (EtherObject **obj, EtherFacet **facet, int *vertnum);
void                    ether_transform_vertex_to_screen    (long int *x, long int *y, EtherObject *obj, EtherVector vertex);

#endif /* __ETHER_RENDER_H__ */
