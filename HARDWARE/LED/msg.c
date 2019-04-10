#include "msg1.h"


#define EMAIL_Q_NUM	1	//按键消息队列的数量
#define DATAMSG_Q_NUM	4	//发送数据的消息队列的数量
OS_Q EMAIL_Msg;				//定义一个消息队列，用于消息传递，模拟消息邮箱
OS_Q DATA_Msg;				//定义一个消息队列，用于发送数据

u8 data[10] = {0};



//查询DATA_Msg消息队列中的总队列数量和剩余队列数量
u8 check_msg_queue(OS_Q *Msg)
{
	u8 p[20] = {0};
	CPU_SR_ALLOC();
	u8 msgq_remain_size;	//消息队列剩余大小
	
	OS_CRITICAL_ENTER();	//进入临界段
	msgq_remain_size = Msg->MsgQ.NbrEntriesSize-Msg->MsgQ.NbrEntries;
	
//	sprintf((char*)p,"Total Size:%d",DATA_Msg.MsgQ.NbrEntriesSize);	//显示DATA_Msg消息队列总的大小
//	printf("%s\r\n", (char*)p);
	sprintf((char*)p,"Remain Size:%d",msgq_remain_size);	//显示DATA_Msg剩余大小
	printf("%s\r\n", (char*)p);
	OS_CRITICAL_EXIT();		//退出临界段
	
	return msgq_remain_size;
}



//模拟消息邮箱发送信息
OS_TCB	Msg1SendTaskTCB;						//任务控制块
CPU_STK Msg1Send_TASK_STK[Msg1Send_STK_SIZE];		//任务堆栈
void Msg1Send_task(void *p_arg)
{
	OS_ERR err;
	static u8 key = 0;
	
	while(1)
	{
		OSQPost((OS_Q*		)&EMAIL_Msg,		
				(void*		)&key,
				(OS_MSG_SIZE)1,
				(OS_OPT		)OS_OPT_POST_FIFO,
				(OS_ERR*	)&err);
		key++;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //
	}
}

