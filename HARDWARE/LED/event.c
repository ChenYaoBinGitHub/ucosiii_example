#include "event.h"

OS_FLAG_GRP	EventFlags;		//定义一个事件标志组


OS_TCB	EVENT_PEND_TaskTCB;							//任务控制块
CPU_STK EVENT_PEND_TASK_STK[EVENT_PEND_STK_SIZE];	//任务堆栈
void EventPend_task(void *p_arg)
{
	OS_FLAGS flags_num;
	OS_ERR err;
	while(1)
	{

		//向事件标志组EventFlags发送标志
		flags_num=OSFlagPost((OS_FLAG_GRP*)&EventFlags,
							 (OS_FLAGS	  )EVENT1_FLAG,
							 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
							 (OS_ERR*	  )&err);
	
		printf("事件标志组111111:%d\r\n",flags_num);

		//向事件标志组EventFlags发送标志
		flags_num=OSFlagPost((OS_FLAG_GRP*)&EventFlags,
							 (OS_FLAGS	  )EVENT2_FLAG,
		(OS_OPT	  )OS_OPT_POST_FLAG_SET,	//置位标志:当EVENT2_FLAG对应的位置位时，标志组相应的位置位
							 (OS_ERR*     )&err);
	
		printf("事件标志组222222:%d\r\n",flags_num);
	
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   
	}	

}

OS_TCB	EVENT_POST_TaskTCB;							//任务控制块
CPU_STK EVENT_POST_TASK_STK[EVENT_POST_STK_SIZE];	//任务堆栈
void EventPost_task(void *p_arg)
{
	OS_ERR err; 
	while(1)
	{
		//等待事件标志组
		OSFlagPend((OS_FLAG_GRP*)&EventFlags,
				   (OS_FLAGS	)EVENT1_FLAG+EVENT2_FLAG,
		     	   (OS_TICK     )0,
				   (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,	//当EVENT1_FLAG+EVENT2_FLAG的标志位被置位时，条件满足，OS_OPT_PEND_FLAG_CONSUME满足条件后标志位清0
				   (CPU_TS*     )0,
				   (OS_ERR*	    )&err);
		
		printf("事件标志组Post:%d\r\n",EventFlags.Flags);	//EventFlags.Flags事件标志组的标志位的值
	}	
}



void Event_CreatTask(void)
{
	OS_ERR err;
	//创建一个事件标志组
	OSFlagCreate((OS_FLAG_GRP*)&EventFlags,		//指向事件标志组
                 (CPU_CHAR*	  )"Event_Flags",	//名字
                 (OS_FLAGS	  )EVENT0_FLAG,		//事件标志组初始值
                 (OS_ERR*  	  )&err);			//错误码	

	//创建标志事件组的发布任务
	OSTaskCreate((OS_TCB 	* )&EVENT_PEND_TaskTCB,		
				 (CPU_CHAR	* )"EventPend_task", 		
                 (OS_TASK_PTR )EventPend_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )EVENT_PEND_TASK_PRIO,     
                 (CPU_STK   * )&EVENT_PEND_TASK_STK[0],	
                 (CPU_STK_SIZE)EVENT_PEND_STK_SIZE/10,	
                 (CPU_STK_SIZE)EVENT_PEND_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);		
				 
	//创建标志事件组的请求任务
	OSTaskCreate((OS_TCB 	* )&EVENT_POST_TaskTCB,		
				 (CPU_CHAR	* )"EventPost_task", 		
                 (OS_TASK_PTR )EventPost_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )EVENT_POST_TASK_PRIO,     
                 (CPU_STK   * )&EVENT_POST_TASK_STK[0],	
                 (CPU_STK_SIZE)EVENT_POST_STK_SIZE/10,	
                 (CPU_STK_SIZE)EVENT_POST_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

}
