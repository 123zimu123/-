#include <absacc.h>
#include <intrins.h>
#include <math.h>
#include "STC12C5A.H"
#include "eeprom.h"
#define uchar unsigned char
#define uint unsigned int
#define led_out P1 

sbit P3_2=P3^2;
sbit P3_3=P3^3;
sbit P3_5=P3^5;

sbit red_v = P1^0;
sbit green_v = P1^1;
sbit yellow_v = P1^2;

sbit red_h = P1^5;
sbit green_h = P1^6;
sbit yellow_h = P1^7;

uchar cnt = 0;
char cnt_add = 0;
char cnt_mis = 0;
char red_sec = 10;
char yellow_sec = 5;
bit led_flag=0;

int sum=0;
uchar code  smg_view[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void delay1ms(uint count)
{
		uint i,j;
		for(i=0;i<count;i++)
		for(j=0;j<120;j++);
}

void smg_display(bit flag)
{	
		uint i;
		sum=IAPReadByte(0x03ff)+cnt_add-cnt_mis;
		if(flag)
		{
			for(i=0;i<4;i++)
			{		
					switch(i)
					{
						case(0):P2=~(0x01);P0=~((sum>5)?(smg_view[(sum-5)/10]):(smg_view[(sum)/10]));break;
						case(1):P2=~(0x02);P0=~((sum>5)?(smg_view[(sum-5)%10]):(smg_view[(sum)%10]));break;	
						case(2):P2=~(0x04);P0=~smg_view[(sum)/10];break;
						case(3):P2=~(0x08);P0=~smg_view[(sum)%10];break;

					}
					delay1ms(20);
					P0=0xff;
			}
		}	  
		if(!flag)
		{
			for(i=0;i<4;i++)
			{		
					switch(i)
					{	
						case(0):P2=~(0x01);P0=~smg_view[sum/10];break;
						case(1):P2=~(0x02);P0=~smg_view[sum%10];break;
						case(2):P2=~(0x04);P0=~((sum>5)?(smg_view[(sum-5)/10]):(smg_view[(sum)/10]));break;
						case(3):P2=~(0x08);P0=~((sum>5)?(smg_view[(sum-5)%10]):(smg_view[(sum)%10]));break;
					}
					delay1ms(20);
					P0=0xff;
			}
		}			
}	

timer0()interrupt 0
{	
		int k;
		cnt_add++;
		P1=~cnt_add;
		delay1ms(3500);
		while(!P3_2)
		{
			P1=~(++cnt_add);
			for(k=30;k>0;k--)
			{
					smg_display(led_flag);
			}
		}
		P1=0xff;
}
timer2()interrupt 2
{		
		int k;
		cnt_mis++;	
		P1=~cnt_mis;
		delay1ms(3500);
		while(!P3_3)
		{
			P1=~(++cnt_mis);
			for(k=30;k>0;k--)
			{
					smg_display(led_flag);
			}
		}
		P1=0xff;
}

void led_v()
{
		if(IAPReadByte(0x03ff)+cnt_add-cnt_mis>5)
		{
				red_v=0;
				green_v=1;
				yellow_v=1;
				
				red_h=1;
				green_h=0;
				yellow_h=1;
		}
		else
		{
				yellow_v=!yellow_v;
				yellow_h=!yellow_h;
				red_v=1;
				green_h=1;
		}
}	
		

void led_h()
{
		if(IAPReadByte(0x03ff)+cnt_add-cnt_mis>5)
		{
				red_v=1;
				green_v=0;
				yellow_v=1;
				
				red_h=0;
				green_h=1;
				yellow_h=1;
		}
		else
		{	
				yellow_v=!yellow_v;
				yellow_h=!yellow_h;
				green_v=1;
				red_h=1;

		}
}	


void main()
{			

			IAPErase(0x03ff);

			IAPWriteByte(0x03ff,30);
	
			P1 = 0xff;
			TCON=0x05;	
			IE=0x85;  

			TR0=1;
			TMOD=0x01;
			while(1)
			{	
				while(P3_5)
				{	
				TH0=0x3c;
				TL0=0xb0;    //50ms	
				while(!TF0)
				{		
						smg_display(led_flag);
				}			
				cnt++;
				if(cnt==20)
				{
						cnt=0;
						red_sec=IAPReadByte(0x03ff)-1;
						IAPErase(0x03ff);
						IAPWriteByte(0x03ff,red_sec);
						smg_display(led_flag);
						if(led_flag)
								led_v();
						else 
								led_h();
						if(red_sec+cnt_add-cnt_mis<0)
						{
								IAPErase(0x03ff);
								IAPWriteByte(0x03ff,30);
								cnt_add=0;
								cnt_mis=0;
								led_flag=!led_flag;
						}
					}
					TF0=0;
				}
				delay1ms(800);
				while(P3_5)
				{
				smg_display(led_flag);
				}
				delay1ms(800);
			}				
}







