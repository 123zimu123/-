//实现流水灯效果
 #include "reg52.h"
 #include<intrins.h>
 #define led P2
 void delay(int i)
{
 	while(i)
	{
		i--;
	}
}

void main()
{
	int i;
	led=0xfe;
	delay(50000);	
	while(1)
	{
		for(i=0;i<7;i++)
		{
			led = _crol_(led,1);
			delay(50000);
		}
		for(i=0;i<7;i++)
		{
			led=_cror_(led,1);
			delay(50000);
		}
	}
}



