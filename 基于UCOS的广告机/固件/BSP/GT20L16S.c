#include "stm32f10x.h"
#include "GT20L16s.h"


void ZK_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
	GPIO_InitStructure.GPIO_Pin = Rom_IN|Rom_SCK|Rom_CS;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化端口

	GPIO_InitStructure.GPIO_Pin = Rom_OUT;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //设置为输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化端口

	_Rom_IN_Set();
	_Rom_SCK_Set();
	_Rom_CS_Set();
}

/****送指令到晶联讯字库IC***/
void send_command_to_ROM( unsigned char datu )
{
	unsigned char i;
	for(i=0;i<8;i++ )
	{
		if(datu&0x80)
			_Rom_IN_Set();
		else
			_Rom_IN_Clear();
			datu = datu<<1;
			_Rom_SCK_Clear();
			_Rom_SCK_Set();
	}
}

/****从晶联讯字库IC中取汉字或字符数据（1个字节）***/
static unsigned char get_data_from_ROM( )
{
	
	unsigned char i;
	unsigned char ret_data=0;
	_Rom_SCK_Set();
	for(i=0;i<8;i++)
	{
//		Rom_OUT=1;
		_Rom_SCK_Clear();
		ret_data=ret_data<<1;
		if( _Rom_OUT_read())
			ret_data=ret_data+1;
		else
			ret_data=ret_data+0;
		_Rom_SCK_Set();
	}
	return(ret_data);
}



/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen )
{
	unsigned char i;
	_Rom_CS_Clear();
	_Rom_SCK_Clear();
	send_command_to_ROM(0x03);
	send_command_to_ROM(addrHigh);
	send_command_to_ROM(addrMid);
	send_command_to_ROM(addrLow);
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();
	_Rom_CS_Set();
}

//由于字库的点阵排列方式与显示驱动不同，所以需要转换一下点阵
void display_chang_led(unsigned char* buf,unsigned char len)
{
	unsigned char data_i[32]={0};
	unsigned char i,j,k;
	k=0;
	
	for(i=0;i<len/2;i++)
	{
		data_i[i*2]=buf[i];
		data_i[2*i+1]=buf[i+len/2];
	}
	
	for(i=0;i<len;i++)
	{
		for(j=0;j<8;j++)
		{
			k=k<<1;
			k=k|((data_i[i]>>j)&0x01);
		}
		data_i[i]=k;
		k=0;
	}
	
	memcpy(buf,data_i,len);
	
}

//获取字符串点阵
//text 输入的gb2312编码
//buf  输出的点阵缓存
//返回值为buf的总字节大小
unsigned long int  fontaddr=0;
unsigned short GET_GB2312_string(unsigned char *text,unsigned char* buf)
{
	unsigned char i= 0;
	unsigned char addrHigh,addrMid,addrLow ;
	unsigned char fontbuf[32];
  unsigned short j=0;	
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			/*国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (unsigned long int)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[16*i],32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_chang_led(&buf[16*i],32);
			i+=2;
			j=j+32;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (unsigned long int)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[16*i],32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_chang_led(&buf[16*i],32);
			i+=2;
			j=j+32;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[16];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[j],16 );/*取16个字节的数据，存到"fontbuf[32]"*/
			display_chang_led(&buf[j],16);
			i+=1;
			j=j+16;
		}
		else
			i++;	
	}
	
	return j;
	
}



