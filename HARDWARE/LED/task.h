#ifndef __TASK_H
#define __TASK_H


#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#define LED0_TASK_PRIO		4				//任务优先级
#define LED0_STK_SIZE 		80				//任务堆栈大小	
extern OS_TCB Led0TaskTCB;							//任务控制块
extern CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//任务堆栈	
void led0_task(void *p_arg);				//任务函数


#define FLOAT_TASK_PRIO		5						//任务优先级
#define FLOAT_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	FloatTaskTCB;								//任务控制块
extern __align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//任务堆栈(8字节对齐)
void float_task(void *p_arg);						//任务函数

#define SuspendResume_TASK_PRIO		7						//任务优先级
#define SuspendResume_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	SuspendResumeTaskTCB;								//任务控制块
extern __align(8) CPU_STK	SuspendResume_TASK_STK[FLOAT_STK_SIZE];	//任务堆栈(8字节对齐)
void SuspendResume_task(void *p_arg);						//任务函数

#define ADC_TASK_PRIO		8						//任务优先级
#define ADC_STK_SIZE		128						//任务堆栈大小
extern OS_TCB	ADCTaskTCB;								//任务控制块
extern CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];	//任务堆栈(8字节对齐)
void Adc_task(void *p_arg);	

void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str);//检测任务的堆栈使用情况




#endif
