#include <STC12C5A60S2.H>
#include "1602.h"
//********************延时
void delay(uchar z)
{
 uchar i,j;
 for(i=z;i>0;i--)
  for(j=110;j>0;j--);
}
//**********************LCM初始化
void InitLCM()
{
 WriteCommandLCM(0x38,0);  //三次显示模式设置，不检测忙信号
 delay(1);
 WriteCommandLCM(0x38,0);
 delay(1);
 WriteCommandLCM(0x38,0);
 delay(1);
 WriteCommandLCM(0x38,1);  //显示模式设置：16×2显示，5×7点阵，8位数据接口
 WriteCommandLCM(0x0c,1);  //显示模式设置：显示开，无光标
 WriteCommandLCM(0x06,1);  //显示模式设置：光标右移，字符不移
 WriteCommandLCM(0x01,1);  //清屏幕指令，将以前的显示内容清除
}
//*********************读状态
uchar ReadStatusLCM()
{
 P0 = 0x80; 
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 1;
 while (P0 & 0x80); //检测忙信号
 return(0);
}
//**********************写指令
void WriteCommandLCM(uchar WCLCM, Busy)
{
 if(Busy) ReadStatusLCM();
 P0=WCLCM;
 LCM_RS=0;
 LCM_RW=0;
 LCM_E=0;
 LCM_E=1;
}
//***********************写数据
void WriteDataLCM(uchar WDLCM)
{
 ReadStatusLCM();
 P0=WDLCM;
 LCM_RS=1;
 LCM_RW=0;
 LCM_E=0;
 LCM_E=1;
}
//*************************显示字符
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{
 if(Y) X|=0X40;   //Y=1显示第二行，Y=0显示第一行
 X|=0X80;
 WriteCommandLCM(X,1); //X用来选择哪一位，1是用来忙检测
 WriteDataLCM(DData); //DData用来写数据
}
//**************************显示字符串
void DisplayListChar(uchar X,uchar Y,uchar code *DData)
{
 uchar ListLength;
 ListLength=0;
 while(DData[ListLength]!='\0')
 {
  if(X<=15)
  {
   DisplayOneChar( X,Y,DData[ListLength]);
   ListLength++;
   X++;
  }
 }
}