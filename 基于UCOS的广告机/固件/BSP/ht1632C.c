/*------------------------------------------------------------------------------------
 * ��Ŀ����:

     HT1632C��������v1.0

 * ��������:

     1���ӿڳ�ʼ��
	 2����������
	 3������һ�У����������У�����һ��
	 4����ȡһ�����ݣ���ȡ���������ݣ���ȡһ������

  * ��ע:
     - ʹ��ǰ��Ҫ��HT1632C.h�ļ�����ͨ�Žӿڡ����������ͳ�ʼ������
	 - ȡģ�������ֽ�Ϊ��λ�����Ҵ��ϵ��°���ȡģ
-------------------------------------------------------------------------------------*/

/************************����ͷ�ļ�***************************************************/

#include "stm32f10x.h"
#include "ht1632c.h"

/*************************************************************************************
*��������void HT1632C_Interface_Init(void)
*��  �ܣ���ʼ��HT1632C�ӿ� 
*˵  ����
**************************************************************************************/
void HT1632C_Interface_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ���˿�

	DAT_Set();
	_WR_Set();
	_RD_Set();
	_CS_Set();
	_CS_1_Set();
	
	HT1632C_Init();
	HT1632C_clr();
	
	HT1632C_1_Init();
	HT1632C_1_clr();
//	DAT=1;
//	_WR=1;
//	_RD=1;
//	_CS=1;
}
/*************************************************************************************
*��������void HT1632C_Writer(unsigned char Data,unsigned char cnt)
*��  �ܣ�HT1632C�������ݺ�������λ��ǰ������λ��������
*˵  ����
**************************************************************************************/
void HT1632C_Writer(unsigned char dat,unsigned char cnt)
{
	unsigned char i;
	for(i=0;i<cnt;i++)
	{
		_WR_Clear();//_WR=0;
		if(dat&0x80)
			DAT_Set();//DAT=1;
		else
			DAT_Clear();//DAT=0;
		dat<<=1;
		_WR_Set();//_WR=1;
	}
}
//HT1632C�������ݺ�������λ��ǰ������λ��������
void HT1632C_Writer_L(unsigned char dat,unsigned char cnt)
{
	unsigned char i;
	for(i=0;i<cnt;i++)
	{
		_WR_Clear();//_WR=0;
		if(dat&0x01)
			DAT_Set();//DAT=1;
		else
			DAT_Clear();//DAT=0;
		dat>>=1;
		_WR_Set();//_WR=1;
	}
}
/*************************************************************************************
*��������unsigned char HT1632C_Reader(unsigned char cnt)
*��  �ܣ�HT1632C��ȡ���ݺ���������һ���ֽ�
*˵  ����
**************************************************************************************/
unsigned char HT1632C_Reader(unsigned char cnt)
{
	unsigned char i,dat;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
	GPIO_InitStructure.GPIO_Pin = DAT;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //DAT����Ϊ���� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ���˿�
	
	for(i=0;i<cnt;i++)
	{
		_RD_Clear();//_RD=0;
		_RD_Set();//_RD=1;
		
		if(GPIO_ReadInputDataBit(GPIOB,DAT))
		{
			dat=dat<<1;
			dat=dat|0x01;
		}
		else
		{
			dat=dat<<1;
		}
	}
	_RD_Set();//_RD=1;

	GPIO_InitStructure.GPIO_Pin = DAT;	                  //DAT����Ϊ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	return dat;
}

