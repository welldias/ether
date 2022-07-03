#include <sys/time.h>
#include <unistd.h>

#include "ether_timer.h"

static int        _ether_timer_notimer = 0;
static EtherTime  _ether_timer_count   = 0;  /* notimer */

struct timeval ether_time_start;
struct timeval ether_time_end;

int ether_timer_init(void)
{
	if(_ether_timer_notimer)
		return 0;
   
  gettimeofday(&ether_time_start, NULL);

	return 0;
}

void ether_timer_quit(void)
{
	if(_ether_timer_notimer)
		return;

  gettimeofday(&ether_time_end, NULL);
}

EtherTime ether_timer_read(void)
{
  long seconds, useconds;    

	if(_ether_timer_notimer)
		return _ether_timer_count++;

  gettimeofday(&ether_time_end, NULL);
  seconds  = ether_time_end.tv_sec  - ether_time_start.tv_sec;
  useconds = ether_time_end.tv_usec - ether_time_start.tv_usec;

	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}

EtherTime ether_timer_tick_rate_get(void)
{
	return (EtherTime)1000;
}

void ether_timer_delay(EtherTime milliseconds)
{
  struct timeval start, end;
  long now, seconds, useconds;    

	if(_ether_timer_notimer)
		return;

  gettimeofday(&start, NULL);

  now = 0;
  while(milliseconds > now) {
    usleep(1000);
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    now = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  }
}
