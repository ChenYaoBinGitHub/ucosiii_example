#include "event.h"

OS_FLAG_GRP	EventFlags;		//����һ���¼���־��


OS_TCB	EVENT_PEND_TaskTCB;							//������ƿ�
CPU_STK EVENT_PEND_TASK_STK[EVENT_PEND_STK_SIZE];	//�����ջ
void EventPend_task(void *p_arg)
{
	OS_FLAGS flags_num;
	OS_ERR err;
	while(1)
	{

		//���¼���־��EventFlags���ͱ�־
		flags_num=OSFlagPost((OS_FLAG_GRP*)&EventFlags,
							 (OS_FLAGS	  )EVENT1_FLAG,
							 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
							 (OS_ERR*	  )&err);
	
		printf("�¼���־��111111:%d\r\n",flags_num);

		//���¼���־��EventFlags���ͱ�־
		flags_num=OSFlagPost((OS_FLAG_GRP*)&EventFlags,
							 (OS_FLAGS	  )EVENT2_FLAG,
		(OS_OPT	  )OS_OPT_POST_FLAG_SET,	//��λ��־:��EVENT2_FLAG��Ӧ��λ��λʱ����־����Ӧ��λ��λ
							 (OS_ERR*     )&err);
	
		printf("�¼���־��222222:%d\r\n",flags_num);
	
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   
	}	

}

OS_TCB	EVENT_POST_TaskTCB;							//������ƿ�
CPU_STK EVENT_POST_TASK_STK[EVENT_POST_STK_SIZE];	//�����ջ
void EventPost_task(void *p_arg)
{
	OS_ERR err; 
	while(1)
	{
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&EventFlags,
				   (OS_FLAGS	)EVENT1_FLAG+EVENT2_FLAG,
		     	   (OS_TICK     )0,
				   (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,	//��EVENT1_FLAG+EVENT2_FLAG�ı�־λ����λʱ���������㣬OS_OPT_PEND_FLAG_CONSUME�����������־λ��0
				   (CPU_TS*     )0,
				   (OS_ERR*	    )&err);
		
		printf("�¼���־��Post:%d\r\n",EventFlags.Flags);	//EventFlags.Flags�¼���־��ı�־λ��ֵ
	}	
}



void Event_CreatTask(void)
{
	OS_ERR err;
	//����һ���¼���־��
	OSFlagCreate((OS_FLAG_GRP*)&EventFlags,		//ָ���¼���־��
                 (CPU_CHAR*	  )"Event_Flags",	//����
                 (OS_FLAGS	  )EVENT0_FLAG,		//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);			//������	

	//������־�¼���ķ�������
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
				 
	//������־�¼������������
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
