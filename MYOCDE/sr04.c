#include "sr04.h"

/*
PC6 DCMI_D0 ---- TRIG
PC8 DCMI_D2 ---- ECHO
*/

void Sr04_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	
	//��GPIO F��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;  		//����2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //�ٶ�	 
	GPIO_Init(GPIOC, &GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8;  		//����3
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//���
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//����	 
	GPIO_Init(GPIOC, &GPIO_InitStruct);		
	
	//1���ܶ�ʱ��ʱ�ӡ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 168-1;		//1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= 50000;	//1us��һ����
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;  	 //��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
    TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);
	
	//5����ʹ�ܶ�ʱ����
    TIM_Cmd(TIM8, DISABLE);
}

//��������
u16 Get_Sr04_Distance(void)
{
	u16 value;
	
	//�����ź�
	TRIG = 0;
	delay_us(5);
	TRIG = 1;
	delay_us(15);
	TRIG = 0;
	
	TIM8->CNT = 0;		//��ʱ����CNTΪ0
	while(ECHO == 0);    //�ȴ��ߵ�ƽ����
	TIM_Cmd(TIM8, ENABLE);
	
	while(ECHO == 1);    //�ȴ��ߵ�ƽ����
	value = TIM8->CNT;
	TIM_Cmd(TIM8, DISABLE);
	
	
	value = value/58;
	
	return value;

}


