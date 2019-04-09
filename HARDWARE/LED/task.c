#include "task.h"

#include "led.h"
#include "adc.h"

#include "stm32f10x_adc.h"
#include <os_app_hooks.h>

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
//���������ָ�
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
//		printf("suspend float_task\r\n");
			OSTaskSuspend(&FloatTaskTCB, &err);
		}
		if(cunt == 10)
		{
//			printf("resume float_task\r\n");
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
//		printf("LED0_TESK\r\n");
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
//		printf("float_num��ֵΪ: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//�˳��ٽ���
		delay_ms(1000);			//��ʱ500ms
//		stkCheck_task(&FloatTaskTCB, p_arg, "float");	//���float����Ķ�ջ�ռ�ʹ��״��
	}
}

//////////////////////////////////////////////////////
//adcת��

OS_TCB	ADCTaskTCB;								//������ƿ�
__align(8) CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];				//�����ջ(8�ֽڶ���)
void Adc_task(void *p_arg)
{
	u16 adcval = 0;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
//		adcval = Get_Adc(ADC_Channel_10);
		printf("ADC_Channel_10:adc:%f\r\n", (float)adcval*3.30/4096);
		adcval = Get_Adc(ADC_Channel_11);
//		printf("ADC_Channel_11111:adc:%f\r\n", (float)adcval*3.30/4096);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}

}

//////////////////////////////////////////////////////
//ʱ��Ƭ��ת��������

OS_TCB	SchedRoundTaskTCB;								//������ƿ�
CPU_STK	SchedRound_TASK_STK[SchedRound_STK_SIZE];				//�����ջ(8�ֽڶ���)
void SchedRound_task(void *p_arg)
{
	
	OS_ERR err;
	OS_TICK	tick;
	p_arg = p_arg;
	
	while(1)
	{	
		tick = OSTimeGet(&err);
//		printf("SchedRound_task-->tick:%d\r\n", tick);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}

}

////////////////////////////////////////////////
//�����ʱ��

OS_TMR 	tmr1;		//��ʱ��1
OS_TMR	tmr2;		//��ʱ��2

void tmr1_callback(void *p_tmr, void *p_arg) 	//��ʱ��1�ص�����
{
	static u8 num = 0;
	CPU_SR_ALLOC();			//Ϊ�ٽ�������κ���OS_CRITICAL_ENTER()������һ������
	
	OS_CRITICAL_ENTER();			//�����ٽ���
	printf("tmr1:%d\r\n", num++);
	OS_CRITICAL_EXIT();				//�˳��ٽ���
//	OSTmrStart(&tmr1,&err);	//������ʱ��1(����ر��˶�ʱ������ʹ�ô˺�������)
//	OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);//�رն�ʱ��1

}
void tmr2_callback(void *p_tmr, void *p_arg)	//��ʱ��2�ص�����
{
	static u8 num = 0;
	printf("tmr2:%d\r\n", num++);
}

//////////////////////////////////////////////////////////////////
//�ź���������Դ

u8 share_resource[30];  //������Դ��
OS_SEM	MY_SEM_SHARE;	//����һ���ź��������ڷ��ʹ�����Դ

OS_TCB	SemShare1TaskTCB;						//������ƿ�
CPU_STK SemShare1_TASK_STK[SemShare1_STK_SIZE];	//�����ջ
void SemShare1_task(void *p_arg)
{
	OS_ERR err;
	u8 task1_str[]="First task Running!";
	
	while(1)
	{
		printf("Semshare:task1111\r\n");
		OSSemPend(&MY_SEM_SHARE,0,OS_OPT_PEND_BLOCKING,0,&err); 	//�����ź���,����
		memcpy(share_resource,task1_str,sizeof(task1_str));			//������Դ����������
		delay_ms(300);
		printf("%s\r\n",share_resource);							//�������������Դ������		
		OSSemPost (&MY_SEM_SHARE,OS_OPT_POST_1,&err);				//�����ź���
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   		//��ʱ1s	
	}

}

OS_TCB	SemShare2TaskTCB;						//������ƿ�
CPU_STK SemShare2_TASK_STK[SemShare2_STK_SIZE];	//�����ջ
void SemShare2_task(void *p_arg)
{
	OS_ERR err;
	u8 task2_str[]="Sencond task Running!";
	
	while(1)
	{
		printf("Semshare:task2222\r\n");
		OSSemPend(&MY_SEM_SHARE,0,OS_OPT_PEND_BLOCKING,0,&err); 	//�����ź���,����
		memcpy(share_resource,task2_str,sizeof(task2_str));			//������Դ����������
		delay_ms(300);
		printf("%s\r\n",share_resource);							//�������������Դ������		
		OSSemPost (&MY_SEM_SHARE,OS_OPT_POST_1,&err);				//�����ź���
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   		//��ʱ1s	
	}
}

///////////////////////////////////////////////////////////////////////////
//�ź�������ͬ�����ź���������������ͬ�����жϷ�����ͬ��

OS_SEM	MY_SEM_SYNC;		//����һ���ź�������������ͬ��

OS_TCB	SemSync1TaskTCB;							//������ƿ�
CPU_STK SemSync1_TASK_STK[SemSync1_STK_SIZE];	//�����ջ

void SemSync1_task(void *p_arg)
{
	OS_ERR err;
	static u8 count = 0;
	
	while(1)
	{
		if(count++ == 100)
		{
			count = 0;
			OSSemPost(&MY_SEM_SYNC,OS_OPT_POST_1,&err);//�����ź���
			printf("SemSync111:SYNC_SEM.Ctr:%d\r\n",MY_SEM_SYNC.Ctr);
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms
	}
}

OS_TCB	SemSync2TaskTCB;							//������ƿ�
CPU_STK SemSync2_TASK_STK[SemSync2_STK_SIZE];	//�����ջ

void SemSync2_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{	
		OSSemPend(&MY_SEM_SYNC,0,OS_OPT_PEND_BLOCKING,0,&err);//�����ź���
		printf("SemSync222:SYNC_SEM.Ctr:%d\r\n",MY_SEM_SYNC.Ctr);
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);  
	}
}

////////////////////////////////////////////////////////////////////////
//