/*************************************************************************************
*��������void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row)
*��  �ܣ���ȡָ���е�����
*˵  ����Arr����������Ҫ���浽�����飬��һ�ֽڱ��浽Arr[0],�ڶ����ֽڱ��浽Arr[1]
		 row:ָ�����ڼ�������
**************************************************************************************/
void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row)
{
	row=(row-1)<<2;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);							//������101
	HT1632C_Writer(row<<1,7);							//д��7λ��ַ
	Arr[0]=HT1632C_Reader(8);							//��ȡ8λ����
	Arr[1]=HT1632C_Reader(8);	
	_CS_Set();//_CS=1;	

} 
/*************************************************************************************
*��������void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt)
*��  �ܣ���ָ���п�ʼ��ȡָ������
*˵  ����Arr:��ȡ���������ݱ��浽�����飬row���ӵڼ��п�ʼ����cnt����ȡ������
**************************************************************************************/
void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt)
{
	unsigned char i,j;

	row=(row-1)<<2;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);							//������101
	HT1632C_Writer(row<<1,7);							//д��7λ��ַ
	for(i=0,j=0;i<cnt;i++,j++,j++)
	{
		Arr[j]=HT1632C_Reader(8);						//��ȡ8λ����
		Arr[j+1]=HT1632C_Reader(8);	
	}
	_CS_Set();//_CS=1;	

} 
/*************************************************************************************
*��������void HT1632C_Read_OnePage(unsigned char *Arr)
*��  �ܣ���ȡһ����ģ�������
*˵  ����Arr����ȡ���������ݱ����λ��
**************************************************************************************/
void HT1632C_Read_OnePage(unsigned char *Arr)
{
	unsigned char i,j;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);						//������101
	HT1632C_Writer(0,7);							//д��7λ��ַ
	for(i=0,j=0;i<24;i++,j++,j++)
	{
		Arr[j]=HT1632C_Reader(8);					//��ȡ8λ����
		Arr[j+1]=HT1632C_Reader(8);	
	}
	_CS_Set();//_CS=1;	

}     
/*************************************************************************************
*��������void HT1632C_Writer_CMD(unsigned char cmd)	
*��  �ܣ�HT1632C���������
*˵  ����
**************************************************************************************/
void HT1632C_Writer_CMD(unsigned char cmd)				
{
	_CS_Clear();//_CS=0;											  
	HT1632C_Writer(MOD_CMD,3);			//����ģʽ���룬ֻ����ǰ��λ100
	HT1632C_Writer(cmd,9);				//�������ݣ���9λΪ0
	_CS_Set();//_CS=1;
}
void HT1632C_1_Writer_CMD(unsigned char cmd)				
{
	_CS_1_Clear();//_CS=0;											  
	HT1632C_Writer(MOD_CMD,3);			//����ģʽ���룬ֻ����ǰ��λ100
	HT1632C_Writer(cmd,9);				//�������ݣ���9λΪ0
	_CS_1_Set();//_CS=1;
}
/*************************************************************************************
*��������void HT1632C_Update_Onerow(unsigned char col,unsigned char row,unsigned char c)		
*��  �ܣ�����ָ��һ��
*˵  ����row���ڼ��У�1-24��*p������Ҫ���͵����ݵĵ�ַ
**************************************************************************************/
void HT1632C_Update_Onerow(unsigned char row,unsigned char *p)	
{
	row=(row-1)<<2;							//����ʵ�ʵ�ַ

	_CS_Clear();//_CS=0;
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(row<<1,7);
	HT1632C_Writer(*p,8);
	HT1632C_Writer(*(p+1),8);
	_CS_Set();//_CS=1;
} 
/*************************************************************************************
*��������void HT1632C_Write_MultiRow(unsigned char row,unsigned char *p,unsigned char cnt)
*��  �ܣ���ָ����row��ʼд���������ݣ�cntΪд������
*˵  ����
**************************************************************************************/
void HT1632C_Write_MultiRow(unsigned char row,unsigned char *p,unsigned char cnt)
{
	unsigned char i,addr;

	addr=(row-1)<<2;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(addr<<1,7);
	for(i=0;i<cnt;i++)
		{
			HT1632C_Writer(*p,8);//HT1632C_Writer(*p,8);
			p++;
				HT1632C_Writer(*p,8);//HT1632C_Writer(*p,8);
			p++;
		}
	_CS_Set();//_CS=1;
} 

void HT1632C_Write_MultiRow_wn(unsigned char row,unsigned char *p,unsigned char cnt,unsigned char number_ic)
{
	unsigned char i,addr;

	addr=(row-1)<<2;
	if(number_ic==0){_CS_Clear();}
	else{_CS_1_Clear();}
	
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(addr<<1,7);
	for(i=0;i<cnt;i++)
		{
			HT1632C_Writer_L(*(1+p),8);//HT1632C_Writer(*p,8);
			HT1632C_Writer_L(*p,8);//HT1632C_Writer(*p,8);
			p=p+2;
		}

	if(number_ic==0){_CS_Set();}
	else{_CS_1_Set();}
} 


/*************************************************************************************
*��������void HT1632C_Update_OnePage(unsigned char *p)
*��  �ܣ�����һ�������ݣ�һ����ҪR_MAX���ֽڵ����ݣ�
*˵  ����*p����Ҫ���͵���������ĵ�ַ
**************************************************************************************/
void HT1632C_Update_OnePage(unsigned char *p,unsigned char number_ic)
{
	unsigned char i;

	if(number_ic==0){_CS_Clear();}
	else{_CS_1_Clear();}	
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(0,7);
	for(i=0;i<R_MAX/2;i++)
		{
			HT1632C_Writer_L(*(p+1),8);
			HT1632C_Writer_L(*p,8);
			p=p+2;
		}
	if(number_ic==0){_CS_Set();}
	else{_CS_1_Set();}
} 
/*************************************************************************************
*��������void HT1632C_clr(void)	
*��  �ܣ���������
*˵  ����
**************************************************************************************/
void HT1632C_clr(void)	
{
	unsigned char i;
	_CS_Clear();//_CS=0;
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(0x00,7);
	for(i=0;i<R_MAX;i++)
	{
		HT1632C_Writer(0x00,8);
	}
	_CS_Set();//_CS=1;
} 

