#ifndef __ETHER_TIMER_H__
#define __ETHER_TIMER_H__

#include "ether_defines.h"

/*
 * Ether has a set of routines which deal with the timer; these 
 * will vary from one platform to another, but they should all 
 * provide the same high-level interface to application software.
 *
 */

EAPI int          ether_timer_init             (void);
EAPI void         ether_timer_quit             (void);
EAPI EtherTime    ether_timer_read             (void);
EAPI EtherTime    ether_timer_tick_rate_get    (void);
EAPI void         ether_timer_delay            (EtherTime milliseconds);

#endif /* __ETHER_TIMER_H__ */
