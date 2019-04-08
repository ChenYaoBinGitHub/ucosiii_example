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
extern CPU_STK SemMutexM_TASK_STK[SemMutexM_STK_SIZE];	//�����ջ
void SemMutexM_task(void *p_arg);

#define SemMutexL_TASK_PRIO		15						//�������ȼ�
#define SemMutexL_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemMutexLTaskTCB;						//������ƿ�
extern CPU_STK SemMutexL_TASK_STK[SemMutexL_STK_SIZE];	//�����ջ
void SemMutexL_task(void *p_arg);


#define SemTask1_TASK_PRIO		16						//�������ȼ�
#define SemTask1_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemTask1TCB;						//������ƿ�
extern CPU_STK SemTask1_TASK_STK[SemTask1_STK_SIZE];	//�����ջ
void Sem_task1(void *p_arg);

#define SemTask2_TASK_PRIO		17						//�������ȼ�
#define SemTask2_STK_SIZE		128						//�����ջ��С
extern OS_TCB	SemTask2TCB;						//������ƿ�
extern CPU_STK SemTask2_TASK_STK[SemTask2_STK_SIZE];	//�����ջ
void Sem_task2(void *p_arg);

void MtxSem_CreateTask(void);							//��������
void FuncSem_CreateTask(void);


#endif
