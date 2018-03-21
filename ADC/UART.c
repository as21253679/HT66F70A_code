// PROGRAM	: 4-9.c								2013.1026
// FUNCTION	: ADC With Interrupt Demo Program	By Steven

#include <HT66F70A.h>
//#include "4-9.h"	
#define LED_Port  _pc								
#define LED_PortC _pcc	
typedef	unsigned char	u8;
typedef	char			s8;
typedef	unsigned short	u16;
typedef	short			s16;
typedef	unsigned long	u32;
typedef	long			s32;
void PC_TX(char da);
char PC_RX();
void UART_init();

extern char output[];
char output1[4]={'0','1'};
#define T2 205  //baudrate  9600=4M/4/T => T=104
#define PE1_Rx  _pe1
#define PE0_Tx  _pe0
char output[50];
void rx_string();
void tx_string(char data[]);
void GPIO_init();

void Delay100us(u16);							//函式原型宣告
char e[4];
void main()
{
	_Bool stop = 0;
	UART_init();
	GPIO_init();
	_wdtc=0b10101111;							//關閉看們狗計時器
	LED_PortC=0;	      						//設定LED_Port為輸出模式	   
								//選擇AN1做轉換,設定轉換結果存放格式
	//_adcr0=0b00000001;
	//_adcr1=0b00000110;							//A/D Clock=fSYS/64,VREFS=0
							//設定PA1為AN1 A/D通道
	//_pas0=0b00110000;
	_ade=1;
	 _emi=1;		//啟動ADC中斷與中斷總開關 
//_start=1; 
//			_start=0;
	int i=0	;					
	
	char kk;
	while(1)
	{	
		kk=PC_RX();
		if(kk=='X')
		{
			_adcr0=0b00000001;
			_adcr1=0b00000110;
			_pas0=0b00110000;
			_ade=1;
			_start=1; 
			_start=0;
		//	PC_TX(e[3]);
		//	Delay100us(1000);
		//	PC_TX(e[2]);
		//	Delay100us(1000);
			PC_TX(e[1]);
			Delay100us(1000);
			PC_TX(e[0]);
			PC_TX('A');

		Delay100us(1000);
		}	
		else if(kk=='C') 
		{
			_adcr0=0b00000011;
			_adcr1=0b00000110;
			_pas1=0b00110000;
			_ade=1;
			_start=1; 
			_start=0;
		//	PC_TX(e[3]);
		//	Delay100us(1000);
		//	PC_TX(e[2]);
		//	Delay100us(1000);
			PC_TX(e[1]);
			Delay100us(1000);
			PC_TX(e[0]);
			PC_TX('A');

		Delay100us(1000);
		}
		else
		{
			PC_TX('A');
			Delay100us(1000);
		}
		
	}
}
DEFINE_ISR(ISR_ADC,0x1C)
{		int d=0,dd=0,ddd=0;
	_start=1; 
			_start=0;						//起始ADC轉換
			Delay100us(1000);
			
			/*d=~_adrh;
			d=0-(d<<4);
			dd=~_adrl;
			dd=0-(dd>>4);
			ddd=d+dd;
			
			e[3]=ddd/1000+48;
			ddd=ddd-(ddd/1000)*1000;
			e[2]=ddd/100+48;
			ddd=ddd-(ddd/100)*100;
			e[1]=ddd/10+48;
			ddd=ddd-(ddd/10)*10;
			e[0]=ddd+48;
			*/
			
			d=~_adrh<<4;
			
			dd=~_adrl>>4;
			
			ddd=d+dd;
			ddd=-(ddd/4);
			e[3]=ddd/1000+48;
			ddd=ddd-(ddd/1000)*1000;
			e[2]=ddd/100+48;
			ddd=ddd-(ddd/100)*100;
			e[1]=ddd/10+48;
			ddd=ddd-(ddd/10)*10;
			e[0]=ddd+48;
			
			_ade=0;
}
void Delay100us(u16 del)						//延遲del*200指令週期
{	u16 i,j;									//@fSYS=8MH,延遲del*100us
	for(i=0;i<del;i++)
		for(j=0;j<=25;j++) GCC_NOP();
}




void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期
}
void GPIO_init()
{
	//_pcpu1 = 1;		//  PC1 設定提升電阻
	//_pcc1 = 1;		//	PC1 設定輸入

}

void UART_init()
{
	_pec0 = 0;	
	_pec1 = 1;		//UART腳位方向規劃		0=OUT 1=IN
}

char PC_RX()//baudrate=9600  PE1
{
	u8 i=0,buff=0;   //資料計數=0，從頭開始
	while(PE1_Rx);    //等待接收啟始位元
	GCC_DELAY(T2);     //啟始位元等待1-bit時間
	GCC_DELAY(T2/2);  //啟始位元再等待1/2-bit時間令資料線上信號較穩定 
	for(i=0; i<8; i++)  //接收8-bit串列資料
	{
		buff=buff >> 1;  //資料緩衝右移1個bit
		if(PE1_Rx)  buff=buff | 0x80;//若接收的bit=1，則令資料的bit7=1
		GCC_DELAY(T2-15);  //等待一段時間，接收下一個bit
	}
	return buff;
}
void rx_string()
{
	char w=0;
	for(w=0;w<10;w++)
	{
		output[w] = (char)(PC_RX() & 0x7f);
		if(output[w]==13) break;
	}
}
void PC_TX(char da)//baudrate=9600  PE0
{
	u8 i=0;   //資料計數=0，從頭開始
	char buff=da; //讀取列表資料，同時資料計數加一
	PE0_Tx=0; GCC_DELAY(T2);   //發射啟始位元     
	for(i=0; i<8; i++)    //發射8-bit串列資料
	{
		if(buff & 0x01) PE0_Tx=1; //若buff資料bit0=1則發射1
		else  PE0_Tx=0;         //若buff資料bit0=0則發射0
		buff >>= 1;           //資料右移換下1個bit
		GCC_DELAY(T2-15);           //延時
	}
	PE0_Tx=1; GCC_DELAY(T2);      //發射停止位元
}
void tx_string(char data[])	
{
	char test=0;
	for(test=0;test<=9;test++)
	{
		PC_TX(data[test]);
	}
}