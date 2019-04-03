#include "mtx_sem.h"

OS_MUTEX	SEM_MUTEX;		//����һ�������ź���


OS_TCB	SemMutexHTaskTCB;						//������ƿ�
CPU_STK SemMutexH_TASK_STK[SemMutexH_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexH_task(void *p_arg)
{
	OS_ERR err;

	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);   		//��ʱ500ms
	
		printf("high task Pend Mutex\r\n");
		OSMutexPend (&SEM_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
		printf("high task Running!\r\n");
		OSMutexPost(&SEM_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);   		//��ʱ500ms
	}
}


OS_TCB	SemMutexMTaskTCB;						//������ƿ�
CPU_STK SemMutexM_TASK_STK[SemMutexM_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexM_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{
		printf("middle task Running!\r\n");

		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //��ʱ1s
	}
}


OS_TCB	SemMutexLTaskTCB;						//������ƿ�
CPU_STK SemMutexL_TASK_STK[SemMutexL_STK_SIZE];	//�����ջ(8�ֽڶ���)
void SemMutexL_task(void *p_arg)
{
	static u32 times;
	OS_ERR err;
	while(1)
	{
		OSMutexPend (&SEM_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//���󻥳��ź���
		printf("low task Running!\r\n");
		for(times=0;times<100;times++)
		{
			OSSched();											//�����������
		}
		OSMutexPost(&SEM_MUTEX,OS_OPT_POST_NONE,&err);			//�ͷŻ����ź���
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   	//��ʱ1s
	}
}

void MtxSem_CreateTask(void)
{
	OS_ERR err;
	
		OSMutexCreate((OS_MUTEX*	)&SEM_MUTEX,
				  (CPU_CHAR*	)"SEM_MUTEX",
                  (OS_ERR*		)&err);
	//����HIGH����
	OSTaskCreate((OS_TCB 	* )&SemMutexHTaskTCB,		
				 (CPU_CHAR	* )"SemMutexH_task", 		
                 (OS_TASK_PTR )SemMutexH_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SemMutexH_TASK_PRIO,     
                 (CPU_STK   * )&SemMutexH_TASK_STK[0],	
                 (CPU_STK_SIZE)SemMutexH_STK_SIZE/10,	
                 (CPU_STK_SIZE)SemMutexH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
				 
	//����MIDDLE����
	OSTaskCreate((OS_TCB 	* )&SemMutexMTaskTCB,		
				 (CPU_CHAR	* )"SemMutexM_task", 		
                 (OS_TASK_PTR )SemMutexM_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SemMutexM_TASK_PRIO,     
                 (CPU_STK   * )&SemMutexM_TASK_STK[0],	
                 (CPU_STK_SIZE)SemMutexM_STK_SIZE/10,	
                 (CPU_STK_SIZE)SemMutexM_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
				 
	//����LOW����
	OSTaskCreate((OS_TCB 	* )&SemMutexLTaskTCB,		
				 (CPU_CHAR	* )"SemMutexL_task", 		
                 (OS_TASK_PTR )SemMutexL_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SemMutexL_TASK_PRIO,     
                 (CPU_STK   * )&SemMutexL_TASK_STK[0],	
                 (CPU_STK_SIZE)SemMutexL_STK_SIZE/10,	
                 (CPU_STK_SIZE)SemMutexL_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

}







