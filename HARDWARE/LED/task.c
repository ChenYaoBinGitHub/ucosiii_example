#include "task.h"

#include "led.h"
#include "adc.h"

#include "stm32f10x_adc.h"
#include <os_app_hooks.h>

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
//任务挂起与恢复
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
//		printf("suspend float_task\r\n");
			OSTaskSuspend(&FloatTaskTCB, &err);
		}
		if(cunt == 10)
		{
//			printf("resume float_task\r\n");
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
//		printf("LED0_TESK\r\n");
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
//		printf("float_num的值为: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//退出临界区
		delay_ms(1000);			//延时500ms
//		stkCheck_task(&FloatTaskTCB, p_arg, "float");	//检测float任务的堆栈空间使用状况
	}
}

//////////////////////////////////////////////////////
//adc转换

OS_TCB	ADCTaskTCB;								//任务控制块
__align(8) CPU_STK	ADC_TASK_STK[ADC_STK_SIZE];				//任务堆栈(8字节对齐)
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
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}

}

//////////////////////////////////////////////////////
//时间片轮转调度任务

OS_TCB	SchedRoundTaskTCB;								//任务控制块
CPU_STK	SchedRound_TASK_STK[SchedRound_STK_SIZE];				//任务堆栈(8字节对齐)
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
//软件定时器

OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2

void tmr1_callback(void *p_tmr, void *p_arg) 	//定时器1回调函数
{
	static u8 num = 0;
	CPU_SR_ALLOC();			//为临界区代码段函数OS_CRITICAL_ENTER()，申请一个变量
	
	OS_CRITICAL_ENTER();			//进入临界区
	printf("tmr1:%d\r\n", num++);
	OS_CRITICAL_EXIT();				//退出临界区
//	OSTmrStart(&tmr1,&err);	//开启定时器1(如果关闭了定时器，可使用此函数开启)
//	OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);//关闭定时器1

}
void tmr2_callback(void *p_tmr, void *p_arg)	//定时器2回调函数
{
	static u8 num = 0;
	printf("tmr2:%d\r\n", num++);
}

//////////////////////////////////////////////////////////////////
//信号量共享资源

u8 share_resource[30];  //共享资源区
OS_SEM	MY_SEM_SHARE;	//定义一个信号量，用于访问共享资源

OS_TCB	SemShare1TaskTCB;						//任务控制块
CPU_STK SemShare1_TASK_STK[SemShare1_STK_SIZE];	//任务堆栈
void SemShare1_task(void *p_arg)
{
	OS_ERR err;
	u8 task1_str[]="First task Running!";
	
	while(1)
	{
		printf("Semshare:task1111\r\n");
		OSSemPend(&MY_SEM_SHARE,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求信号量,阻塞
		memcpy(share_resource,task1_str,sizeof(task1_str));			//向共享资源区拷贝数据
		delay_ms(300);
		printf("%s\r\n",share_resource);							//串口输出共享资源区数据		
		OSSemPost (&MY_SEM_SHARE,OS_OPT_POST_1,&err);				//发送信号量
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   		//延时1s	
	}

}

OS_TCB	SemShare2TaskTCB;						//任务控制块
CPU_STK SemShare2_TASK_STK[SemShare2_STK_SIZE];	//任务堆栈
void SemShare2_task(void *p_arg)
{
	OS_ERR err;
	u8 task2_str[]="Sencond task Running!";
	
	while(1)
	{
		printf("Semshare:task2222\r\n");
		OSSemPend(&MY_SEM_SHARE,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求信号量,阻塞
		memcpy(share_resource,task2_str,sizeof(task2_str));			//向共享资源区拷贝数据
		delay_ms(300);
		printf("%s\r\n",share_resource);							//串口输出共享资源区数据		
		OSSemPost (&MY_SEM_SHARE,OS_OPT_POST_1,&err);				//发送信号量
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   		//延时1s	
	}
}

///////////////////////////////////////////////////////////////////////////
//信号量任务同步：信号量多数用于任务同步和中断服务函数同步

OS_SEM	MY_SEM_SYNC;		//定义一个信号量，用于任务同步

OS_TCB	SemSync1TaskTCB;							//任务控制块
CPU_STK SemSync1_TASK_STK[SemSync1_STK_SIZE];	//任务堆栈

void SemSync1_task(void *p_arg)
{
	OS_ERR err;
	static u8 count = 0;
	
	while(1)
	{
		if(count++ == 100)
		{
			count = 0;
			OSSemPost(&MY_SEM_SYNC,OS_OPT_POST_1,&err);//发送信号量
			printf("SemSync111:SYNC_SEM.Ctr:%d\r\n",MY_SEM_SYNC.Ctr);
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms
	}
}

OS_TCB	SemSync2TaskTCB;							//任务控制块
CPU_STK SemSync2_TASK_STK[SemSync2_STK_SIZE];	//任务堆栈

void SemSync2_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{	
		OSSemPend(&MY_SEM_SYNC,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
		printf("SemSync222:SYNC_SEM.Ctr:%d\r\n",MY_SEM_SYNC.Ctr);
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);  
	}
}

////////////////////////////////////////////////////////////////////////
//








