#include "msg.h"


#define EMAIL_Q_NUM	1	//������Ϣ���е�����
#define DATAMSG_Q_NUM	4	//�������ݵ���Ϣ���е�����
OS_Q EMAIL_Msg;				//����һ����Ϣ���У�������Ϣ���ݣ�ģ����Ϣ����
OS_Q DATA_Msg;				//����һ����Ϣ���У����ڷ�������

u8 data[10] = {0};


//��ѯDATA_Msg��Ϣ�����е��ܶ���������ʣ���������
u8 check_msg_queue(OS_Q *Msg)
{
	u8 p[20] = {0};
	CPU_SR_ALLOC();
	u8 msgq_remain_size;	//��Ϣ����ʣ���С
	
	OS_CRITICAL_ENTER();	//�����ٽ��
	msgq_remain_size = Msg->MsgQ.NbrEntriesSize-Msg->MsgQ.NbrEntries;
	
//	sprintf((char*)p,"Total Size:%d",DATA_Msg.MsgQ.NbrEntriesSize);	//��ʾDATA_Msg��Ϣ�����ܵĴ�С
//	printf("%s\r\n", (char*)p);
	sprintf((char*)p,"Remain Size:%d",msgq_remain_size);	//��ʾDATA_Msgʣ���С
	printf("%s\r\n", (char*)p);
	OS_CRITICAL_EXIT();		//�˳��ٽ��
	
	return msgq_remain_size;
}



//ģ����Ϣ���䷢����Ϣ
OS_TCB	Msg1SendTaskTCB;						//������ƿ�
CPU_STK Msg1Send_TASK_STK[Msg1Send_STK_SIZE];		//�����ջ
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

//ģ����Ϣ���������Ϣ
OS_TCB	Msg1RecTaskTCB;						//������ƿ�
CPU_STK Msg1Rec_TASK_STK[Msg1Rec_STK_SIZE];		//�����ջ
void Msg1Rec_task(void *p_arg)
{
	OS_ERR err;
	u8 *key;
	OS_MSG_SIZE size;

	while(1)
	{
		//������ϢKEY_Msg
		key=OSQPend((OS_Q*			)&EMAIL_Msg,   
					(OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		
//		printf("email_rec:key:%d\r\n", *key);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}
}


//ʹ�ö�ʱ��������Ϣ����Ϣ����
OS_TMR 	msgtmr;					//��Ϣ�жӶ�ʱ��1

void DataMsgTmr_callback(void *p_arg)
{
	static u8 num = 0;
	OS_ERR err;

//	if( check_msg_queue(&DATA_Msg) )
//	{
		sprintf((char*)data,"data:%d",num++);
		//������Ϣ
		OSQPost((OS_Q*		)&DATA_Msg,		
				(void*		)(void *)data,
				(OS_MSG_SIZE)10,
				(OS_OPT		)OS_OPT_POST_FIFO,
				(OS_ERR*	)&err);
				
		if(err != OS_ERR_NONE)
		{	
			printf("DataMsgTmr:%d\r\n", err);			
			OSTmrStop(&msgtmr,OS_OPT_TMR_NONE,0,&err); //ֹͣ��ʱ��1
			printf("msg_tmr stop\r\n");	
		}
//	}

}


//��Ϣ�жӽ�����Ϣ����
OS_TCB	Msg2TaskTCB;						//������ƿ�
CPU_STK Msg2_TASK_STK[Msg2_STK_SIZE];	//�����ջ
void Msg2_task(void *p_arg)
{
	u8 *p;
	OS_MSG_SIZE size;
	OS_ERR err; 
	CPU_SR_ALLOC();
	
	
	while(1)
	{
		OS_CRITICAL_ENTER();	//�����ٽ��
		//������Ϣ
		p=(u8 *)OSQPend((OS_Q*		)&DATA_Msg,   
				  (OS_TICK		)0,
                  (OS_OPT		)OS_OPT_PEND_BLOCKING,
                  (OS_MSG_SIZE*	)&size,	
                  (CPU_TS*		)0,
                  (OS_ERR*		)&err);
		printf("%s\r\n", p);
		OS_CRITICAL_EXIT();		//�˳��ٽ��
		
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_PERIODIC,&err);
	
	}
}




//��Ϣ�ж�������񴴽�

void Msg_CreateTask(void)
{
	OS_ERR err;
	
	OSQCreate ((OS_Q*		)&EMAIL_Msg,	//��Ϣ����
                (CPU_CHAR*	)"EMAIL_Msg",		//��Ϣ��������
                (OS_MSG_QTY	)EMAIL_Q_NUM,	//��Ϣ���г��ȣ���������Ϊ1
                (OS_ERR*	)&err);			//������
	//������Ϣ����DATA_Msg
	OSQCreate ((OS_Q*		)&DATA_Msg,	
                (CPU_CHAR*	)"DATA Msg",	
                (OS_MSG_QTY	)DATAMSG_Q_NUM,	
                (OS_ERR*	)&err);	
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&msgtmr,		//��ʱ��1
                (CPU_CHAR	*)"msgtmr",		//��ʱ������
                (OS_TICK	 )0,			//0ms
                (OS_TICK	 )50,          //50*10=500ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
	(OS_TMR_CALLBACK_PTR)DataMsgTmr_callback,//��ʱ��1�ص�����,������Ϣ��DATA_Msg��Ϣ�ж�
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����
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




