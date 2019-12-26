#include "includes.h"


unsigned Display_flag=0; //显示缓存准备好标致位，准备好后才显示 0 没有准备好；1 准备好了
unsigned short display_length=0; //需要显示多少字节数据

unsigned char display_buf[32*40];
unsigned char Text[128];
unsigned char Text_l=0;
unsigned char uart_R_buf[128];//串口接收缓冲区
unsigned char uart_R_Length=0;//串口接收长度
unsigned char uart_R_flag=0;  //串口接收完成标志位

extern OS_STK task_led2_stk[TASK_LED2_STK_SIZE];	
//任务1 用来获取字模
void Task_Start(void *p_arg)
{
	static unsigned char i=0;
    (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告
	SysTick_init();


	
    while (1)
    {
			if(i<20)
			{
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED1( OFF);   
		    OSTimeDlyHMSM(0, 0,0,100); 
        i++;				
			}	
      LED1( ON);	
      OSTimeDlyHMSM(0, 0,0,10);
			
      if(Display_flag==2)//有显示需要更新
			{
//				UART1SendByte(0x12);
				Display_flag=0;
				memset(display_buf,0,sizeof(display_buf));
				display_length=GET_GB2312_string(Text,display_buf);
//				HT1632C_1_clr();
//				HT1632C_clr();
//				OSTimeDlyHMSM(0, 0,0,1000);	
				Display_flag=1;  //显示更新完成
//				UART1SendByte(0x34);
			}
 			
     			
    }
}

//任务2 用来led显示
void Task_LED2(void *p_arg)
{
	unsigned char i,j;	
  unsigned char *p;						 //定义指向page数组的指针
		
  (void)p_arg;                	
	SysTick_init();


	while(1)
	{
		if(Display_flag==1)
			{
//				UART1SendByte(0xaa);
				Display(display_buf,display_length);
			}
		
		OSTimeDlyHMSM(0, 0,0,10);
	}
		
}


//计算校验值
unsigned char crc8_(unsigned char* buf,unsigned char l)
{
	unsigned char i=0;
	unsigned char crc=0;
	for(i=0;i<l;i++)
	{
		crc=crc^buf[i];
	}
	return crc;
}

#define Fail 0x67
#define Success 0x90

unsigned char AT_IN[]="+++";                  //进入AT模式
unsigned char AT_DEFAULT[]="AT+DEFAULT";      //恢复出厂设置
unsigned char AT_OUT[]="AT+RESET";            //退出AT模式
unsigned char AT_MAC[]="AT+MAC";              //获取mac地址
void uart_revice(void)
{
			if(uart_R_flag==1)//串口有数据接收
		{
			switch (uart_R_Length)
			{
				case 1:	if(uart_R_buf[0]!=0xa5){uart_R_Length=0;}
				break;
				
				case 2: if(uart_R_buf[0]!=0xa5||uart_R_buf[1]!=0xa5){uart_R_Length=0;}
				break;
				
			}
			
			if(uart_R_Length>2)
			{
				if(uart_R_buf[2]!=0){
					if(uart_R_buf[0]!=0xa5||uart_R_buf[1]!=0xa5){uart_R_Length=0;}
					else
						{
							if(uart_R_buf[2]>(uart_R_Length-3))
							{}//数据没有全部接收完，继续接收
							else//数据接收完成，进行分析
							{
								if(1)//(uart_R_buf[uart_R_Length-1]==crc8_(&uart_R_buf[3],uart_R_buf[2]-1))//校验是否正确接收
								{
								  switch (uart_R_buf[3])
									{
				             case 1:
											       memset(Text,0,sizeof(Text));
										         Text_l=uart_R_buf[2]-2;
										         memcpy(Text,&uart_R_buf[4],Text_l);
											       Display_flag=2;//更新显示数据
										         UART1SendByte(Success);
										 break;
										
									}
									uart_R_Length=0;
								
								}
								else
								{
									 uart_R_Length=0;
					         UART1SendByte(Fail);
								}
							}
						}
				}
				else
				{
					uart_R_Length=0;
					UART1SendByte(Fail);
				}
				
			}
				


			uart_R_flag=0;
		}
}
//任务3 用来监听网络数据用来更改显示信息，相关逻辑处理
//串口数据格式：
/*头 + 长度 + 指令 + 数据 + crc（1字节）
 *a5a5  XX     1     XX      xx             -----------设置显示数据
 *a5a5  01     2     NC      NC             -----------进入WiFi模块恢复出厂设置
*/
void Task_wifi(void *p_arg)
{
	unsigned char i,j;	
  unsigned char *p;						 //定义指向page数组的指针
		
  (void)p_arg;                	
	SysTick_init();


	while(1)
	{    

		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
		{uart_revice();}
		else
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//显示mac地址
			{
				OSTimeDlyHMSM(0, 0,0,100);
				while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0);
				OSTimeDlyHMSM(0, 0,0,100);
				for(i=0;i<sizeof(AT_IN)-1;i++)
				{UART1SendByte(AT_IN[i]);}
				OSTimeDlyHMSM(0, 0,0,100);
				for(i=0;i<sizeof(AT_MAC)-1;i++)
				{
				UART1SendByte(AT_MAC[i]);
				}	
				OSTimeDlyHMSM(0, 0,0,100);
				memset(Text,0,sizeof(Text));
				for(i=0;i<uart_R_Length;i++)
				{
					if(uart_R_buf[i]=='M'&&uart_R_buf[i+1]=='A'&&uart_R_buf[i+2]=='C')
					{break;}
				}
				if(i<uart_R_Length){}
				else{i=0;}
				memcpy(Text,&uart_R_buf[i],uart_R_Length-i);
				Text[uart_R_Length-i]=' ';
				
				Display_flag=2;//更新显示数据
				for(i=0;i<sizeof(AT_OUT)-1;i++)
				{
					UART1SendByte(AT_OUT[i]);
				}	
				OSTimeDlyHMSM(0, 0,0,100);
        uart_R_Length=0;				
			}
			else
			{
         if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				 {
						OSTimeDlyHMSM(0, 0,0,100);
						while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0);
						OSTimeDlyHMSM(0, 0,0,100);
					 
						for(i=0;i<sizeof(AT_IN)-1;i++)
						{UART1SendByte(AT_IN[i]);}						
						OSTimeDlyHMSM(0, 0,0,100);
						
						for(i=0;i<sizeof(AT_DEFAULT)-1;i++)
						{
							UART1SendByte(AT_DEFAULT[i]);
						}	
						OSTimeDlyHMSM(0, 0,0,100); 
						
						for(i=0;i<sizeof(AT_OUT)-1;i++)
						{
							UART1SendByte(AT_OUT[i]);
						}	
						OSTimeDlyHMSM(0, 0,0,100);
        
						memcpy(Text,uart_R_buf,2);
				
				    Display_flag=2;//更新显示数据           
            uart_R_Length=0;						
				 }
				
			}
			
		}
		
		
    OSTimeDlyHMSM(0, 0,0,10);		
	}     
		
}


