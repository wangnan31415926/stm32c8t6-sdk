#ifndef _GT20L16S_H_
#define _GT20L16S_H_

#define Rom_OUT   GPIO_Pin_6  //PB6
#define Rom_IN    GPIO_Pin_7  //PB7
#define Rom_SCK   GPIO_Pin_8  //PB8
#define Rom_CS    GPIO_Pin_9  //PB9

#define _Rom_IN_Clear()    GPIO_ResetBits(GPIOB,Rom_IN)
#define _Rom_SCK_Clear()   GPIO_ResetBits(GPIOB,Rom_SCK)
#define _Rom_CS_Clear()    GPIO_ResetBits(GPIOB,Rom_CS)

#define _Rom_OUT_Set()      GPIO_SetBits(GPIOB,Rom_OUT)
#define _Rom_IN_Set()      GPIO_SetBits(GPIOB,Rom_IN)
#define _Rom_SCK_Set()     GPIO_SetBits(GPIOB,Rom_SCK)
#define _Rom_CS_Set()      GPIO_SetBits(GPIOB,Rom_CS)

#define _Rom_OUT_read()    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)


void ZK_GPIO_init(void);
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen );
unsigned short GET_GB2312_string(unsigned char *text,unsigned char* buf);
#endif

