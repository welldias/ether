#ifndef __ETHER_SYSTEM_H__
#define __ETHER_SYSTEM_H__

#include "ether_defines.h"

int                ether_system_startup         (void);
void               ether_system_run             (void);
EtherRenderStatus *ether_system_render          (EtherObject *list);
EtherTime          ether_system_render_time_get (void);
EtherTime          ether_system_frame_rate_get  (void);
void               ether_system_command_line    (int argc, char *argv[]);
void               ether_system_request_refresh (void);
int                ether_system_query_refresh   (void);
void               ether_system_start_running   (void);
void               ether_system_stop_running    (void);
int                ether_system_is_running      (void);
void               ether_system_blit_func_set   (your_blit_especial_func  *your_blit);

#endif /* __ETHER_SYSTEM_H__ */


