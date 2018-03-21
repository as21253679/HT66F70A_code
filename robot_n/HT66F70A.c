#include <HT66F70A.h>
#include "MyType.H"
#include <string.h>
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
char string[7];

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期岆
}

void command(char c1,char c2,char c3)
{
	PC_TX(0xFF);
	PC_TX(0xFF);
				
	PC_TX(0x02);
	PC_TX(c1);
	PC_TX(c2);
			
	PC_TX(0x05);
	PC_TX(c3);
				
	PC_TX(0xFF);
	PC_TX(0xFF);
	PC_TX(0x00);
}

void command_29()
{
	char c;
	int i=0;
	
	PC_TX(0xFF);
	PC_TX(0xFE);
	PC_TX(0x29);
	PC_TX(0x2A);
	PC_TX(0x24);
	PC_TX(0xFF);
	PC_TX(0xFE);
	PC_TX(0x00);
	
	/*while(1)
	{
		if(PC_RX()==0xFF)
			break;
	}*/
	c=PC_RX();
	c=PC_RX();
	for(i=1;i<5;i++)
	{
		string[i]=PC_RX();
	}
}

void command_2A()
{	
	char c;
	
	c=PC_RX();
	switch(c)
	{
		case 0x01:
			string[5]='1';
			break;
		case 0x02:
			string[5]='2';
			break;
		case 0x03:
			string[5]='3';
			break;
		case 0x04:
			string[5]='4';
			break;
		case 0x05:
			string[5]='5';
			break;
		case 0x06:
			string[5]='6';
			break;
		case 0x07:
			string[5]='7';
			break;
		case 0x08:
			string[5]='8';
			break;
		case 0x09:
			string[5]='9';
			break;
		case 0x11:
			string[5]='1';
			break;
		case 0x12:
			string[5]='2';
			break;
		case 0x13:
			string[5]='3';
			break;
		case 0x14:
			string[5]='4';
			break;
		case 0x15:
			string[5]='5';
			break;
		case 0x16:
			string[5]='6';
			break;
		case 0x17:
			string[5]='7';
			break;
		case 0x18:
			string[5]='8';
			break;
		case 0x19:
			string[5]='9';
			break;
		default:
			string[5]='0';
			break;
	}
	c=PC_RX();
}

void command_24()
{
	char c;
	
	c=PC_RX();
	if(c==0x01)
		string[6]='1';
	else if(c==0x10)
		string[6]='2';
	else if(c==0x11)
		string[6]='3';
	else
		string[6]='0';
		
}

void main()
{	
	char s,s1,s2,s3;
	_Bool stop = 0;
	_wdtc=0b10101111;
	UART_init();
	string[0]='A';	

	Delayms(5000);
	tx_string_115200("AT+CIPMUX=1\r\n",13);
	Delayms(1000);
	tx_string_115200("AT+CIPSERVER=1,8087\r\n",21);
	
	while(1)
	{
		s=PC_RX_115200();
		if(s==':')
		{
			s1=PC_RX_115200();
			s2=PC_RX_115200();
			s3=PC_RX_115200();
			command(s1,s2,s3);
			command_29();
			command_2A();
			command_24();
			
			tx_string_115200("AT+CIPSEND=0,7\r\n",16);
			Delayms(100);
			tx_string_115200(string,7);
			Delayms(100);
			tx_string_115200("AT+CIPSEND=1,7\r\n",16);
			Delayms(100);
			tx_string_115200(string,7);
			Delayms(100);
		}
	}
}