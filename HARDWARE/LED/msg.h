#ifndef __MSG_H
#define __MSG_H

#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"


#define Msg1Send_TASK_PRIO		18						//任务优先级
#define Msg1Send_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	Msg1SendTaskTCB;						//任务控制块
extern CPU_STK Msg1Send_TASK_STK[Msg1Send_STK_SIZE];	//任务堆栈
void Msg1Send_task(void *p_arg);


#define Msg1Rec_TASK_PRIO		19						//任务优先级
#define Msg1Rec_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	Msg1RecTaskTCB;						//任务控制块
extern CPU_STK Msg1Rec_TASK_STK[Msg1Rec_STK_SIZE];	//任务堆栈
void Msg1Rec_task(void *p_arg);

#define Msg2_TASK_PRIO		20						//任务优先级
#define Msg2_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	Msg2TaskTCB;						//任务控制块
extern CPU_STK Msg2_TASK_STK[Msg2_STK_SIZE];	//任务堆栈
void Msg2_task(void *p_arg);

void Msg_CreateTask(void);


#endif
