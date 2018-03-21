#include <HT66F70A.h>
#include "MyType.H"
//d0 d1 d2 d3 d7
#define PE1_Rx  _pe1
#define PE0_Tx  _pe0
char PC_RX();
void PC_TX(char da);
void UART_init();
void tx_string(char data[],int c);

char PC_RX_115200();
void rx_string_115200();
void PC_TX_115200(char da);
void tx_string_115200(char data[],int c);
extern char output[];
char output1[4]={'0','1'};
char s='0',s2='0';//s2紀錄上一個回饋
int gg=0;//震動0.5秒  
int dd=0;//冷熱持續5秒
int ff=0;//電人持續5秒
int r1=0,r2=0,r3=0;//r1震動 r2冷熱 r3店
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
void LED_change()//0洪 3藍   2綠
{
	switch(s)
	{
		case '4':
			_pd0 = 1;_pd1 = 1;_pd2 = 0;_pd3 = 1;
			break;
		case '3':
			_pd0 = 0;_pd1 = 1;_pd2 = 0;_pd3 = 1;
			break;
		case '1':
			_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 0;
			break;
		case '2':
			_pd0 = 0;_pd1 = 1;_pd2 = 1;_pd3 = 1;
			break;
		default:
			_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;
			break;
	}
}
void LED()
{
	int j=0;
	u16 i,z;
	i=0;
	z=100;
	LED_Delay(1000);
	for(j=0;j<120;j++)
	{
		if(i>100)
		{
			LED_change();
		}
		else
		{
			_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;		
			LED_Delay(z); 
			LED_change();
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
			_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;
		}
		else
		{
			_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;		
			LED_Delay(z); 
			LED_change();
			LED_Delay(i);
			i--;
			z++;
		}
	}
}
void GPIO_init()
{
	//製冷開關
	_pcc6 = 0;
	_pc6=1;
	//led
	_pdc0 = 0;    
	_pdc1 = 0;
	_pdc2 = 0;
	_pdc3 = 0;
	_pdc7 = 0;
	_pd0=0;
	_pd1=0;
	_pd2=0;
	_pd3=0;
	_pd7=0;
	//電人
	_pcc0 = 0;
	_pc0=1;
	//震動
	_pec2 = 0;
	_pe2=0;
	//製冷正反
	_pgc0 = 0;
	_pg0=1;
}
DEFINE_ISR(ISR_UART,0x08)      //INT0=0x04    INT1=0x08   
{ 	
	s=PC_RX();
	if(s=='3')      //震動on
	{			
		_pe2=1;//震動on
		r1=1;
		gg=0;
	}
	else if(s=='4')  //電人on
	{
		_pc0=0;//電人on
		_pc6=1;//製冷開關off
		_pg0=1;//製冷正反off
		
		if(r3!=1)
			ff=0;
		r3=1;
	}	
	else if(s=='1')//製冷正
	{
		_pc0=1;//電人off
		_pc6=0;//製冷開關on
		_pg0=1;//製冷正反off
		
		if(r2!=1)
			r2=1;
		dd=0;
	}	
	else if(s=='2')//製冷反
	{
		_pc0=1;//電人off
		_pc6=0;//製冷開關on
		_pg0=0;//製冷正反on
		
		if(r2!=1)
			r2=1;
		dd=0;
	}		 
	else if(s=='0')//全關
	{
		_pc0=1;//電人off
		_pc6=1;//製冷開關off
		_pg0=1;//製冷正反off
	}
	if(s!='3')
		s2=s;
	_int1f=0;
}

DEFINE_ISR(ISR_TIMER,0x14)
{ 
	if(gg>500 && r1==1)
	{
		_pe2=0;
		s=s2;
		r1=0;
		gg=0;
	}
	if(dd>7000 && r2==1)
	{
		_pc6=1;_pg0=1;		
		r2=0;
		dd=0;
		s2='0';
		s='0';
		_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;
	}	
	if(ff>7000 && r3==1)
	{
		_pc0=1;		
		r3=0;
		ff=0;
		s2='0';
		s='0';
		_pd0 = 1;_pd1 = 1;_pd2 = 1;_pd3 = 1;
	}
	if(r1==1)
		gg++;
	if(r2==1)
		dd++;
	if(r3==1)
		ff++;
	_t0pf=0;
}
void main()
{	
	char s;
	_Bool stop = 0;
	_wdtc=0b10101111;						
	UART_init();
	GPIO_init();
	int i;
	
	Delayms(3000);
	tx_string("AT+CWJAP=\"nafbuc2\",\"40343244\"\r\n",31);
	Delayms(7000);
	tx_string("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8087\r\n",38);
	Delayms(1000);
	tx_string("AT+CIPMODE=1\r\n",14);
	Delayms(500);
	
	//timer中斷  
	_emi=1;
	_mf0e=1;
	_mfi0=0b00000001;
	_tm0c0=0b00001000;
	
	_ifs0=0b00001000;		//共用寄存器INT1=PE1
	_integ=0b00001000; 		//負緣觸發
	_intc0=0b00000101;
	_pd7=1;
	while(1)
	{
		LED();
	}
} 