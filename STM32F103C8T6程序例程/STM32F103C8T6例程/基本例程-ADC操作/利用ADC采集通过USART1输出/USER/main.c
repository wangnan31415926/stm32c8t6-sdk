/*************************************
 * �ļ���  ��main.c
 * ����    ������1(USART1)����Եĳ����ն���1sΪ�����ӡ��ǰADC1��ת����ѹֵ         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"       

extern __IO u16 ADC_ConvertedValue;	 
// �����ʱ
void Delay(unsigned long time)
{unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}

int main(void)
{ 
  u32 AD_value;	  
  /* ����ϵͳʱ��Ϊ72M */      
  SystemInit();	
  /* ���ô��� */
  USART1_Config();
  /* ��ʼ��ADC1 */
  ADC1_Init();

  printf("\r\n -------����һ��ADCʵ��------\r\n");
  printf("\r\n -------ADC��������PA0��------\r\n");

  while (1)
  {
       Delay(500);
	   AD_value  = 3300000/4096*ADC_ConvertedValue/1000;
	   
	   printf("AD value = %d mV  \r\n", AD_value);
  }
}



