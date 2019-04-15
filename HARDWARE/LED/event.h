#ifndef __EVENT_H
#define __EVENT_H

#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#define EVENT0_FLAG		0X00
#define EVENT1_FLAG		0x01
#define EVENT2_FLAG		0x04


#define EVENT_PEND_TASK_PRIO		21						//任务优先级
#define EVENT_PEND_STK_SIZE		128							//任务堆栈大小
extern OS_TCB	EVENT_PEND_TaskTCB;							//任务控制块
extern CPU_STK EVENT_PEND_TASK_STK[EVENT_PEND_STK_SIZE];	//任务堆栈
void EventPend_task(void *p_arg);


#define EVENT_POST_TASK_PRIO		22						//任务优先级
#define EVENT_POST_STK_SIZE		128							//任务堆栈大小
extern OS_TCB	EVENT_POST_TaskTCB;							//任务控制块
extern CPU_STK EVENT_POST_TASK_STK[EVENT_POST_STK_SIZE];	//任务堆栈
void EventPost_task(void *p_arg);

void Event_CreatTask(void);


#endif

