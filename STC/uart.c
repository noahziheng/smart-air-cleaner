#include <STC12C5A60S2.H>
#include "uart.h"
#define S2RI 0x01	//串口2接收中断请求标志位
#define MOTOR P14
uint   j =  0   ;
uchar  GP2Y_DATA[]={0xAA,0x00,0x00,0x00,0x00,0x00,0xFF,0x00};
float v0=0.0;
float PM=0.0;
uchar MOTOR_SPEED=10;
uint GP2Y_K=800;
uchar getflag;
uchar *time="00:00:00";
uchar timeflag=0;

void Uart_One_Init()					 //串口1初始化函数，波特率9600
{
	TMOD=0x21;//设置定时器1为工作方式2
	TH1=0xfd;	//设置波特率为9600
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
//	AUXR|=0X40;				//T1*12;
}
//========================================
void Uart_One_Send(char k)			 //串口1发送一个字符
{
	ES  =  0 ;
	 SBUF=k;
    while(TI!=1);
    TI  =  0 ;
		ES  =  1  ;
}
//=========baud 2400=======================
void Uart_Two_Init()			 //串口2初始化函数，波特率2400
{
		AUXR &= 0xF7;		//波特率不倍速
		S2CON = 0x50;		//8位数据,可变波特率
		AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
		BRT = 0x70;		//设定独立波特率发生器重装值
		AUXR |= 0x10;		//启动独立波特率发生器
		IE2=0x01;
}

//=============================================
void UART_One_Printf(uchar *p)
{
   while(* p!='\0') Uart_One_Send(*p++);
}
//===========================================
void Uart_One_Receive() interrupt 4
{
	if(RI==1)
	{
		RI = 0   ;
		if(SBUF=='G'){
			getflag=0;
			timeflag=1;
		}
		if(timeflag>0&&timeflag<9){
			time[timeflag-1]=SBUF;
			timeflag++;
		}else{
			timeflag=0;
		}
		if(SBUF=='M'){
			MOTOR=!MOTOR;
		}
		if(SBUF=='U'){
			MOTOR_SPEED-=1;
		}
		if(SBUF=='D'){
			MOTOR_SPEED+=1;
		}
	}
}
//========================================
void Uart_Two_Receive() interrupt 8
{
	EA=0;
  if(S2CON&S2RI)
	{
		S2CON&=~S2RI;
		GP2Y_DATA[j]=S2BUF;
		if(S2BUF==0xff){
			j=0;
			GP2Y_CAL();
			GP2Y_DATA[7]=0xFF;
		}else if(S2BUF==0xaa){
			j=1;
		}else{
			j++;
		}
	}
	EA=1;
}
//=====================================
void GP2Y_CAL()
{
	uchar flag=0;
	uint sum;
	sum=GP2Y_DATA[1]+GP2Y_DATA[2]+GP2Y_DATA[3]+GP2Y_DATA[4];
	if(sum==GP2Y_DATA[5]){
		v0=(GP2Y_DATA[1]*256.0+GP2Y_DATA[2])/1024.0*5.0;
		PM=v0*GP2Y_K;
	}else{
		v0=0.0;
		PM=999.0;
	}
}