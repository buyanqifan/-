#include "rtc.h"
#include "delay.h"

void Rtc_Init(void)
{
	RTC_InitTypeDef	 	RTC_InitStruct;
	RTC_TimeTypeDef 	RTC_TimeStruc;
	RTC_DateTypeDef 	RTC_DateStruct;
	//1��ʹ��PWRʱ�ӣ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//2��ʹ�ܺ󱸼Ĵ�������:   
	PWR_BackupAccessCmd(ENABLE);
	
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x1916)
	{
	
		//3������RTCʱ��Դ��ʹ��RTCʱ�ӣ�
		 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		 RCC_RTCCLKCmd(ENABLE);
		
		//���ʹ��LSE��Ҫ��LSE��
		RCC_LSEConfig(RCC_LSE_ON);
		delay_ms(50);		//��ʱ�ȴ�ʱ���ȶ�
		
		RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;	//24Сʱ��
		RTC_InitStruct.RTC_AsynchPrediv	= 0x7F;					//�첽��Ƶ��
		RTC_InitStruct.RTC_SynchPrediv	= 0xFF;					//ͬ����Ƶ��
		//4�� ��ʼ��RTC(ͬ��/�첽��Ƶϵ����ʱ�Ӹ�ʽ)��
		RTC_Init(&RTC_InitStruct);
		
		RTC_TimeStruc.RTC_H12		= RTC_H12_PM;    //24Сʱ�Ʋ���Ҫд
		RTC_TimeStruc.RTC_Hours		= 16;
		RTC_TimeStruc.RTC_Minutes	= 10;
		RTC_TimeStruc.RTC_Seconds	= 15;
		//5�� ����ʱ�䣺	
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruc);
		
		
		RTC_DateStruct.RTC_Year		= 19;
		RTC_DateStruct.RTC_Month	= 9;
		RTC_DateStruct.RTC_Date		= 19;
		RTC_DateStruct.RTC_WeekDay	= RTC_Weekday_Thursday;
		//6���������ڣ�
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
		
		//���󱸼Ĵ�дֵ
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x1916);
	}
}