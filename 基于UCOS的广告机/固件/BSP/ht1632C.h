/*------------------------------------------------------------------------------------
 * ��Ŀ����:

     HT1632C��������v1.0

 * ��������:

     1���ӿڳ�ʼ��
	 2����������
	 3������һ�У����������У�����һ��
	 4����ȡһ�����ݣ���ȡ���������ݣ���ȡһ������

 * ��Ȩ��Ϣ:

     (c) �������, 2015.

 * ��ʷ�汾:

     2015-8-16:
       - ��ʼ�汾 V1.0.0;

 * ��ע:
     - ʹ��ǰ��Ҫ������Ľӿ����ò�������ͨ�Žӿ�
----------------------------------------------------------------------------------------*/

#ifndef _HT1632C_H_
#define _HT1632C_H_

#define HT1632C_NUMBER_0 0
#define HT1632C_NUMBER_1 1
#define R_MAX             48 //��ʾ��������
/*---------------------------�ӿ�����---------------------------------------------------*/
#define DAT GPIO_Pin_12
#define _WR GPIO_Pin_13
#define _RD GPIO_Pin_14
#define _CS GPIO_Pin_15
#define _CS_1 GPIO_Pin_11

#define DAT_Clear()   GPIO_ResetBits(GPIOB,DAT)
#define _WR_Clear()   GPIO_ResetBits(GPIOB,_WR)
#define _RD_Clear()   GPIO_ResetBits(GPIOB,_RD)
#define _CS_Clear()   GPIO_ResetBits(GPIOB,_CS)
#define _CS_1_Clear() GPIO_ResetBits(GPIOB,_CS_1)

#define DAT_Set()     GPIO_SetBits(GPIOB,DAT)
#define _WR_Set()     GPIO_SetBits(GPIOB,_WR)
#define _RD_Set()     GPIO_SetBits(GPIOB,_RD)
#define _CS_Set()     GPIO_SetBits(GPIOB,_CS)
#define _CS_1_Set()   GPIO_SetBits(GPIOB,_CS_1)


/*---------------------------��������---------------------------------------------------*/

extern unsigned char const Seg_test[8];			

/*------------------------��������ģʽ��ָ����------------------------------------------*/
#define MOD_READ			0xC0			//������ģʽ
#define MOD_WRITE			0xA0			//д����ģʽ
#define MOD_RMW				0xA0			//���޸�дģʽ
#define MOD_CMD				0x80			//����ģʽ

#define SYS_DIS				0x00			//�ر�ϵͳʱ��
#define SYS_EN				0x01			//��ϵͳʱ��
#define LED_OFF				0x02			//�ر�LED��ʾ
#define LED_ON				0x03			//��LED��ʾ
#define BLINK_OFF			0x08		   	//�ر���˸
#define	BLINK_ON			0X09		   	//����˸
#define	SLAVE_MODE			0X10		   	//��ģʽ
#define	RC_MASTER_MODE		0X18	   		//�ڲ�RCʱ��
#define	EXT_CLK_MASTER_MODE	0X1C   			//�ⲿʱ��
#define COM_OPTION			0X24		   	//16COM��NMOS��©ģʽ 0x2C��16COM��PMOS��©���
#define PWM_DUTY			0XA0			//PWM���ȿ���

/*---------------------------��������---------------------------------------------------*/
void HT1632C_Interface_Init(void);			//ͨ�Žӿڳ�ʼ��
void HT1632C_Init(void);					//HT1632C��ʼ������
void HT1632C_clr(void);						//��������

void HT1632C_1_Init(void);		//HT1632C��ʼ������
void HT1632C_1_clr(void);						//��������

//��ָ����row��ʼд���������ݣ�cntΪд��������pָ��Ҫ���͵����ݵ�����
void HT1632C_Write_MultiRow(unsigned char row,unsigned char *p,unsigned char cnt);
//���µ�row�е�һ�����ݣ�pָ��Ҫ���͵����ݵ�����
void HT1632C_Update_Onerow(unsigned char row,unsigned char *p);
//����һ�������ݣ�pָ��Ҫ���͵����ݵ�����
void HT1632C_Update_OnePage(unsigned char *p,unsigned char number_ic);
//��HT1632C��ȡָ����row��һ�����ݱ��浽Arrָ�������
void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row);
//��HT1632C��ȡָ����row��ʼ��cnt��ô���е����ݱ��浽Arrָ�������
void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt);
//��HT1632C��ȡһ�������ݱ��浽Arrָ�������
void HT1632C_Read_OnePage(unsigned char *Arr);

void HT1632C_Write_MultiRow_wn(unsigned char row,unsigned char *p,unsigned char cnt,unsigned char number_ic);
void Display(unsigned char* buf, unsigned short length);
#endif
