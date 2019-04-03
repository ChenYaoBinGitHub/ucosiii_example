#ifndef __MTX_SEM_H
#define __MTX_SEM_H

#include "stm32f10x.h"
#include "includes.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

extern OS_MUTEX	SEM_MUTEX;		//����һ�������ź���

#define SemMutexH_TASK_PRIO		13						//�������ȼ�
#define SemMutexH_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemMutexHTaskTCB;						//������ƿ�
extern CPU_STK SemMutexH_TASK_STK[SemMutexH_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexH_task(void *p_arg);	

#define SemMutexM_TASK_PRIO		14						//�������ȼ�
#define SemMutexM_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemMutexMTaskTCB;						//������ƿ�
extern CPU_STK SemMutexM_TASK_STK[SemMutexM_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexM_task(void *p_arg);

#define SemMutexL_TASK_PRIO		15						//�������ȼ�
#define SemMutexL_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemMutexLTaskTCB;						//������ƿ�
extern CPU_STK SemMutexL_TASK_STK[SemMutexL_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexL_task(void *p_arg);

void MtxSem_CreateTask(void);							//��������



#endif
