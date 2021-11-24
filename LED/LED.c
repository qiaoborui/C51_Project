#include <REG52.H>
sbit power = P1^4;
sbit plus = P1^2;
sbit subtract = P1^3;
sbit powerLed = P1^5;
sbit operateLed = P1^6;
void delay(unsigned int xms);
void main()
{
	P2=0x00;
	powerLed=0;
	operateLed=0;
	//初始化完成
	while(1){
		if (power==0)
		{
			delay(30);
			if(power==0)
			{
				powerLed=~powerLed;
			}
		}
		//电源led控制
		if(powerLed)
		{
			 if(plus==0){
			 	delay(20);
				if(plus==0)
				{	
					operateLed=1;
					P2 = P2 << 1;
					P2+=1;
					while(!plus);
					operateLed=0;
				}
			 }
			 if (subtract==0)
			 {
			 	delay(20);
				if(subtract==0)
				{
					operateLed=1;
					P2 = P2 >> 1;
					while(!subtract);
					operateLed=0;
				}
			 }

		}
		else
		{
			P2 = 0x00;
		}
	}
}
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=0;i<xms;++i)
	{
		for(j=0;j<110;++j);
	}
}
