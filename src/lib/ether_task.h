#ifndef __ETHER_TASK_H__
#define __ETHER_TASK_H__


/* Pseudo-tasking routines */

int         ether_task_create       (void (*function)(void), void *data, EtherTime period);
void        ether_task_run          (void); 
void       *ether_task_data_get     (void); /* Lets the current task get a pointer to its data */ 
EtherTime   ether_task_elapsed_get  (void); /* Lets the current task get the elapsed time since it last ran */
EtherTime   ether_task_time_now_get (void); /* Lets the current task get the current time (same time for all tasks) */


#endif /* __ETHER_TASK_H__ */
