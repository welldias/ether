#ifndef __ETHER_APPLICATION_H__
#define __ETHER_APPLICATION_H__

EAPI void          ether_application_init                (void);
EAPI void          ether_application_draw_under          (void);
EAPI void          ether_application_draw_over           (EtherRenderStatus *stat);
EAPI void          ether_application_key                 (unsigned int key);
EAPI void          ether_application_mouse_up            (int x, int y, unsigned int buttons);

#endif /* __ETHER_APPLICATION_H__ */
