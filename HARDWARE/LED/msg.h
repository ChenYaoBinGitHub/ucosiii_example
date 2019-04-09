#ifndef __MSG_H
#define __MSG_H

#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"


#define Msg1Send_TASK_PRIO		18						//�������ȼ�
#define Msg1Send_STK_SIZE		128						//�����ջ��С
extern OS_TCB	Msg1SendTaskTCB;						//������ƿ�
extern CPU_STK Msg1Send_TASK_STK[Msg1Send_STK_SIZE];	//�����ջ
void Msg1Send_task(void *p_arg);


#define Msg1Rec_TASK_PRIO		19						//�������ȼ�
#define Msg1Rec_STK_SIZE		128						//�����ջ��С
extern OS_TCB	Msg1RecTaskTCB;						//������ƿ�
extern CPU_STK Msg1Rec_TASK_STK[Msg1Rec_STK_SIZE];	//�����ջ
void Msg1Rec_task(void *p_arg);

#define Msg2_TASK_PRIO		20						//�������ȼ�
#define Msg2_STK_SIZE		128						//�����ջ��С
extern OS_TCB	Msg2TaskTCB;						//������ƿ�
extern CPU_STK Msg2_TASK_STK[Msg2_STK_SIZE];	//�����ջ
void Msg2_task(void *p_arg);

void Msg_CreateTask(void);


#endif
