#ifndef __ADS1118_H
#define __ADS1118_H

#include "stm32f10x.h"
#include "includes.h"

#define CS_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_4)


//ucosiii
#define ADS1118_TASK_PRIO		6					//�������ȼ�
#define ADS1118_STK_SIZE 		128					//�����ջ��С	
extern OS_TCB ADS1118TaskTCB;						//������ƿ�
extern __align(8) CPU_STK ADS1118_TASK_STK[ADS1118_STK_SIZE]; 	//�����ջ	
void ads1118_task(void *p_arg);						//������

void stkCheck_task(OS_TCB *p_tcb,void *p_arg, char *title_str)	;			//�����ջ������


//user
void GPIO_ADS_Init(void);
int8_t SPI_ADS_Send(uint8_t dat);
s16 SPI_RW_Reg(uint16_t CofigReg);

s16 SPI_RW(uint16_t CofigReg);




#endif
