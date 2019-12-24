/************************************
 * �ļ���  ��crc.c
 * ����    ��crc��ѭ������У�飩Ӧ�ú�����         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0

**********************************************************************************/

#include "crc.h"

__IO uint32_t CRCValue = 0;		 // ���ڴ�Ų�����CRCУ��ֵ

/*
 * ��������CRC_Config
 * ����  ��ʹ��CRCʱ��
 * ����  ����
 * ���  ����
 * ����  : �ⲿ����
 */
void CRC_Config(void)
{
	/* Enable CRC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}


