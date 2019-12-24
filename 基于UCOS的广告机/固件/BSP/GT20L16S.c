#include "stm32f10x.h"
#include "GT20L16s.h"


void ZK_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
	GPIO_InitStructure.GPIO_Pin = Rom_IN|Rom_SCK|Rom_CS;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ���˿�

	GPIO_InitStructure.GPIO_Pin = Rom_OUT;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //����Ϊ���� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ���˿�

	_Rom_IN_Set();
	_Rom_SCK_Set();
	_Rom_CS_Set();
}

/****��ָ�����Ѷ�ֿ�IC***/
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

/****�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
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



/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
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

//�����ֿ�ĵ������з�ʽ����ʾ������ͬ��������Ҫת��һ�µ���
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

//��ȡ�ַ�������
//text �����gb2312����
//buf  ����ĵ��󻺴�
//����ֵΪbuf�����ֽڴ�С
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
			/*������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (unsigned long int)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[16*i],32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
			display_chang_led(&buf[16*i],32);
			i+=2;
			j=j+32;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (unsigned long int)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[16*i],32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
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

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,&buf[j],16 );/*ȡ16���ֽڵ����ݣ��浽"fontbuf[32]"*/
			display_chang_led(&buf[j],16);
			i+=1;
			j=j+16;
		}
		else
			i++;	
	}
	
	return j;
	
}



