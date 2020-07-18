#include <Ether.h>


#ifdef MS_OS_WINDOWS

#include <sys/timeb.h>
#include <time.h>

typedef struct _timeb SystemTime;

#define _TIMER_SECS(t) (t.time)
#define _TIMER_MILS(t) (t.millitm)

#else
#include <sys/time.h>
#include <unistd.h>

typedef timeval SystemTime;

#define _TIMER_SECS(t) ((size_t)t.tv_sec)
#define _TIMER_MILS(t) ((size_t)t.tv_usec)
#endif // MS_OS_WINDOWS

static int        _ether_timer_notimer = 0;
static EtherTime  _ether_timer_count = 0;  /* notimer */

SystemTime ether_time_start;
SystemTime ether_time_end;


static void _ether_timer_systemtime_get(SystemTime* time)
{
#ifdef MS_OS_WINDOWS
	_ftime64_s(time);
#else
	gettimeofday(ether_time_start, NULL);
#endif
}

void _ether_timer_system_sleep(int milliseconds)
{
#ifdef MS_OS_WINDOWS
	//TODO: fix it
	//Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

int
ether_timer_init(void)
{
	if (_ether_timer_notimer)
		return 0;

	_ether_timer_systemtime_get(&ether_time_start);

	return 0;
}

void
ether_timer_quit(void)
{
	if (_ether_timer_notimer)
		return;

	_ether_timer_systemtime_get(&ether_time_end);
}

EtherTime
ether_timer_read(void)
{
	size_t seconds, useconds;

	if (_ether_timer_notimer)
		return _ether_timer_count++;

	_ether_timer_systemtime_get(&ether_time_end);

	seconds = _TIMER_SECS(ether_time_end) - _TIMER_SECS(ether_time_start);
	useconds = _TIMER_MILS(ether_time_end) - _TIMER_MILS(ether_time_start);

	return (EtherTime)((seconds * 1000 + useconds / 1000.0) + 0.5);
}

EtherTime
ether_timer_tick_rate_get(void)
{
	return (EtherTime)1000;
}

void
ether_timer_delay(EtherTime milliseconds)
{
	SystemTime start, end;
	size_t now, seconds, useconds;

	if (_ether_timer_notimer)
		return;

	_ether_timer_systemtime_get(&start);

	now = 0;
	while (milliseconds > now) {
		_ether_timer_system_sleep(1);
		_ether_timer_systemtime_get(&end);
		seconds = _TIMER_SECS(end) - _TIMER_SECS(start);
		useconds = _TIMER_MILS(end) - _TIMER_MILS(start);
		now = (size_t)((seconds * 1000 + useconds / 1000.0) + 0.5);
	}
}

