/**************************************
 * �ļ���  ��main.c
 * ����    ����ȡCPU��96bit ID �� flash�Ĵ�С����ͨ��USART1�ӳ����ն˴�ӡ������
 *           �Ӵ��ڿɿ�����Ϣ
 *                    
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "chipid.h"

extern u32 ChipUniqueID[3];
/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

int main(void)
{
  /* ����ϵͳʱ��Ϊ72MHZ */
  SystemInit();	
  /* ���ô��� */
  USART1_Config();

  /* ��ȡоƬ��Ϣ */
  Get_ChipID();	

	/* ���оƬID*/
  printf("\r\nоƬ��ΨһIDΪ: %X-%X-%X\r\n", ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);

	/* ���оƬ���� */
  printf("\r\nоƬflash������Ϊ: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));	
  while (1)
  {
     
  }
}	



