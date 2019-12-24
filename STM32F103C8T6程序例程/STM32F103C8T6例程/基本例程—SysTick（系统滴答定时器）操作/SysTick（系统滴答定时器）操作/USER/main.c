/***********************************
 * �ļ���  ��main.c
 * ����    ��1��LED�� SysTick �Ŀ����£���500ms��Ƶ����˸��         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0 
 
*********************************************************/
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{
	
SystemInit();	// ����ϵͳʱ��Ϊ72M 	
LED_GPIO_Config(); //LED �˿ڳ�ʼ�� 

	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
	
  while (1)
  {
	
    LED1( 0 ); 
	
    Delay_us(50000);    // 50000 * 10us = 500ms
	LED1( 1 );    	
	
	Delay_us(50000);
		
  }
}
 






