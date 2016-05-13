#ifndef __UART_H__
#define __UART_H__
#define uchar unsigned char
#define uint unsigned int

extern uchar  GP2Y_DATA[];
extern float PM;
extern uchar getflag;

void Uart_One_Init();					 //串口1初始化函数，波特率9600
//========================================
void Uart_One_Send(char k);			 //串口1发送一个字符

void Uart_Two_Init();			 //串口2初始化函数，波特率2400

//=============================================
void Uart_One_Printf(uchar *p);
//===========================================
void Uart_One_Receive();

//========================================
void Uart_Two_Receive();
void GP2Y_CAL();
//=====================================
#endif