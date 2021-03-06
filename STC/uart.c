#include <STC12C5A60S2.H>
#include <intrins.h>
#include "uart.h"
#define S2RI 0x01	//串口2接收中断请求标志位
#define MOTOR P14
#define BEER P10
#define LED1 P27
uchar MAN_MOTOR=0;
uint   j =  0   ;
uchar  GP2Y_DATA[]={0xAA,0x00,0x00,0x00,0x00,0x00,0xFF,0x00};//GP2Y1051AU0F传感器数据帧
float v0=0.0;
float PM=0.0;
uchar MOTOR_SPEED=10;
uint GP2Y_K=800;
uchar getflag;
uchar time[]={'0','1',':','0','0',':','0','0'};
uchar temp[16],tempflag;

void Delay2ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}

void sound(int x){
	uint i;
	for(i=0;i<x;i++){
		BEER=0;
		Delay2ms();
		BEER=1;
		Delay2ms();
	}
}

void Uart_One_Init()	//串口1初始化函数，波特率9600
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
	uint i=0;
	if(RI==1)
	{
		RI = 0;
		if(SBUF=='G'){
			getflag=0;
			tempflag=0;
		}else if(SBUF=='I'){
			tempflag=0;
			for(i=0;i<8;i++)
				time[i]=temp[i];
		}else if(SBUF=='T'){
			LED1=0;
			sound(800);
			LED1=1;
		}else if(SBUF=='R'){
			IAP_CONTR|=0x20;
		}else if(SBUF=='M'){
			MAN_MOTOR=!MAN_MOTOR;
		}else if(SBUF=='C'){
			MOTOR=!MOTOR;
		}else if(SBUF=='U'){
			if(MOTOR_SPEED>3)
				MOTOR_SPEED-=1;
		}else if(SBUF=='D'){
			if(MOTOR_SPEED<15)
				MOTOR_SPEED+=1;
		}else{
			temp[tempflag]=SBUF;
			tempflag++;
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