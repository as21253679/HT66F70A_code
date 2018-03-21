#include <HT66F70A.h>
#include "MyType.H"

#define PE1_Rx  _pe1
void PC_TX(char da);
void UART_init();
void tx_string(char data[]);

extern char output[];
char output1[4]={'0','1'};

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,����del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//���ب禡�A����2000���O�g��
}
void GPIO_init()
{
	_pcpu1 = 1;		//  PC1 �]�w���ɹq��
	_pcc1 = 1;		//	PC1 �]�w��J
}
void main()
{	
	_Bool stop = 0;
	_wdtc=0b10101111;						//�����ݪ����p�ɾ�
	UART_init();
	GPIO_init();
	int i;
	while(1)
	{	//PC_TX('j');
		//PC_TX(0x0d);
		//
		if(PC_RX()=='V')
		{
			tx_string("V1234");
			PC_TX(0x0d);
			Delayms(100);
			//stop=1;
		}
	else	if(PC_RX()=='C')
		{
			tx_string("C6789");
			PC_TX(0x0d);
			Delayms(100);
			
			
			
		}
		else
		{
			PC_TX('D');
			PC_TX(output1[1]);
			PC_TX(0x0d);
		}
		
	}
} 