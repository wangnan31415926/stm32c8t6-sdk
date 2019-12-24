/***************************************
 * �ļ���  ��main.c
 * ����    ������KEY1��KEY2���ֱ�תLED1�� LED2�����ò�ѯ��ʽ��         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0
 
*********************************************************/ 

#include "stm32f10x.h"
#include "led.h"
#include "key.h"  


int main(void)
{   
  SystemInit();	// ����ϵͳʱ��Ϊ72M 	
  LED_GPIO_Config(); //LED �˿ڳ�ʼ��   	
  Key_GPIO_Config();//�����˿ڳ�ʼ��


  while(1)                            
  {	   
 	if( Key_Scan(GPIOA,GPIO_Pin_0) == KEY_ON  )	 //�ж�KEY1�Ƿ���
	{			
     GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));//LED1��ת
	} 
	
		
  }
}


