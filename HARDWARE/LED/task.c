#include "task.h"

#include "led.h"
#include "adc.h"

#include "stm32f10x_adc.h"

//检测任务的堆栈使用情况
void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str)				//计算堆栈任务函数
{
	OS_ERR err;
	CPU_STK_SIZE free;
	CPU_STK_SIZE used;
	
	p_arg = p_arg;

	OSTaskStkChk(p_tcb,&free,&used,&err);
	
	printf("%s:tcb_free:%d\ttcb_use:%d\r\n",title_str, free, used);

}

/////////////////////////////////////////////////////////////////////
OS_TCB SuspendResumeTaskTCB;						//任务控制块
CPU_STK SuspendResume_TASK_STK[SuspendResume_STK_SIZE];		//任务堆栈

void SuspendResume_task(void *p_arg)				//挂起与恢复任务
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
		
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
	}
}

////////////////////////////////////////////////////////////////
OS_TCB Led0TaskTCB;							//任务控制块
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//任务堆栈
void led0_task(void *p_arg)					//led0任务函数
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED0=0;
		printf("LED0_TESK\r\n");
		Usart_ReceiveMsg(USART1);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
		LED0=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
//		stkCheck_task(&Led0TaskTCB, p_arg, "led0");				//检测led0任务的堆栈空间使用状况
	}
}

/////////////////////////////////////////////////////////////////
OS_TCB	FloatTaskTCB;								//任务控制块
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//任务堆栈(8字节对齐)
						
void float_task(void *p_arg)		//浮点测试任务
{
	CPU_SR_ALLOC();
	static float float_num=0.01;
	
	while(1)
	{
		float_num+=0.01f;
		OS_CRITICAL_ENTER();	//进入临界区
		printf("float_num的值为: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//退出临界区
		delay_ms(1000);			//延时500ms
//		stkCheck_task(&FloatTaskTCB, p_arg, "float");	//检测float任务的堆栈空间使用状况
	}
}

//////////////////////////////////////////////////////
//adc转换

#define ADC_TASK_PRIO		8					//任务优先级
#define ADC_STK_SIZE		128					//任务堆栈大小
OS_TCB	ADCTaskTCB;								//任务控制块
__align(8) CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];				//任务堆栈(8字节对齐)
void Adc_task(void *p_arg)
{
	u16 adcval = 0;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		adcval = Get_Adc(ADC_Channel_10);
		printf("ADC_Channel_10:adc:%f\r\n", (float)adcval*3.30/4096);
		adcval = Get_Adc(ADC_Channel_11);
		printf("ADC_Channel_11111:adc:%f\r\n", (float)adcval*3.30/4096);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}

}






