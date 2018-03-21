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
void LED_Delay(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(200);		//內建函式，延遲2000指令週期岆
}

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期岆
}

void LED()
{
	int j=0;
	u16 i,z;
	while(1)
	{
		i=0;
		z=100;
		LED_Delay(1000);
		for(j=0;j<120;j++)
		{
			if(i>100)
			{
				_pd0 = 0;_pd1 = 0;_pd2 = 0;_pd3 = 0;_pd7 = 0;
			}
			else
			{
				_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;_pd7 = 1;		
				LED_Delay(z); 
				_pd0 = 0;_pd1 = 0;_pd2 = 0;_pd3 = 0;_pd7 = 0;
				LED_Delay(i);
				i++;
				z--;
			}
		}				
		
		i=100;
		z=0;
		LED_Delay(1000);
		for(j=0;j<120;j++)
		{
			if(i<10)
			{
				_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;_pd7 = 1;
			}
			else
			{
				_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;_pd7 = 1;		
				LED_Delay(z); 
				_pd0 = 0;_pd1 = 0;_pd2 = 0;_pd3 = 0;_pd7 = 0;
				LED_Delay(i);
				i--;
				z++;
			}
		}
	}
}

DEFINE_ISR(ISR_UART,0x08)      //INT0=0x04    INT1=0x08   
{ 	
	u8 i=0,buff=0;   //資料計數=0，從頭開始
	GCC_DELAY(20);   //20
	for(i=0; i<8; i++)  //接收8-bit串列資料
	{
		buff=buff >> 1;  //資料緩衝右移1個bit
		if(PE1_Rx)  buff=buff | 0x80;//若接收的bit=1，則令資料的bit7=1
		asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	}
	
	Delayms(100);
	for(i=0; i<8; i++)    //發射8-bit串列資料
	{
		if(buff & 0x01)
			PC_TX_115200('1');
		else
			PC_TX_115200('0');
		buff >>= 1;           //資料右移換下1個bit
		Delayms(10);
	}
	PC_TX_115200('\n');
	_int1f=0;
}

DEFINE_ISR(ISR_TIMER,0x14)
{ 
	g++;
	if(g==1000)
	{
		if(f==0)
		{
			_pe2=0;
			_pd0=0;
			f=1;
		}
		else
		{
			_pe2=1;
			_pd0=1;
			f=0;
		}
		g=0;
	}
	_t0pf=0;
}

void main()
{	
	char s;	
	int i,j;		
	_Bool stop = 0;
	_wdtc=0b10101111;
	UART_init();
	
	//timer中斷  
	/*_emi=1;                                
	_mf0e=1;
	_mfi0=0b00000001;
	_tm0c0=0b00001000;*/
	
/*	_ifs0=0b00001000;		//共用寄存器INT1=PE1
	_integ=0b00001000; 		//負緣觸發
	_intc0=0b00000101;*/
	
	Delayms(5000);
	tx_string_115200("AT+CIPMUX=1\r\n",13);
	Delayms(1000);
	tx_string_115200("AT+CIPSERVER=1,8087\r\n",21);
	Delayms(500);
	while(1)
	{
		s=PC_RX();
		Delayms(100);
		tx_string_115200("AT+CIPSEND=0,1\r\n",16);
		Delayms(100);
		PC_TX_115200(s);
	}
} 