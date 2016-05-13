#ifndef _1602_H_
#define _1602_H_
#define uchar unsigned char
#define uint unsigned int
//************************LCD引脚定义
sbit LCM_RS=P1^5;
sbit LCM_RW=P1^6;
sbit LCM_E=P1^7;
//**************************延时
void delay();
//**************************初始化
void InitLCM();
//****************************读状态
uchar ReadStatusLCM();
//******************************读数据
uchar ResdDataLCM();
//***************************写指令
void WriteCommandLCM(uchar WCLCM, Busy);
//***************************写数据
void WriteDataLCM(uchar WDLCM);
//******************************显示1个字符
void DisplayOneChar(uchar X,uchar Y,uchar DData);
//******************************显示字符串
void DisplayListChar(uchar X,uchar Y,uchar code  *DData);
#endif