/****************************************
 * �ļ���  ��main.c
 * ����    ������2�������������һ��LED���Թ̶���Ƶ��������˸��Ƶ�ʿɵ����b�����Ӵ�һ��LED������һ������һ�����񣬿��Ը����Լ���Ҫ������������         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0
 
**********************************************************************************/	

#include "includes.h"

OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //����ջ
  
int main(void)
{
  	BSP_Init();
	OSInit();
	OSTaskCreate(Task_Start,(void *)0,
	   &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSStart();
    return 0;
 }


