#include "iic.h"


void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	//��GPIO B��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8|GPIO_Pin_9;  		//����9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //�ٶ�	 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	

}

void Iic_Mode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//����9
	GPIO_InitStruct.GPIO_Mode	= Mode;	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//�������
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;	//50MHZ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Iic_Start(void)
{
	//ģʽ���
	Iic_Mode(GPIO_Mode_OUT);
	
	SCL		 = 1;
	SDA_OUT  = 1;
	delay_us(5);
	
	SDA_OUT = 0;
	delay_us(5);
	
	//SDA��SCLΪ��ǯס����
	SCL		= 0;

}

void Iic_Stop(void)
{
	//ģʽ���
	Iic_Mode(GPIO_Mode_OUT);	
	
	SCL		 = 0;
	SDA_OUT  = 0;
	delay_us(5);
	
	SCL		 = 1;
	delay_us(5);
	
	//SDA��SCLΪ�ߵ�ƽ�����߿���
	SDA_OUT  = 1;

}

//����һλ����
void Iic_Send_Ack(u8 ack)
{
	//ģʽ���
	Iic_Mode(GPIO_Mode_OUT);
	
	SCL		 = 0;
	
	//����׼��
	if(ack == 1)
	{
		SDA_OUT = 1;	
	}
	else
	{
		SDA_OUT = 0;	
	}
	//ʱ���γ�һ���������ڣ�����һλ����
	delay_us(5);
	SCL = 1;
	delay_us(5);
	SCL = 0;
	

}
//����һ���ֽ����� �ȴ���λ���ٴ���λ
void Iic_Send_Byte(u8 data)
{
	u8 i;
	//ģʽ���
	Iic_Mode(GPIO_Mode_OUT);	
	
	
	SCL		 = 0;
	
	for(i=0; i<8; i++)
	{
		//����׼��
		if(	data   &  1<<(7-i) )
		{
			SDA_OUT = 1;	
		}
		else
		{
			SDA_OUT = 0;	
		}
		//ʱ���γ�һ���������ڣ�����һλ����
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;	
	
	}
	
	
}

//����һλ����
u8 Iic_Recv_Ack(void)
{
	u8 ack;
	
	//ģʽ���
	Iic_Mode(GPIO_Mode_IN);		
	
	SCL = 0;
	delay_us(5);
	SCL = 1;
	
	if(SDA_IN == 1)
	{
		ack = 1;
	}
	else
	{
		ack = 0;
	}
	delay_us(5);
	SCL = 0;
	
	return ack;
}

//����һ���ֽ����� �ȽӸ�λ���ٽӵ�λ
u8 Iic_Recv_Byte(void)
{
	u8 i, data = 0;  //0100 1010
	
	//ģʽ���
	Iic_Mode(GPIO_Mode_IN);		
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		delay_us(5);
		SCL = 1;
		
		if(SDA_IN == 1)
		{
			data |= 1<<(7-i);
		}

		delay_us(5);
		SCL = 0;
	}
	
	return data;



}

//addr:д���ݵ���ʼ��ַ; len <= 8;
void At24c02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	
	//�����ź�
	Iic_Start();
	
	//�����豸��ַ����ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}
	
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
	
	
	//��������
	while(len--)
	{
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack failure\n");
			return ;
		}	
		write_buff++;
	
	}
	
	
	Iic_Stop();
	
	printf("write finish\n");
}


void At24c02_Read_Page(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	
	//�����ź�
	Iic_Start();
	
	//�����豸��ַ����ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}
	
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
	//�����ź�
	Iic_Start();	
	
	//�����豸��ַ����ִ�ж�����
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
		
	//��������
	while(len--)
	{
		
		*read_buff = Iic_Recv_Byte();
		
		if(len > 0)
			Iic_Send_Ack(0); //������ЧӦ��
	
		read_buff++;
	
	}
	
	Iic_Send_Ack(1); //������ЧӦ��
	Iic_Stop();
	
	printf("read finish\n");
}




























