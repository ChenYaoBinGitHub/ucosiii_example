#include "task.h"

#include "led.h"
#include "adc.h"
#include "ADS1118.h"
#include "os_app_hooks.h"

#define START_TASK_PRIO		3				//�������ȼ�
#define START_STK_SIZE 		512				//�����ջ��С	
OS_TCB StartTaskTCB;						//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];		//�����ջ	
void start_task(void *p_arg);				//������


/////////////////////////////////////////
int main(void)
{
	
	OS_ERR err;
	CPU_SR_ALLOC();			//Ϊ�ٽ�������κ���OS_CRITICAL_ENTER()������һ������
	
	delay_init();       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	uart_init(115200);    	//���ڲ���������
	GPIO_ADS_Init();		//ADS1118 ��������
	Adc_Init();		  		//ADC��ʼ��
	LED_Init();         	//LED��ʼ��
	printf("progame ok\r\n");
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ(ʹ�ó�����ջ��90%���Ǿ�������)
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�(ͬһ���ȼ���ʹ��ʱ��Ƭ��ת����)
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  	//����UCOSIII
	while(1)
	{
//		adcval = Get_Adc_Average(ADC_Channel_10, 10);

	}

}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#if OS_CFG_APP_HOOKS_EN > 0u
	App_OS_SetAllHooks();  		//ʹ�ù��Ӻ���              
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED0����
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
	//���������������
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
	
	//����ADS1118����
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

	//����������ָ�����
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

	//����ADC����
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
				 
	//����ʱ��Ƭ��ת��������
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

	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,					//OS_TMR *p_tmr:ָ��ʱ����ָ��
				(CPU_CHAR	*)"tmr1",					//CPU_CHAR *p_name:��ʱ������
				(OS_TICK	 )20,						//OS_TICK dly:��ʼ����ʱ�����ӳ�ֵ
				(OS_TICK	 )100,          			//OS_TICK period:�ظ�����
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 		//OS_OPT opt:��ʱ������ѡ��
				(OS_TMR_CALLBACK_PTR)tmr1_callback,	//p_callback:ָ��ص�����������
				(void	    *)0,						//*p_callback_arg:�ص������Ĳ���
                (OS_ERR	    *)&err);					//���صĴ�����
			 
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,						//200*10=2000ms	
                (OS_TICK	 )0,  						//���ζ�ʱ������Ҫ���� period					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 		//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)tmr2_callback,		//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OSTmrStart(&tmr1,&err);	//������ʱ��1
	OSTmrStart(&tmr2,&err);	//������ʱ��2
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}




