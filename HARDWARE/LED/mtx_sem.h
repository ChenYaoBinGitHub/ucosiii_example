#ifndef __MTX_SEM_H
#define __MTX_SEM_H

#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

extern OS_MUTEX	SEM_MUTEX;		//定义一个互斥信号量

#define SemMutexH_TASK_PRIO		13						//任务优先级
#define SemMutexH_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	SemMutexHTaskTCB;						//任务控制块
extern CPU_STK SemMutexH_TASK_STK[SemMutexH_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexH_task(void *p_arg);	

#define SemMutexM_TASK_PRIO		14						//任务优先级
#define SemMutexM_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	SemMutexMTaskTCB;						//任务控制块
extern CPU_STK SemMutexM_TASK_STK[SemMutexM_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexM_task(void *p_arg);

#define SemMutexL_TASK_PRIO		15						//任务优先级
#define SemMutexL_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	SemMutexLTaskTCB;						//任务控制块
extern CPU_STK SemMutexL_TASK_STK[SemMutexL_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexL_task(void *p_arg);

void MtxSem_CreateTask(void);							//创建任务



#endif
