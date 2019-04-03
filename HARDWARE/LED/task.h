#ifndef __TASK_H
#define __TASK_H


#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str);//�������Ķ�ջʹ�����




#define LED0_TASK_PRIO		4						//�������ȼ�
#define LED0_STK_SIZE 		80						//�����ջ��С	
extern OS_TCB Led0TaskTCB;							//������ƿ�
extern CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//�����ջ	
void led0_task(void *p_arg);						//������


#define FLOAT_TASK_PRIO		5								//�������ȼ�
#define FLOAT_STK_SIZE		128								//�����ջ��С
extern OS_TCB	FloatTaskTCB;								//������ƿ�
extern __align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
void float_task(void *p_arg);								//������

//���������ָ�
#define SuspendResume_TASK_PRIO		7								//�������ȼ�
#define SuspendResume_STK_SIZE		128								//�����ջ��С
extern OS_TCB	SuspendResumeTaskTCB;								//������ƿ�
extern __align(8) CPU_STK	SuspendResume_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SuspendResume_task(void *p_arg);								//������

#define ADC_TASK_PRIO		8						//�������ȼ�
#define ADC_STK_SIZE		128						//�����ջ��С
extern OS_TCB	ADCTaskTCB;							//������ƿ�
extern CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];			//�����ջ(8�ֽڶ���)
void Adc_task(void *p_arg);	

//ʱ��Ƭ��ת��������
#define SchedRound_TASK_PRIO		8						//�������ȼ�
#define SchedRound_STK_SIZE		128							//�����ջ��С
extern OS_TCB	SchedRoundTaskTCB;							//������ƿ�
extern CPU_STK SchedRound_TASK_STK[SchedRound_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SchedRound_task(void *p_arg);	

//�����ʱ��
extern OS_TMR 	tmr1;		//��ʱ��1
extern OS_TMR	tmr2;		//��ʱ��2
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����

//�ź���������Դ
extern u8 share_resource[30];   //������Դ��
extern OS_SEM	MY_SEM_SHARE;	//����һ���ź��������ڷ��ʹ�����Դ

#define SemShare1_TASK_PRIO		9						//�������ȼ�
#define SemShare1_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemShare1TaskTCB;						//������ƿ�
extern CPU_STK SemShare1_TASK_STK[SemShare1_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemShare1_task(void *p_arg);	

#define SemShare2_TASK_PRIO		10						//�������ȼ�
#define SemShare2_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemShare2TaskTCB;						//������ƿ�
extern CPU_STK SemShare2_TASK_STK[SemShare2_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemShare2_task(void *p_arg);

//�ź��������ͬ��
extern OS_SEM	MY_SEM_SYNC;	//����һ���ź�������������ͬ��

#define SemSync1_TASK_PRIO		11						//�������ȼ�
#define SemSync1_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemSync1TaskTCB;						//������ƿ�
extern CPU_STK SemSync1_TASK_STK[SemSync1_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemSync1_task(void *p_arg);	

#define SemSync2_TASK_PRIO		12						//�������ȼ�
#define SemSync2_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemSync2TaskTCB;						//������ƿ�
extern CPU_STK SemSync2_TASK_STK[SemSync2_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemSync2_task(void *p_arg);








#endif
