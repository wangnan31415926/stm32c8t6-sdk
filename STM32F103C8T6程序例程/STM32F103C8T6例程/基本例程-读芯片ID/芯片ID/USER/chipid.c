/*************************************
 * �ļ���  ��xxx.c
 * ����    ����ȡCPU��ID������ÿ��оƬ����Ψһ�� 96_bit unique ID         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0 

**********************************************************************************/


#include "chipid.h"

u32 ChipUniqueID[3];

/*  ��ȡоƬID */
void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
}


