#ifndef __TASK_H
#define __TASK_H


#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#define LED0_TASK_PRIO		4				//�������ȼ�
#define LED0_STK_SIZE 		80				//�����ջ��С	
extern OS_TCB Led0TaskTCB;							//������ƿ�
extern CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//�����ջ	
void led0_task(void *p_arg);				//������


#define FLOAT_TASK_PRIO		5						//�������ȼ�
#define FLOAT_STK_SIZE		128						//�����ջ��С
extern OS_TCB	FloatTaskTCB;								//������ƿ�
extern __align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
void float_task(void *p_arg);						//������

#define SuspendResume_TASK_PRIO		7						//�������ȼ�
#define SuspendResume_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SuspendResumeTaskTCB;								//������ƿ�
extern __align(8) CPU_STK	SuspendResume_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SuspendResume_task(void *p_arg);						//������

#define ADC_TASK_PRIO		8						//�������ȼ�
#define ADC_STK_SIZE		128						//�����ջ��С
extern OS_TCB	ADCTaskTCB;								//������ƿ�
extern CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];	//�����ջ(8�ֽڶ���)
void Adc_task(void *p_arg);	

void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str);//�������Ķ�ջʹ�����




#endif
