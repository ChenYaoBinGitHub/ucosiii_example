#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"

#include "ADS1118.h"

#define START_TASK_PRIO		3				//�������ȼ�
#define START_STK_SIZE 		512				//�����ջ��С	
OS_TCB StartTaskTCB;						//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];		//�����ջ	
void start_task(void *p_arg);				//������

#define LED0_TASK_PRIO		4				//�������ȼ�
#define LED0_STK_SIZE 		80				//�����ջ��С	
OS_TCB Led0TaskTCB;							//������ƿ�
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];		//�����ջ	
void led0_task(void *p_arg);				//������


#define FLOAT_TASK_PRIO		5						//�������ȼ�
#define FLOAT_STK_SIZE		128						//�����ջ��С
OS_TCB	FloatTaskTCB;								//������ƿ�
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];	//�����ջ(8�ֽڶ���)
void float_task(void *p_arg);						//������

/////////////////////////////////////////
int main(void)
{
	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	uart_init(115200);    	//���ڲ���������
	GPIO_ADS_Init();		//ADS1118 ��������
	LED_Init();         	//LED��ʼ��
	
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
	while(1);

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
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}

//led0������
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED0=0;
		printf("LED0_TESK\r\n");
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
		LED0=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
		stkCheck_task(&Led0TaskTCB, p_arg, "led0");
	}
}

//�����������
void float_task(void *p_arg)
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
		stkCheck_task(&FloatTaskTCB, p_arg, "float");
	}
}
