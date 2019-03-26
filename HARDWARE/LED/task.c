#include "task.h"

#include "led.h"

//�������Ķ�ջʹ�����
void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str)				//�����ջ������
{
	OS_ERR err;
	CPU_STK_SIZE free;
	CPU_STK_SIZE used;
	
	p_arg = p_arg;

	OSTaskStkChk(p_tcb,&free,&used,&err);
	
	printf("%s:tcb_free:%d\ttcb_use:%d\r\n",title_str, free, used);

}

/////////////////////////////////////////////////////////////////////
OS_TCB SuspendResumeTaskTCB;						//������ƿ�
CPU_STK SuspendResume_TASK_STK[SuspendResume_STK_SIZE];		//�����ջ

void SuspendResume_task(void *p_arg)				//������ָ�����
{
	static u8 cunt = 0;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		cunt++;
		if(cunt == 5)
		{
			printf("suspend float_task\r\n");
			OSTaskSuspend(&FloatTaskTCB, &err);
		}
		if(cunt == 10)
		{
			printf("resume float_task\r\n");
			OSTaskResume(&FloatTaskTCB, &err);
			cunt = 0;
		}
		
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
	}
}

////////////////////////////////////////////////////////////////
OS_TCB Led0TaskTCB;							//������ƿ�
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//�����ջ
void led0_task(void *p_arg)					//led0������
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED0=0;
		printf("LED0_TESK\r\n");
		Usart_ReceiveMsg(USART1);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
		LED0=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
//		stkCheck_task(&Led0TaskTCB, p_arg, "led0");				//���led0����Ķ�ջ�ռ�ʹ��״��
	}
}

/////////////////////////////////////////////////////////////////
OS_TCB	FloatTaskTCB;								//������ƿ�
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
						
void float_task(void *p_arg)		//�����������
{
	CPU_SR_ALLOC();
	static float float_num=0.01;
	
	while(1)
	{
		float_num+=0.01f;
		OS_CRITICAL_ENTER();	//�����ٽ���
		printf("float_num��ֵΪ: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//�˳��ٽ���
		delay_ms(1000);			//��ʱ500ms
//		stkCheck_task(&FloatTaskTCB, p_arg, "float");	//���float����Ķ�ջ�ռ�ʹ��״��
	}
}








