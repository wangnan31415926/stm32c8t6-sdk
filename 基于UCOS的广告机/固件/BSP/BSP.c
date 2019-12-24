#include "includes.h"




/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
    SystemInit();		          /* ����ϵͳʱ��Ϊ72M */	
    LED_GPIO_Config();        /* LED �˿ڳ�ʼ�� */
	  HT1632C_Interface_Init(); /*ht1632c �˿ڳ�ʼ��*/
    ZK_GPIO_init();           /*�ֿ�оƬ��ʼ��*/
	  USART1_Config();          /*���ڳ�ʼ��*/
	  KEY_GPIO_Init();          /*������ʼ��*/
}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
    SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
}
