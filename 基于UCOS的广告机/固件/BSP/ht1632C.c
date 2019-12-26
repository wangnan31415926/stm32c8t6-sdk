/*------------------------------------------------------------------------------------
 * 项目名称:

     HT1632C驱动程序v1.0

 * 功能描述:

     1、接口初始化
	 2、清屏函数
	 3、更新一行，更新若干行，更新一屏
	 4、读取一行数据，读取若干行数据，读取一屏数据

  * 备注:
     - 使用前需要在HT1632C.h文件设置通信接口、级联个数和初始化数据
	 - 取模方向，以字节为单位从左到右从上到下按行取模
-------------------------------------------------------------------------------------*/

/************************包含头文件***************************************************/

#include "stm32f10x.h"
#include "ht1632c.h"

/*************************************************************************************
*函数名：void HT1632C_Interface_Init(void)
*功  能：初始化HT1632C接口 
*说  明：
**************************************************************************************/
void HT1632C_Interface_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化端口

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
*函数名：void HT1632C_Writer(unsigned char Data,unsigned char cnt)
*功  能：HT1632C发送数据函数，高位在前，发送位数可设置
*说  明：
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
//HT1632C发送数据函数，低位在前，发送位数可设置
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
*函数名：unsigned char HT1632C_Reader(unsigned char cnt)
*功  能：HT1632C读取数据函数，返回一个字节
*说  明：
**************************************************************************************/
unsigned char HT1632C_Reader(unsigned char cnt)
{
	unsigned char i,dat;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
	GPIO_InitStructure.GPIO_Pin = DAT;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //DAT设置为输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化端口
	
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

	GPIO_InitStructure.GPIO_Pin = DAT;	                  //DAT设置为输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	return dat;
}

