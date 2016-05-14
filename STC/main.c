#include <STC12C5A60S2.H>
#include "common.h"
#include "1602.h"
#include "uart.h"
#include "DHT11.h"

#define uchar unsigned char
#define uint unsigned int
#define MOTOR P14
#define LED1 P27
#define LED2 P26
#define BUTTON1 P25
#define BUTTON2 P24
#define BUTTON3 P23
#define BUTTON4 P22
#define buzzer P20

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
	DisplayListChar(0,0," 00:00:00 R 00 O");
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

void Senddata()
{
	Uart_One_Printf("N:");
	Uart_One_Send((int)MOTOR+0x30);
	Uart_One_Send(':');
	Uart_One_Send(8+0x30);
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

void main()
{
	unsigned int i=0 ;
	InitLCD();
	LED1=1;
	LED2=1;
	BUTTON1=1;
	BUTTON2=1;
	BUTTON3=1;
	BUTTON4=1;
	MOTOR=1;
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
	DisplayOneChar(15,0,'N');
	DisplayListChar(0,1,"T=00 H=00 A=000");
	while(1){
		for(i=0;i<8;i++)
			DisplayOneChar(i+1,0,time[i]);
		if(MOTOR)
			DisplayOneChar(10,0,'R');
		else
			DisplayOneChar(10,0,'S');
		CCAP0H=MOTOR_SPEED*16;
		CCAP0L=MOTOR_SPEED*16;
		DisplayOneChar(12,0,MOTOR_SPEED/10+0x30);
		DisplayOneChar(13,0,MOTOR_SPEED%10+0x30);
		RH();
		DisplayOneChar(2,1,U8T_data_H/10+0x30);
		DisplayOneChar(3,1,U8T_data_H%10+0x30);
		DisplayOneChar(7,1,U8RH_data_H/10+0x30);
		DisplayOneChar(8,1,U8RH_data_H%10+0x30);
		DisplayOneChar(12,1,(int)PM/100%10+0x30);
		DisplayOneChar(13,1,(int)PM/10%10+0x30);
		DisplayOneChar(14,1,(int)PM%10+0x30);
		if(PM>100.0){
			MOTOR=1;
			MOTOR_SPEED=(PM/10)<15?(int)(PM/10):15;
		}else if(PM<80.0){
			MOTOR=0;
			MOTOR_SPEED=10;
		}
		Senddata();
	}
}