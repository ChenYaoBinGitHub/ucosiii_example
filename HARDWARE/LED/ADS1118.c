#include "ADS1118.h"

void GPIO_ADS_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /*���� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù���
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ù���
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    /*����CE���ţ�GPIOA^2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    CS_SET; 
 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//���ݴ�С8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 					//ʱ�Ӽ��ԣ�����ʱΪ��
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//��1��������Ч��������Ϊ����ʱ��
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS�ź����������
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //8��Ƶ��9MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

int8_t SPI_ADS_Send(uint8_t dat)	 //��������
{
	/* �� SPI���ͻ������ǿ�ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
    /* ͨ�� SPI2����һ�ֽ����� */
    SPI_I2S_SendData(SPI1, dat);		
 
    /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

s16  SPI_RW_Reg(uint16_t CofigReg)
{
	volatile uint8_t readata1;
	volatile uint8_t readata2;
	volatile uint8_t readata3;
	volatile uint8_t readata4;
	volatile uint32_t readata;
	readata = 0;

    readata1=SPI_ADS_Send((uint8_t)(CofigReg>>8));
    readata2=SPI_ADS_Send((uint8_t)CofigReg);
    readata3=SPI_ADS_Send((uint8_t)(CofigReg>>8));
    readata4=SPI_ADS_Send((uint8_t)CofigReg);
//    readata= ((uint32_t)readata4) | ((uint32_t)readata3<<8) | ((uint32_t)readata2<<16) | ((uint32_t)readata1<<24);
	readata=( ((s16)readata1<<8) | (s16)readata2 );
	
    return readata;
}



int16_t SPI_Send(uint16_t dat)	 //��������
{
	/* �� SPI���ͻ������ǿ�ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
    /* ͨ�� SPI2����һ�ֽ����� */
    SPI_I2S_SendData(SPI1, dat);		
 
    /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}
s16 SPI_RW(uint16_t CofigReg)
{
	s16 data1 = 0;


	data1 = SPI_Send(CofigReg);
	SPI_Send(CofigReg);
	
    return (s16)data1;
}

//////////////////////////////////////////////////
//ucosiii
OS_TCB ADS1118TaskTCB;						//������ƿ�	
__align(8) CPU_STK ADS1118_TASK_STK[ADS1118_STK_SIZE];	//�����ջ

void ads1118_task(void *p_arg)				//������
{
	OS_ERR err;
	
	float ad = 0;
	s16 ConversionValue = 0;
	u16 ads1118_register=0x8283;
		
	p_arg = p_arg;
	
	while(1)
	{
		CS_RESET;	//Ƭѡspi
		ConversionValue=SPI_RW_Reg(ads1118_register);//8283  
		CS_SET;
		
	//	printf(" The  Value is 0X%08x ,%08d\r\n",ConversionValue,ConversionValue);
		ad=	(float)ConversionValue*4.096/32768;
		printf("AD=%fV\r\n",ad);
//		stkCheck_task(&ADS1118TaskTCB, p_arg, "ADS1118");
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); 
	}
}




