#include <Ether.h>

typedef struct _Ether_Task EtherTask;

struct _Ether_Task
{
	void (*function)(void);
	void *data;
	EtherTime period;
	EtherTime lastran;
	EtherTask *next;
};

static void *_ether_task_current_task_data         = NULL;
static EtherTime _ether_task_current_task_elapsed = 0;
static EtherTime _ether_task_time_now             = 0;
static EtherTask *_ether_task_list                = NULL;

int
ether_task_create(void (*function)(void), void *data, EtherTime period)
{
	EtherTask *task = malloc(sizeof(EtherTask));
	if(task == NULL)
		return -1;
	task->period     = period;
	task->lastran    = ether_timer_read();
	task->function   = function;
	task->data       = data;
	task->next       = _ether_task_list;
	_ether_task_list = task;
	return 0;
}

void
ether_task_run(void)
{
	EtherTask *task;
	static EtherTime lastran = 0L;
	_ether_task_time_now = ether_timer_read();
	if(_ether_task_time_now < lastran)
	{
		lastran = _ether_task_time_now;
		return;
	}
	for(task = _ether_task_list; task; task = task->next)
	{
		_ether_task_current_task_elapsed = _ether_task_time_now - task->lastran;
		_ether_task_current_task_data = task->data;
		if(task->period == 0 || _ether_task_current_task_elapsed > task->period)
			task->function();
		task->lastran = _ether_task_time_now;
	}
}

void
*ether_task_data_get(void)
{
	if(_ether_task_current_task_data)
		return _ether_task_current_task_data;
	else
		return NULL;
}

EtherTime
ether_task_elapsed_get(void)
{
	return _ether_task_current_task_elapsed;
}

EtherTime
ether_task_time_now_get(void)
{
	return _ether_task_time_now;
}
