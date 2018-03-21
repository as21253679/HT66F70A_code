#include <HT66F70A.h>
#include "MyType.H"

#define PE1_Rx  _pe1
#define PE0_Tx  _pe0

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
char string[6];

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期岆
}

void LED_init()
{
	/*pg0              pd5
    	pd0 pd1 pd2
    	pc3 pc4 pc5  
    	pd6 pd7 pc2
	ph2              ph3
	
    	pg5   pg7  電池*/
    	
	_pgc0=0;_pdc5=0;_pdc0=0;_pdc1=0;_pdc2=0;_pcc3=0;_pcc4=0;_pcc5=0;_pdc6=0;_pdc7=0;_pcc2=0;_phc2=0;_phc3=0;
	_pgc5=0;_pgc7=0;
	
	_pg0=0;_pd5=0;_pd0=0;_pd1=0;_pd2=0;_pc3=0;_pc4=0;_pc5=0;_pd6=0;_pd7=0;_pc2=0;_ph2=0;_ph3=0;
	_pg5=0;_pg7=0;
}

void LED4()
{
	if(string[0]==0x0F)
		_pg0=1;
	if(string[1]==0x0F)
		_pd5=1;
	if(string[2]==0x0F)
		_ph2=1;
	if(string[3]==0x0F)
		_ph3=1;
}

void LED9()
{
	switch(string[4])
	{
		case '1':
			_pd0=1;
			break;
		case '2':
			_pd1=1;
			break;	
		case '3':
			_pd2=1;
			break;
		case '4':
			_pc5=1;
			break;
		case '5':
			_pc2=1;
			break;
		case '6':
			_pd7=1;
			break;	
		case '7':
			_pd6=1;
			break;
		case '8':
			_pc3=1;
			break;
		case '9':
			_pc4=1;
			break;
	}
}

void shock()
{
	switch(string[5])
	{
		case '1':
			_pg5=1;
			break;
		case '2':
			_pg7=1;
			break;	
		case '3':
			_pg5=1;_pg7=1;
			break;
	}
}

void clear()
{
	_pg0=0;_pd5=0;_pd0=0;_pd1=0;_pd2=0;_pc3=0;_pc4=0;_pc5=0;_pd6=0;_pd7=0;_pc2=0;_ph2=0;_ph3=0;
	_pg5=0;_pg7=0;
}

void main()
{	
	char c;	
	int i,j;		
	_Bool stop = 0;
	_wdtc=0b10101111;
	UART_init();
	LED_init();
	
	Delayms(3000);
	tx_string("AT+CWJAP=\"nafbuc1\",\"40343244\"\r\n",31);
	Delayms(7000);
	tx_string("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8087\r\n",38);
	Delayms(1000);
	tx_string("AT+CIPMODE=1\r\n",14);
	Delayms(500);
	
	while(1)
	{
		c=PC_RX();
		if(c=='A')
		{
			for(i=0;i<6;i++)
			{
				string[i]=PC_RX();
			}
			clear();
			LED4();
			LED9();
			shock();
		}
	}
} 