void HT1632C_1_clr(void)	
{
	unsigned char i;
	_CS_1_Clear();//_CS=0;
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(0x00,7);
	for(i=0;i<R_MAX;i++)
	{
		HT1632C_Writer(0x00,8);
	}
	_CS_1_Set();//_CS=1;
} 
/*************************************************************************************
*��������void HT1632C_clr(void)	
*��  �ܣ�HT1632C��ʼ������
*˵  ����
**************************************************************************************/
void HT1632C_Init(void)		//HT1632C��ʼ������
{
	_CS_Set();//_CS=1;
	_WR_Set();//_WR=1;
	DAT_Set();//DAT=1;
	HT1632C_Writer_CMD(SYS_DIS);
	HT1632C_Writer_CMD(COM_OPTION);
	HT1632C_Writer_CMD(RC_MASTER_MODE);
	HT1632C_Writer_CMD(SYS_EN);
	HT1632C_Writer_CMD(PWM_DUTY);
	HT1632C_Writer_CMD(BLINK_OFF);
	HT1632C_Writer_CMD(LED_ON);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void HT1632C_1_Init(void)		//HT1632C��ʼ������
{
	_CS_1_Set();//_CS=1;
	_WR_Set();//_WR=1;
	DAT_Set();//DAT=1;
	HT1632C_1_Writer_CMD(SYS_DIS);
	HT1632C_1_Writer_CMD(COM_OPTION);
	HT1632C_1_Writer_CMD(RC_MASTER_MODE);
	HT1632C_1_Writer_CMD(SYS_EN);
	HT1632C_1_Writer_CMD(PWM_DUTY);
	HT1632C_1_Writer_CMD(BLINK_OFF);
	HT1632C_1_Writer_CMD(LED_ON);
}

extern unsigned short display_length;
/************************************wn drive*****************************************/
//buf ��ʾ������������ʾ����
//length ��Ҫ��ʾ�����ݴ�С��һ��Ҫ��ʾ�����ֽ�
void Display(unsigned char* buf, unsigned short length)
{
	unsigned short i,j,k;
	
	if(length>(R_MAX*2))
  {
	    //��Ļ����ȫ����ʾ����Ҫ��̬��ʾ
		  //		//����������length���ֽڣ��ֳ�length/2�У�ÿ��2���ֽ�
      //		//����һ��ѭ��length/2��
			for(i=0,j=0;i<length/2;i++,j++,j++)		 
				{
					if(length!=display_length)
          {
						HT1632C_1_clr();
						HT1632C_clr();
						return;
					}
					//һ������16�У�R_MAX��
					if(i>((length/2)-R_MAX))						 //ÿ��ʾһ����ָ������ƶ�2���ֽڣ���һ�У�
					{
						HT1632C_Write_MultiRow_wn(1,buf+j,R_MAX/2,HT1632C_NUMBER_0);			         //ȡ��������������ʾ
						HT1632C_Write_MultiRow_wn(1,&buf[R_MAX]+j,R_MAX/2,HT1632C_NUMBER_1);			 //ȡ��������������ʾ
						
						if((R_MAX-(length/2-i))>R_MAX/2)
						{
							HT1632C_Write_MultiRow_wn(length/2-i+1,buf,(R_MAX/2-(length/2-i)),HT1632C_NUMBER_0);	
						  HT1632C_Write_MultiRow_wn(1,&buf[(R_MAX/2-(length/2-i))*2],R_MAX/2,HT1632C_NUMBER_1);	 
						}
						else
						{
						  HT1632C_Write_MultiRow_wn(R_MAX/2-(R_MAX-(length/2-i))+1,buf,R_MAX-(length/2-i),HT1632C_NUMBER_1);	 //������ǰ��ȡ��������ʾ	
						}
					}								 		//�ķ�ʽ��������ȡ��
					else							 //���ƶ�����������56ʱ����Ҫ��������ȡ������
					{								 //Ȼ�������ǰ��ȡ������
						HT1632C_Update_OnePage(buf+j,HT1632C_NUMBER_0); 	//���ƶ�����С��56��ʱ������ֱ��ͨ���ƶ�ָ���
						HT1632C_Update_OnePage(&buf[R_MAX]+j,HT1632C_NUMBER_1);
					}
					OSTimeDlyHMSM(0, 0,0,100);						 //�ƶ���ʱ
				}
	  
	 }
	else
	{
	    //��̬������ʾ
	    i=(R_MAX-length/2)/2+1;                                              //������ʼ��ʾ����
	    HT1632C_Write_MultiRow_wn(i,buf,length/4,HT1632C_NUMBER_0);			 //ȡ��������������ʾ
		  HT1632C_Write_MultiRow_wn(1,&buf[length/2],length/4,HT1632C_NUMBER_1);			 //ȡ��������������ʾ
	}
	
}
