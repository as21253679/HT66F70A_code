#include <HT66F70A.h>
#include "MyType.H"

#define T2 205  //baudrate  9600=4M/4/T => T=104
#define PE1_Rx  _pe1
#define PE0_Tx  _pe0

char output[50];

char PC_RX();
void rx_string();
void PC_TX(char da);
void tx_string(char data[]);

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
	for(test=0;test<=13;test++)
	{
		PC_TX(data[test]);
	}
}