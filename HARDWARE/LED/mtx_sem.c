#include "mtx_sem.h"

OS_MUTEX	SEM_MUTEX;		//定义一个互斥信号量


OS_TCB	SemMutexHTaskTCB;						//任务控制块
CPU_STK SemMutexH_TASK_STK[SemMutexH_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexH_task(void *p_arg)
{
	OS_ERR err;

	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);   		//延时500ms
	
		printf("high task Pend Mutex\r\n");
		OSMutexPend (&SEM_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//请求互斥信号量
		printf("high task Running!\r\n");
		OSMutexPost(&SEM_MUTEX,OS_OPT_POST_NONE,&err);				//释放互斥信号量
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);   		//延时500ms
	}
}


OS_TCB	SemMutexMTaskTCB;						//任务控制块
CPU_STK SemMutexM_TASK_STK[SemMutexM_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexM_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{
		printf("middle task Running!\r\n");

		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //延时1s
	}
}


OS_TCB	SemMutexLTaskTCB;						//任务控制块
CPU_STK SemMutexL_TASK_STK[SemMutexL_STK_SIZE];	//任务堆栈(8字节对齐)
void SemMutexL_task(void *p_arg)
{
	static u32 times;
	OS_ERR err;
	while(1)
	{
		OSMutexPend (&SEM_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求互斥信号量
		printf("low task Running!\r\n");
		for(times=0;times<100;times++)
		{
			OSSched();											//发起任务调度
		}
		OSMutexPost(&SEM_MUTEX,OS_OPT_POST_NONE,&err);			//释放互斥信号量
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   	//延时1s
	}
}

void MtxSem_CreateTask(void)
{
	OS_ERR err;
	
		OSMutexCreate((OS_MUTEX*	)&SEM_MUTEX,
				  (CPU_CHAR*	)"SEM_MUTEX",
                  (OS_ERR*		)&err);
	//创建HIGH任务
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
				 
	//创建MIDDLE任务
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
				 
	//创建LOW任务
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


//////////////////////////////////////////////////////////////////
//内嵌信号量

OS_TCB	SemTask1TCB;						//任务控制块
CPU_STK SemTask1_TASK_STK[SemTask1_STK_SIZE];	//任务堆栈
void Sem_task1(void *p_arg)
{
	OS_ERR err;
	static u8 num = 0;
	
	while(1)
	{
		if(num++ == 10)
		{
			
			OSTaskSemPost(&SemTask2TCB,OS_OPT_POST_NONE,&err);	//使用系统内建信号量向任务task2发送信号量
			printf("task sem Post task2\r\n");
			num = 0;
		}
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);   		//延时100ms	
	}

}



OS_TCB	SemTask2TCB;						//任务控制块
CPU_STK SemTask2_TASK_STK[SemTask2_STK_SIZE];	//任务堆栈
void Sem_task2(void *p_arg)
{
	static u8 num = 0;
	OS_ERR err;

	while(1)
	{
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);		//请求任务内建的信号量
		printf("task sem pend:%d\r\n", num++);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //延时1s

	}
}


void FuncSem_CreateTask(void)
{
	OS_ERR err;
	

	//创建任务1
	OSTaskCreate((OS_TCB 	* )&SemTask1TCB,		
				 (CPU_CHAR	* )"Sem_task1", 		
                 (OS_TASK_PTR )Sem_task1, 			
                 (void		* )0,					
                 (OS_PRIO	  )SemTask1_TASK_PRIO,     
                 (CPU_STK   * )&SemTask1_TASK_STK[0],	
                 (CPU_STK_SIZE)SemTask1_STK_SIZE/10,	
                 (CPU_STK_SIZE)SemTask1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
				 
	//创建任务2
	OSTaskCreate((OS_TCB 	* )&SemTask2TCB,		
				 (CPU_CHAR	* )"Sem_task2", 		
                 (OS_TASK_PTR )Sem_task2, 			
                 (void		* )0,					
                 (OS_PRIO	  )SemTask2_TASK_PRIO,     
                 (CPU_STK   * )&SemTask2_TASK_STK[0],	
                 (CPU_STK_SIZE)SemTask2_STK_SIZE/10,	
                 (CPU_STK_SIZE)SemTask2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
}

