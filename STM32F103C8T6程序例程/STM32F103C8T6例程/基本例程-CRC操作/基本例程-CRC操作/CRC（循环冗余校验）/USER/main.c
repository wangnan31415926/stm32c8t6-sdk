/**************************************
 * �ļ���  ��main.c
 * ����    ��CRC(����У��)ʵ�顣����������ͨ����Ƭ���ڲ���CRCУ���·������CRCУ��
 *           ��ͨ��USART1(����1)�ڵ����ϵĳ����ն˴�ӡ������         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "crc.h" 

extern __IO uint32_t CRCValue;

void RCC_Configuration(void);


int main(void)
{
  u8 i = 0;	
  /* ����ϵͳ(����clock, PLL and Flash ����) */
    SystemInit();	
  /* ���ô���1 */
	USART1_Config();

	/* ʹ��CRCʱ�� */
	CRC_Config();

	printf("\r\n ����һ�� CRC(ѭ������У��)ʵ�� \r\n");

    /* Compute the CRC of "DataBuffer" */
	for(i=0; i<BUFFER_SIZE; i++ ) 
	{
		CRCValue = CRC_CalcBlockCRC((uint32_t *)DataBuffer, BUFFER_SIZE);
		printf("\r\n32-bit CRC У����Ϊ:0X%X\r\n", CRCValue);
	}
 	printf("\r\nCRC(ѭ������У��)�������\r\n");

  while (1)
  {      
  }
}



