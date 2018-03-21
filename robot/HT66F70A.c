#include <HT66F70A.h>
#include "MyType.H"
//d0 d1 d2 d3 d7
#define PE1_Rx  _pe0
#define PE0_Tx  _pe1
#define PE3_Tx  _pe3
char PC_RX();
void PC_TX(char da);
void UART_init();
void tx_string(char data[],int len);

char PC_RX_115200();
void rx_string_115200();
void PC_TX_115200(char da);
void tx_string_115200(char data[],int c);
extern char output[];
char output1[4]={'0','1'};
int g=0,f=0;

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,����del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//���ب禡�A����2000���O�g����
}

void command(char c)
{
	switch(c)
	{
		case '1': //���e
			PC_TX(0xFF);
			PC_TX(0xFF);
				
			PC_TX(0x02);
			PC_TX(0x06);
			PC_TX(0x06);
				
			PC_TX(0xFF);
			PC_TX(0xFF);
			PC_TX(0x00);
			break;
			
		case '2': //����
			PC_TX(0xFF);
			PC_TX(0xFF);
				
			PC_TX(0x02);
			PC_TX(0x16);
			PC_TX(0x16);
				
			PC_TX(0xFF);
			PC_TX(0xFF);
			PC_TX(0x00);
			break;
		
		case '3': //���k
			PC_TX(0xFF);
			PC_TX(0xFF);
				
			PC_TX(0x02);
			PC_TX(0x16);
			PC_TX(0x06);
				
			PC_TX(0xFF);
			PC_TX(0xFF);
			PC_TX(0x00);
			break;
			
		case '4': //����
			PC_TX(0xFF);
			PC_TX(0xFF);
				
			PC_TX(0x02);
			PC_TX(0x06);
			PC_TX(0x16);
				
			PC_TX(0xFF);
			PC_TX(0xFF);
			PC_TX(0x00);
			break;
			
		case '5': //��
			PC_TX(0xFF);
			PC_TX(0xFF);
				
			PC_TX(0x02);
			PC_TX(0x00);
			PC_TX(0x00);
				
			PC_TX(0xFF);
			PC_TX(0xFF);
			PC_TX(0x00);
			break;
	}
}

void main()
{	
	char s;	
	int i,j;		
	_Bool stop = 0;
	_wdtc=0b10101111;
	UART_init();
	
	
	
	Delayms(5000);
	tx_string_115200("AT+CIPMUX=1\r\n",13);
	Delayms(1000);
	tx_string_115200("AT+CIPSERVER=1,8087\r\n",21);
	
	while(1)
	{
		s=PC_RX_115200();
		if(s==':')
		{
			s=PC_RX_115200();
			command(s);
		}
	}
} 