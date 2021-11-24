/*
Author:Str.
Created:2021-11-20
Desc:秒表
*/

#include <REGX52.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define SMG P0
#define WELA P1
//-----------------------------------------------------------------------------------------------
//变量声明部分
uint hsec = 0; // 毫秒百分位
uint datas[8]={0,11,0,12,0,0,12,0};//显示数据
uchar n = 0 ;//用于判断时间
bit sign = 0;//用于判断计时器状态
uint recordData[9];//存储的数据节点
uchar recordTimes = 0;//存储的节点数目
uchar breakpoint = 0;//用来显示特点节点
sbit resetBut = P3^0;//重置按钮
//P3^2为启停按钮,P3^3为记录按钮

//-----------------------------------------------------------------------------------------------
//函数声明部分
void Delay20us(); 
void resetAll();
void operateHSEC(uint hsec,uint datas[]);
void Timer0Init(void);
void display();
uchar code table[]={  
	0xc0,//0
	0xf9,//1
	0xa4,//2
	0xb0,//3
	0x99,//4
	0x92,//5
	0x82,//6
	0xf8,//7
	0x80,//8
	0x90,//9
	0x00,//全亮
	0xff,//全灭
	0xBF//-  
};


void display()
{
	uchar i;
	WELA = 0x01;
	for(i=0;i<8;i++)
	{
		SMG = table[datas[i]];
		Delay20us();
		SMG =0xFF;
		WELA = WELA << 1;
	}
}

void main()
{ 
	Timer0Init();
	while(1)
	{
		if (!resetBut)
		{
			Delay20us();
			if (!resetBut)
				if (sign==0)
					resetAll();
		}
		display();
	}
}

void Delay20us()
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57000;
	while (--i);
}
//50ms 12Mhz 一个周期一微秒 50ms = 50000us 65536-50000=15536
void Timer0Init(void)		
{
	EA = 1;
	TMOD=0x01;
	TH0=15536/256;
	TL0=15536%256;
	ET0 = 1;
	IT0 = 1;
	EX0 = 1;
	EX1 = 1;
	IT1 = 1;
	//TR0=1;
}



void timer() interrupt 1
{
	TH0=15536/256;
	TL0=15536%256;
	n++;
	if(n==2)
	{
		n=0;
		hsec++;	
	}
	operateHSEC(hsec,datas);
	

}
void operateHSEC(uint hsec,uint datas[])
{
	uint sec = 0;
	uchar bigSec=0;
	uchar smaSec=0;
	uint min =0;
	min = hsec / 600;
	sec = (hsec % 600) / 10;
	bigSec = sec /10;
	smaSec = sec % 10;
	sec = (hsec % 600) % 10;
	datas[2] = min;
	datas[4] = bigSec;
	datas[5] = smaSec;
	datas[7] = sec;
}

void resetAll()
{
	TR0=0;
	recordTimes=0;
	hsec=0;
	operateHSEC(hsec,datas);
	datas[0]=0;
	sign=0;
	n=0;
	SMG=0xFF;
}

void press() interrupt 0
{
	if(sign==0)
	{
		TR0=1;
		sign=1;
		datas[0]=recordTimes;
	}
	else
	{
		TR0=0;
		sign=0;
	}
	return ; 
}

void recordButton() interrupt 2
{
	if(sign==1&&datas[0]<9)
	{
		recordData[datas[0]]=hsec;
		datas[0]++;
		recordTimes=datas[0];
  }
	else
	{
		if(recordTimes!=0)
		{
			operateHSEC(recordData[breakpoint],datas);
			datas[0]=breakpoint+1;
			breakpoint++;
			breakpoint = (breakpoint>=recordTimes)? 0: breakpoint;	
		}
	}
}