/*************************************************************************************
*函数名：void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row)
*功  能：读取指定行的数据
*说  明：Arr：读出数据要保存到的数组，第一字节保存到Arr[0],第二个字节保存到Arr[1]
		 row:指定读第几行数据
**************************************************************************************/
void HT1632C_Read_Onerow(unsigned char *Arr,unsigned char row)
{
	row=(row-1)<<2;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);							//命令码101
	HT1632C_Writer(row<<1,7);							//写入7位地址
	Arr[0]=HT1632C_Reader(8);							//读取8位数据
	Arr[1]=HT1632C_Reader(8);	
	_CS_Set();//_CS=1;	

} 
/*************************************************************************************
*函数名：void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt)
*功  能：从指定行开始读取指定行数
*说  明：Arr:读取上来的数据保存到的数组，row：从第几行开始读，cnt：读取的行数
**************************************************************************************/
void HT1632C_Read_Multirow(unsigned char *Arr,unsigned char row,unsigned char cnt)
{
	unsigned char i,j;

	row=(row-1)<<2;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);							//命令码101
	HT1632C_Writer(row<<1,7);							//写入7位地址
	for(i=0,j=0;i<cnt;i++,j++,j++)
	{
		Arr[j]=HT1632C_Reader(8);						//读取8位数据
		Arr[j+1]=HT1632C_Reader(8);	
	}
	_CS_Set();//_CS=1;	

} 
/*************************************************************************************
*函数名：void HT1632C_Read_OnePage(unsigned char *Arr)
*功  能：读取一整个模块的数据
*说  明：Arr：读取上来的数据保存的位置
**************************************************************************************/
void HT1632C_Read_OnePage(unsigned char *Arr)
{
	unsigned char i,j;
	_RD_Set();//_RD=1;
	_CS_Clear();//_CS=0;	
	HT1632C_Writer(MOD_READ,3);						//命令码101
	HT1632C_Writer(0,7);							//写入7位地址
	for(i=0,j=0;i<24;i++,j++,j++)
	{
		Arr[j]=HT1632C_Reader(8);					//读取8位数据
		Arr[j+1]=HT1632C_Reader(8);	
	}
	_CS_Set();//_CS=1;	

}     
/*************************************************************************************
*函数名：void HT1632C_Writer_CMD(unsigned char cmd)	
*功  能：HT1632C发送命令函数
*说  明：
**************************************************************************************/
void HT1632C_Writer_CMD(unsigned char cmd)				
{
	_CS_Clear();//_CS=0;											  
	HT1632C_Writer(MOD_CMD,3);			//命令模式代码，只发送前三位100
	HT1632C_Writer(cmd,9);				//命令数据，第9位为0
	_CS_Set();//_CS=1;
}
void HT1632C_1_Writer_CMD(unsigned char cmd)				
{
	_CS_1_Clear();//_CS=0;											  
	HT1632C_Writer(MOD_CMD,3);			//命令模式代码，只发送前三位100
	HT1632C_Writer(cmd,9);				//命令数据，第9位为0
	_CS_1_Set();//_CS=1;
}
/*************************************************************************************
*函数名：void HT1632C_Update_Onerow(unsigned char col,unsigned char row,unsigned char c)		
*功  能：更新指定一行
*说  明：row：第几行，1-24；*p：传递要发送的数据的地址
**************************************************************************************/
void HT1632C_Update_Onerow(unsigned char row,unsigned char *p)	
{
	row=(row-1)<<2;							//计算实际地址

	_CS_Clear();//_CS=0;
	HT1632C_Writer(MOD_WRITE,3);
	HT1632C_Writer(row<<1,7);
	HT1632C_Writer(*p,8);
	HT1632C_Writer(*(p+1),8);
	_CS_Set();//_CS=1;
} 
/*************************************************************************************
*函数名：void HT1632C_Write_MultiRow(unsigned char row,unsigned char *p,unsigned char cnt)
*功  能：从指定行row开始写入连续数据，cnt为写入行数
*说  明：
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
*函数名：void HT1632C_Update_OnePage(unsigned char *p)
*功  能：更新一整屏数据（一屏需要R_MAX个字节的数据）
*说  明：*p传递要发送的数据数组的地址
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
*函数名：void HT1632C_clr(void)	
*功  能：清屏函数
*说  明：
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
*函数名：void HT1632C_clr(void)	
*功  能：HT1632C初始化函数
*说  明：
**************************************************************************************/
void HT1632C_Init(void)		//HT1632C初始化函数
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
void HT1632C_1_Init(void)		//HT1632C初始化函数
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
//buf 显示缓冲区，待显示数据
//length 需要显示的数据大小，一共要显示多少字节
void Display(unsigned char* buf, unsigned short length)
{
	unsigned short i,j,k;
	
	if(length>(R_MAX*2))
  {
	    //屏幕不能全部显示，需要动态显示
		  //		//整个数组有length个字节，分成length/2列，每列2个字节
      //		//所以一共循环length/2次
			for(i=0,j=0;i<length/2;i++,j++,j++)		 
				{
					if(length!=display_length)
          {
						HT1632C_1_clr();
						HT1632C_clr();
						return;
					}
					//一块屏是16行，R_MAX列
					if(i>((length/2)-R_MAX))						 //每显示一屏，指针向后移动2个字节（即一列）
					{
						HT1632C_Write_MultiRow_wn(1,buf+j,R_MAX/2,HT1632C_NUMBER_0);			         //取数组后段若干列显示
						HT1632C_Write_MultiRow_wn(1,&buf[R_MAX]+j,R_MAX/2,HT1632C_NUMBER_1);			 //取数组后段若干列显示
						
						if((R_MAX-(length/2-i))>R_MAX/2)
						{
							HT1632C_Write_MultiRow_wn(length/2-i+1,buf,(R_MAX/2-(length/2-i)),HT1632C_NUMBER_0);	
						  HT1632C_Write_MultiRow_wn(1,&buf[(R_MAX/2-(length/2-i))*2],R_MAX/2,HT1632C_NUMBER_1);	 
						}
						else
						{
						  HT1632C_Write_MultiRow_wn(R_MAX/2-(R_MAX-(length/2-i))+1,buf,R_MAX-(length/2-i),HT1632C_NUMBER_1);	 //从数组前段取若干列显示	
						}
					}								 		//的方式从数组中取数
					else							 //当移动的列数大于56时，需要从数组后段取若干列
					{								 //然后从数组前段取若干列
						HT1632C_Update_OnePage(buf+j,HT1632C_NUMBER_0); 	//当移动列数小于56列时，可以直接通过移动指针的
						HT1632C_Update_OnePage(&buf[R_MAX]+j,HT1632C_NUMBER_1);
					}
					OSTimeDlyHMSM(0, 0,0,100);						 //移动延时
				}
	  
	 }
	else
	{
	    //静态居中显示
	    i=(R_MAX-length/2)/2+1;                                              //计算起始显示行数
	    HT1632C_Write_MultiRow_wn(i,buf,length/4,HT1632C_NUMBER_0);			 //取数组后段若干列显示
		  HT1632C_Write_MultiRow_wn(1,&buf[length/2],length/4,HT1632C_NUMBER_1);			 //取数组后段若干列显示
	}
	
}