//模拟消息邮箱接收信息
OS_TCB	Msg1RecTaskTCB;						//任务控制块
CPU_STK Msg1Rec_TASK_STK[Msg1Rec_STK_SIZE];		//任务堆栈
void Msg1Rec_task(void *p_arg)
{
	OS_ERR err;
	u8 *key;
	OS_MSG_SIZE size;

	while(1)
	{
		//请求消息KEY_Msg
		key=OSQPend((OS_Q*			)&EMAIL_Msg,   
					(OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		
		printf("email_rec:key:%d\r\n", *key);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}
}


//使用定时器发送信息给消息队列
OS_TMR 	msgtmr;					//消息列队定时器1

void DataMsgTmr_callback(void *p_arg)
{
	static u8 num = 0;
	OS_ERR err;

//	if( check_msg_queue(&DATA_Msg) )
//	{
		sprintf((char*)data,"data:%d",num++);
		//发送消息
		OSQPost((OS_Q*		)&DATA_Msg,		
				(void*		)(void *)data,
				(OS_MSG_SIZE)10,
				(OS_OPT		)OS_OPT_POST_FIFO,
				(OS_ERR*	)&err);
				
		if(err != OS_ERR_NONE)
		{	
			printf("DataMsgTmr:%d\r\n", err);			
			OSTmrStop(&msgtmr,OS_OPT_TMR_NONE,0,&err); //停止定时器1
			printf("msg_tmr stop\r\n");	
		}
//	}

}


//消息列队接收信息数据
OS_TCB	Msg2TaskTCB;						//任务控制块
CPU_STK Msg2_TASK_STK[Msg2_STK_SIZE];	//任务堆栈
void Msg2_task(void *p_arg)
{
	u8 *p;
	OS_MSG_SIZE size;
	OS_ERR err; 
	CPU_SR_ALLOC();
	
	while(1)
	{
		OS_CRITICAL_ENTER();	//进入临界段
		//请求消息
		p=(u8 *)OSQPend((OS_Q*		)&DATA_Msg,   
				  (OS_TICK		)0,
                  (OS_OPT		)OS_OPT_PEND_BLOCKING,
                  (OS_MSG_SIZE*	)&size,	
                  (CPU_TS*		)0,
                  (OS_ERR*		)&err);
		printf("%s\r\n", p);
		OS_CRITICAL_EXIT();		//退出临界段
		
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_PERIODIC,&err);
	
	}
}




//消息列队相关任务创建

void Msg_CreateTask(void)
{
	OS_ERR err;
	
	OSQCreate ((OS_Q*		)&EMAIL_Msg,	//消息队列
                (CPU_CHAR*	)"EMAIL_Msg",		//消息队列名称
                (OS_MSG_QTY	)EMAIL_Q_NUM,	//消息队列长度，这里设置为1
                (OS_ERR*	)&err);			//错误码
	//创建消息队列DATA_Msg
	OSQCreate ((OS_Q*		)&DATA_Msg,	
                (CPU_CHAR*	)"DATA Msg",	
                (OS_MSG_QTY	)DATAMSG_Q_NUM,	
                (OS_ERR*	)&err);	
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&msgtmr,		//定时器1
                (CPU_CHAR	*)"msgtmr",		//定时器名字
                (OS_TICK	 )0,			//0ms
                (OS_TICK	 )50,          //50*10=500ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
	(OS_TMR_CALLBACK_PTR)DataMsgTmr_callback,//定时器1回调函数,发送消息给DATA_Msg消息列队
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
	printf("msgtmr_ERR:%d\r\n", err);
				
	OSTaskCreate((OS_TCB 	* )&Msg1SendTaskTCB,		
				 (CPU_CHAR	* )"Msg1Send_task task", 		
                 (OS_TASK_PTR )Msg1Send_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Msg1Send_TASK_PRIO,     
                 (CPU_STK   * )&Msg1Send_TASK_STK[0],	
                 (CPU_STK_SIZE)Msg1Send_STK_SIZE/10,	
                 (CPU_STK_SIZE)Msg1Send_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);			
	
	OSTaskCreate((OS_TCB 	* )&Msg1RecTaskTCB,		
				 (CPU_CHAR	* )"Msg1Rec_task", 		
                 (OS_TASK_PTR )Msg1Rec_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Msg1Rec_TASK_PRIO,     
                 (CPU_STK   * )&Msg1Rec_TASK_STK[0],	
                 (CPU_STK_SIZE)Msg1Rec_STK_SIZE/10,	
                 (CPU_STK_SIZE)Msg1Rec_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	

	OSTaskCreate((OS_TCB 	* )&Msg2TaskTCB,		
				 (CPU_CHAR	* )"Msg2_task", 		
                 (OS_TASK_PTR )Msg2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Msg2_TASK_PRIO,     
                 (CPU_STK   * )&Msg2_TASK_STK[0],	
                 (CPU_STK_SIZE)Msg2_STK_SIZE/10,	
                 (CPU_STK_SIZE)Msg2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	printf("Msg2_task_ERR:%d\r\n", err);			 
	OSTmrStart(&msgtmr,&err);
	printf("msgtmrstart_ERR:%d\r\n", err);
				 
}

////////////////////////////////////////////////////////////////
//任务内建消息队列

#define TASK_Q_NUM	4		//发任务内建消息队列的长度

u8 TaskQ_Data[10] = {0};

//查询DATA_Msg消息队列中的总队列数量和剩余队列数量
u8 Check_TaskMsgQueue(OS_TCB *TCB)
{
	u8 p[20] = {0};
	CPU_SR_ALLOC();
	u8 msgq_remain_size;	//消息队列剩余大小
	
	OS_CRITICAL_ENTER();	//进入临界段
	msgq_remain_size =Msgdis_TaskTCB.MsgQ.NbrEntriesSize-Msgdis_TaskTCB.MsgQ.NbrEntries;
	sprintf((char*)p,"Remain Size:%d",msgq_remain_size);	//显示DATA_Msg剩余大小
	printf("%s\r\n", p);

	OS_CRITICAL_EXIT();		//退出临界段
	
	return msgq_remain_size;
}

//定时器用于定时发送信息给等待任务内建的消息队列
OS_TMR 	msgQctmr;					//消息列队定时器1
void TaskQMsgTmr_callback(void *p_arg)
{
	static u8 num = 0;
	OS_ERR err;

	if( Check_TaskMsgQueue(&Msgdis_TaskTCB) )
	{
		printf("msgQ:ok\r\n");
		sprintf((char*)TaskQ_Data,"Qmsg:%d",num++);
		//发送消息
		OSTaskQPost((OS_TCB*	)&Msgdis_TaskTCB,	//向任务Msgdis发送消息
                    (void*		)TaskQ_Data,
                    (OS_MSG_SIZE)10,
                    (OS_OPT		)OS_OPT_POST_FIFO,
					(OS_ERR*	)&err);
				
		if(err != OS_ERR_NONE)
		{	
			printf("TaskQ_DataTMR:%d\r\n", err);			
			OSTmrStop(&msgQctmr,OS_OPT_TMR_NONE,0,&err); //停止定时器
		}
	}

}



OS_TCB	Msgdis_TaskTCB;
CPU_STK	MSGDIS_TASK_STK[MSGDIS_STK_SIZE];
//显示消息队列中的消息
void msgdis_task(void *p_arg)
{	
	u8 *p = NULL;
	OS_MSG_SIZE size;
	OS_ERR err; 
	while(1)
	{
		//请求消息
		p = (u8 *)OSTaskQPend((OS_TICK		)0,
						  (OS_OPT		)OS_OPT_PEND_BLOCKING,
						  (OS_MSG_SIZE*	)&size,
						  (CPU_TS*		)0,
						  (OS_ERR*      )&err );
		
		printf("msgPend:%s\r\n", p);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err); //延时1s
	}
}


//任务内建消息列队相关任务创建

void TaskQMsg_CreateTask(void)
{
	OS_ERR err;
	
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&msgQctmr,		//定时器1
                (CPU_CHAR	*)"msgQctmr",		//定时器名字
                (OS_TICK	 )0,			//0ms
                (OS_TICK	 )100,          //50*10=500ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
	(OS_TMR_CALLBACK_PTR)TaskQMsgTmr_callback,//定时器1回调函数,发送消息给DATA_Msg消息列队
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
	printf("msgQtmr_ERR:%d\r\n", err);
				

	//创建MSGDIS任务
	OSTaskCreate((OS_TCB 	* )&Msgdis_TaskTCB,		
				 (CPU_CHAR	* )"Msgdis task", 		
                 (OS_TASK_PTR )msgdis_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MSGDIS_TASK_PRIO,     
                 (CPU_STK   * )&MSGDIS_TASK_STK[0],	
                 (CPU_STK_SIZE)MSGDIS_STK_SIZE/10,	
                 (CPU_STK_SIZE)MSGDIS_STK_SIZE,		
                 (OS_MSG_QTY  )TASK_Q_NUM,		//任务Msgdis_task需要使用内建消息队列，消息队列长度为4					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
				 	 
	OSTmrStart(&msgQctmr,&err);
				 
}

