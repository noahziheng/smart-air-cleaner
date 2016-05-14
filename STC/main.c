#include <STC12C5A60S2.H>
#include "common.h"
#include "1602.h"
#include "uart.h"
#include "DHT11.h"
#define BEER P10

#define uchar unsigned char
#define uint unsigned int
#define MOTOR P14
#define LED1 P27
#define LED2 P26
#define BUTTON1 P25
#define BUTTON2 P24
#define BUTTON3 P23
#define BUTTON4 P22

uchar soundflag=0;

void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void InitLCD ()
{
	InitLCM();
	DisplayListChar(0,0," TUST_SAC BY:GFD");
	DisplayListChar(0,1,"Connecting...");
}

void Init_online()
{
	getflag=1;
	while(getflag){
		Uart_One_Send('A');
		Delay500ms();
		if(getflag==0)
			break;
	}

}

int speedtrans(int x)
{
	if (x==1)
		return 13;
	else if (x==2)
		return 12;
	else if (x==3)
		return 11;
	else if (x==4)
		return 10;
	else if (x==5)
		return 9;
	else if (x==6)
		return 8;
	else if (x==7)
		return 7;
	else if (x==8)
		return 6;
	else if (x==9)
		return 5;
	else if (x==10)
		return 4;
	else
		return 3;
}

void Senddata()
{
	Uart_One_Printf("N:");
	Uart_One_Send((int)MOTOR+0x30);
	Uart_One_Send(':');
	Uart_One_Send((int)MAN_MOTOR+0x30);
	Uart_One_Send(':');
	Uart_One_Send(speedtrans(MOTOR_SPEED)/10+0x30);
	Uart_One_Send(speedtrans(MOTOR_SPEED)%10+0x30);
	Uart_One_Send(':');
	Uart_One_Send(U8T_data_H/10+0x30);
	Uart_One_Send(U8T_data_H%10+0x30);
	Uart_One_Send(':');
	Uart_One_Send(U8RH_data_H/10+0x30);
	Uart_One_Send(U8RH_data_H%10+0x30);
	Uart_One_Send(':');
	Uart_One_Send((int)PM/100%10+0x30);
	Uart_One_Send((int)PM/10%10+0x30);
	Uart_One_Send((int)PM%10+0x30);
}

void buttoncheck()
{
	if(BUTTON1==1){
		Delay20ms();
		if (BUTTON1==1)
		{
			if(MAN_MOTOR){
				if(MOTOR_SPEED>3)
					MOTOR_SPEED-=1;
			}else{
				IAP_CONTR|=0x20;
			}
			while(BUTTON1==0);
		}
	}
	if(BUTTON2==1){
		Delay20ms();
		if (BUTTON2==1)
		{
			if(MOTOR_SPEED<15)
				MOTOR_SPEED+=1;
			while(BUTTON2==0);
		}
	}
	if(BUTTON3==1){
		Delay20ms();
		if (BUTTON3==1)
		{
			if(MAN_MOTOR){
				MOTOR=!MOTOR;
			}else{
				LED1=0;
				sound(800);
				LED1=1;
			}
			while(BUTTON3==0);
		}
	}
	if(BUTTON4==1){
		Delay20ms();
		if (BUTTON4==1)
		{
			MAN_MOTOR=!MAN_MOTOR;
			while(BUTTON4==0);
		}
	}

}

void main()
{
	unsigned int i=0 ;
	InitLCD();
	LED1=1;
	LED2=1;
	BUTTON1=0;
	BUTTON2=0;
	BUTTON3=0;
	BUTTON4=0;
	MOTOR=0;
	CCON=0;          //PCA初始化
	CL=0;       //PCA的16位计数器低八位
	CH=0;       //PCA的16位计数器高八位
	CMOD=0x00; //选择 系统时钟/12 为计数脉冲,则PWM的频率f=sysclk/256/12;
	CCAP0H=0xA0;  //  占空比控制
	CCAP0L=0xA0;
	PCA_PWM0=0x00; //控制占空比的第九位为0
	CCAPM0=0x42;  //允许P13作为PWM输出
	CR=1;     //启动PCA计数器
	Uart_Two_Init();
	Uart_One_Init();
	Init_online();
	BEER=1;
	DisplayListChar(0,0," 00:00:00 R 00 N");
	DisplayListChar(0,1,"T=00 H=00 A=000");
	LED2=0;
	while(1){
		buttoncheck();
		if(MOTOR_SPEED>15)
			MOTOR_SPEED=3;
		else if(MOTOR_SPEED<3)
			MOTOR_SPEED=15;
		for(i=0;i<8;i++)
			DisplayOneChar(i+1,0,time[i]);
		if(MOTOR)
			DisplayOneChar(10,0,'R');
		else
			DisplayOneChar(10,0,'S');
		CCAP0H=MOTOR_SPEED*16;
		CCAP0L=MOTOR_SPEED*16;
		DisplayOneChar(12,0,speedtrans(MOTOR_SPEED)/10+0x30);
		DisplayOneChar(13,0,speedtrans(MOTOR_SPEED)%10+0x30);
		RH();
		DisplayOneChar(2,1,U8T_data_H/10+0x30);
		DisplayOneChar(3,1,U8T_data_H%10+0x30);
		DisplayOneChar(7,1,U8RH_data_H/10+0x30);
		DisplayOneChar(8,1,U8RH_data_H%10+0x30);
		DisplayOneChar(12,1,(int)PM/100%10+0x30);
		DisplayOneChar(13,1,(int)PM/10%10+0x30);
		DisplayOneChar(14,1,(int)PM%10+0x30);
		if(!MAN_MOTOR){
			if(PM>100.0){
				MOTOR=1;
				MOTOR_SPEED=(PM/10)<15?speedtrans((int)(PM/10)):3;
				LED1=0;
				if(soundflag==0)
					sound(800);
				soundflag=1;
			}else if(PM<80.0){
				MOTOR=0;
				MOTOR_SPEED=10;
				LED1=1;
				soundflag=0;
			}else{
				LED1=1;
				soundflag=0;
			}
			DisplayOneChar(15,0,'N');
		}else{
			DisplayOneChar(15,0,'M');
		}
		Senddata();
	}
}