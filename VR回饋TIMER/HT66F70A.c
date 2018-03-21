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
char s='0',s2='0';//s2�����W�@�Ӧ^�X
int gg=0;//�_��0.5��  
int dd=0;//�N������5��
int ff=0;//�q�H����5��
int r1=0,r2=0,r3=0;//r1�_�� r2�N�� r3��
void LED_Delay(u16 del)
{	
	u16 i;									//@fSYS=8MH,����del*1ms
	for(i=0;i<del;i++) GCC_DELAY(200);		//���ب禡�A����2000���O�g����
}
void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,����del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//���ب禡�A����2000���O�g����
}
void LED_change()//0�x 3��   2��
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
	//�s�N�}��
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
	//�q�H
	_pcc0 = 0;
	_pc0=1;
	//�_��
	_pec2 = 0;
	_pe2=0;
	//�s�N����
	_pgc0 = 0;
	_pg0=1;
}
DEFINE_ISR(ISR_UART,0x08)      //INT0=0x04    INT1=0x08   
{ 	
	s=PC_RX();
	if(s=='3')      //�_��on
	{			
		_pe2=1;//�_��on
		r1=1;
		gg=0;
	}
	else if(s=='4')  //�q�Hon
	{
		_pc0=0;//�q�Hon
		_pc6=1;//�s�N�}��off
		_pg0=1;//�s�N����off
		
		if(r3!=1)
			ff=0;
		r3=1;
	}	
	else if(s=='1')//�s�N��
	{
		_pc0=1;//�q�Hoff
		_pc6=0;//�s�N�}��on
		_pg0=1;//�s�N����off
		
		if(r2!=1)
			r2=1;
		dd=0;
	}	
	else if(s=='2')//�s�N��
	{
		_pc0=1;//�q�Hoff
		_pc6=0;//�s�N�}��on
		_pg0=0;//�s�N����on
		
		if(r2!=1)
			r2=1;
		dd=0;
	}		 
	else if(s=='0')//����
	{
		_pc0=1;//�q�Hoff
		_pc6=1;//�s�N�}��off
		_pg0=1;//�s�N����off
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
	
	//timer���_  
	_emi=1;
	_mf0e=1;
	_mfi0=0b00000001;
	_tm0c0=0b00001000;
	
	_ifs0=0b00001000;		//�@�αH�s��INT1=PE1
	_integ=0b00001000; 		//�t�tĲ�o
	_intc0=0b00000101;
	_pd7=1;
	while(1)
	{
		LED();
	}
} 