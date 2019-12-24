/*------------------------------------------------------------------------------------
 * 项目名称:

     HT1632C驱动程序v1.0

 * 功能描述:

     1、接口初始化
	 2、清屏函数
	 3、更新一行，更新若干行，更新一屏
	 4、读取一行数据，读取若干行数据，读取一屏数据

 * 版权信息:

     (c) 飞翼电子, 2015.

 * 历史版本:

     2015-8-16:
       - 初始版本 V1.0.0;

 * 备注:
     - 使用前需要在下面的接口设置部分设置通信接口
----------------------------------------------------------------------------------------*/

#ifndef _HT1632C_H_
#define _HT1632C_H_

#define HT1632C_NUMBER_0 0
#define HT1632C_NUMBER_1 1
#define R_MAX             48 //显示最大多少列
/*---------------------------接口设置---------------------------------------------------*/
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


/*---------------------------声明数组---------------------------------------------------*/

extern unsigned char const Seg_test[8];			

/*------------------------定义命令模式和指令码------------------------------------------*/
#define MOD_READ			0xC0			//读数据模式
#define MOD_WRITE			0xA0			//写数据模式
#define MOD_RMW				0xA0			//读修改写模式
#define MOD_CMD				0x80			//命令模式

#define SYS_DIS				0x00			//关闭系统时钟
#define SYS_EN				0x01			//打开系统时钟
#define LED_OFF				0x02			//关闭LED显示
#define LED_ON				0x03			//打开LED显示
#define BLINK_OFF			0x08		   	//关闭闪烁
#define	BLINK_ON			0X09		   	//打开闪烁
#define	SLAVE_MODE			0X10		   	//从模式
#define	RC_MASTER_MODE		0X18	   		//内部RC时钟
#define	EXT_CLK_MASTER_MODE	0X1C   			//外部时钟
#define COM_OPTION			0X24		   	//16COM，NMOS开漏模式 0x2C：16COM，PMOS开漏输出
#define PWM_DUTY			0XA0			//PWM亮度控制

/*---------------------------函数声明---------------------------------------------------*/
void HT1632C_Interface_Init(void);			//通信接口初始化
void HT1632C_Init(void);					//HT1632C初始化函数
void HT1632C_clr(void);						//清屏函数

void HT1632C_1_Init(void);		//HT1632C初始化函数
void HT1632C_1_clr(void);						//清屏函数

//从指定行row开始写入连续数据，cnt为写入行数，p指向要发送的数据的数组
void HT1632C_Write_MultiRow(unsigned char row,unsigned char *p,unsigned char cnt);
//更新第row行的一行数据，p指向要发送的数据的数组
void HT1632C_Update_Onerow(unsigned char row,unsigned char *p);
//更新一整屏数据，p指向要发送的数据的数组
void HT1632C_Update_OnePage(unsigned char *p,unsigned char number_ic);
//从HT1632C读取指定行row的一行数据保存到Arr指向的数组
void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row);
//从HT1632C读取指定行row开始的cnt这么多行的数据保存到Arr指向的数组
void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt);
//从HT1632C读取一整屏数据保存到Arr指向的数组
void HT1632C_Read_OnePage(unsigned char *Arr);

void HT1632C_Write_MultiRow_wn(unsigned char row,unsigned char *p,unsigned char cnt,unsigned char number_ic);
void Display(unsigned char* buf, unsigned short length);
#endif
