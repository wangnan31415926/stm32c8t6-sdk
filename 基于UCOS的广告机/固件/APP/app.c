#include "includes.h"


unsigned Display_flag=0; //��ʾ����׼���ñ���λ��׼���ú����ʾ 0 û��׼���ã�1 ׼������
unsigned short display_length=0; //��Ҫ��ʾ�����ֽ�����

unsigned char display_buf[32*40];
unsigned char Text[128];
unsigned char Text_l=0;
unsigned char uart_R_buf[128];//���ڽ��ջ�����
unsigned char uart_R_Length=0;//���ڽ��ճ���
unsigned char uart_R_flag=0;  //���ڽ�����ɱ�־λ

extern OS_STK task_led2_stk[TASK_LED2_STK_SIZE];	
//����1 ������ȡ��ģ
void Task_Start(void *p_arg)
{
	static unsigned char i=0;
    (void)p_arg;                				// 'p_arg' ��û���õ�����ֹ��������ʾ����
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
			
      if(Display_flag==2)//����ʾ��Ҫ����
			{
//				UART1SendByte(0x12);
				Display_flag=0;
				memset(display_buf,0,sizeof(display_buf));
				display_length=GET_GB2312_string(Text,display_buf);
//				HT1632C_1_clr();
//				HT1632C_clr();
//				OSTimeDlyHMSM(0, 0,0,1000);	
				Display_flag=1;  //��ʾ�������
//				UART1SendByte(0x34);
			}
 			
     			
    }
}

//����2 ����led��ʾ
void Task_LED2(void *p_arg)
{
	unsigned char i,j;	
  unsigned char *p;						 //����ָ��page�����ָ��
		
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


//����У��ֵ
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

unsigned char AT_IN[]="+++";                  //����ATģʽ
unsigned char AT_DEFAULT[]="AT+DEFAULT";      //�ָ���������
unsigned char AT_OUT[]="AT+RESET";            //�˳�ATģʽ
unsigned char AT_MAC[]="AT+MAC";              //��ȡmac��ַ
void uart_revice(void)
{
			if(uart_R_flag==1)//���������ݽ���
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
							{}//����û��ȫ�������꣬��������
							else//���ݽ�����ɣ����з���
							{
								if(1)//(uart_R_buf[uart_R_Length-1]==crc8_(&uart_R_buf[3],uart_R_buf[2]-1))//У���Ƿ���ȷ����
								{
								  switch (uart_R_buf[3])
									{
				             case 1:
											       memset(Text,0,sizeof(Text));
										         Text_l=uart_R_buf[2]-2;
										         memcpy(Text,&uart_R_buf[4],Text_l);
											       Display_flag=2;//������ʾ����
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
//����3 ��������������������������ʾ��Ϣ������߼�����
//�������ݸ�ʽ��
/*ͷ + ���� + ָ�� + ���� + crc��1�ֽڣ�
 *a5a5  XX     1     XX      xx             -----------������ʾ����
 *a5a5  01     2     NC      NC             -----------����WiFiģ��ָ���������
*/
void Task_wifi(void *p_arg)
{
	unsigned char i,j;	
  unsigned char *p;						 //����ָ��page�����ָ��
		
  (void)p_arg;                	
	SysTick_init();


	while(1)
	{    

		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
		{uart_revice();}
		else
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//��ʾmac��ַ
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
				
				Display_flag=2;//������ʾ����
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
				
				    Display_flag=2;//������ʾ����           
            uart_R_Length=0;						
				 }
				
			}
			
		}
		
		
    OSTimeDlyHMSM(0, 0,0,10);		
	}     
		
}


