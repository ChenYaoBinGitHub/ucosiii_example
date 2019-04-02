#include "task.h"

#include "led.h"
#include "adc.h"
#include "ADS1118.h"
#include "os_app_hooks.h"

#define START_TASK_PRIO		3				//任务优先级
#define START_STK_SIZE 		512				//任务堆栈大小	
OS_TCB StartTaskTCB;						//任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];		//任务堆栈	
void start_task(void *p_arg);				//任务函数


/////////////////////////////////////////
int main(void)
{
	
	OS_ERR err;
	CPU_SR_ALLOC();			//为临界区代码段函数OS_CRITICAL_ENTER()，申请一个变量
	
	delay_init();       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	uart_init(115200);    	//串口波特率设置
	GPIO_ADS_Init();		//ADS1118 引脚配置
	Adc_Init();		  		//ADC初始化
	LED_Init();         	//LED初始化
	printf("progame ok\r\n");
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位(使用超过堆栈的90%就是警告或错误)
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，(同一优先级下使用时间片轮转调度)
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  	//开启UCOSIII
	while(1)
	{
//		adcval = Get_Adc_Average(ADC_Channel_10, 10);

	}

}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#if OS_CFG_APP_HOOKS_EN > 0u
	App_OS_SetAllHooks();  		//使用钩子函数              
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED0任务
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);								
				 printf("led0:%d\r\n", err);		 
	//创建浮点测试任务
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	
	//创建ADS1118任务
	OSTaskCreate((OS_TCB 	* )&ADS1118TaskTCB,		
				 (CPU_CHAR	* )"ADS1118 task", 		
                 (OS_TASK_PTR )ads1118_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )ADS1118_TASK_PRIO,     	
                 (CPU_STK   * )&ADS1118_TASK_STK[0],	
                 (CPU_STK_SIZE)ADS1118_STK_SIZE/10,	
                 (CPU_STK_SIZE)ADS1118_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 

	//创建挂起与恢复任务
	OSTaskCreate((OS_TCB 	* )&SuspendResumeTaskTCB,		
				 (CPU_CHAR	* )"SuspendResume task", 		
                 (OS_TASK_PTR )SuspendResume_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SuspendResume_TASK_PRIO,     	
                 (CPU_STK   * )&SuspendResume_TASK_STK[0],	
                 (CPU_STK_SIZE)SuspendResume_STK_SIZE/10,	
                 (CPU_STK_SIZE)SuspendResume_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	

	//创建ADC任务
	OSTaskCreate((OS_TCB 	* )&ADCTaskTCB,		
				 (CPU_CHAR	* )"ADC task", 		
                 (OS_TASK_PTR )Adc_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )ADC_TASK_PRIO,     	
                 (CPU_STK   * )&ADC_TASK_STK[0],	
                 (CPU_STK_SIZE)ADC_STK_SIZE/10,	
                 (CPU_STK_SIZE)ADC_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )1,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
				 
	//创建时间片轮转调度任务
	OSTaskCreate((OS_TCB 	* )&SchedRoundTaskTCB,		
				 (CPU_CHAR	* )"SchedRound_task", 		
                 (OS_TASK_PTR )SchedRound_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SchedRound_TASK_PRIO,     	
                 (CPU_STK   * )&SchedRound_TASK_STK[0],	
                 (CPU_STK_SIZE)SchedRound_STK_SIZE/10,	
                 (CPU_STK_SIZE)SchedRound_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )1,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);

	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,					//OS_TMR *p_tmr:指向定时器的指针
				(CPU_CHAR	*)"tmr1",					//CPU_CHAR *p_name:定时器名称
				(OS_TICK	 )20,						//OS_TICK dly:初始化定时器的延迟值
				(OS_TICK	 )100,          			//OS_TICK period:重复周期
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 		//OS_OPT opt:定时器运行选项
				(OS_TMR_CALLBACK_PTR)tmr1_callback,	//p_callback:指向回调函数的名字
				(void	    *)0,						//*p_callback_arg:回调函数的参数
                (OS_ERR	    *)&err);					//返回的错误码
			 
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,						//200*10=2000ms	
                (OS_TICK	 )0,  						//单次定时器不需要设置 period					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 		//单次定时器
                (OS_TMR_CALLBACK_PTR)tmr2_callback,		//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OSTmrStart(&tmr1,&err);	//开启定时器1
	OSTmrStart(&tmr2,&err);	//开启定时器2
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
}




