#include <HT66F70A.H>
#include <HT66F70A.h>
#include "MyType.H"

#define T3 105
#define T2 212  //212 199
#define PE1_Rx  _pe1
#define PE0_Tx  _pe0
#define PE2_Rx  _pe2
#define PE3_Tx  _pe3

char output[50];

char PC_RX();
void rx_string();
void PC_TX(char da);
void tx_string(char data[],int len);
char PC_RX_115200();
void rx_string_115200();
void PC_TX_115200(char da);
void tx_string_115200(char data[],int c);

void UART_init()
{
	_pec0 = 0;	
	_pec1 = 1;		//UART�}���V�W��		0=OUT 1=IN
	_pec2 = 1;	
	_pec3 = 0;
	PE3_Tx=1;
}

char PC_RX()//baudrate=9600  PE1
{
	u8 i=0,j=0,buff=0;   //��ƭp��=0�A�q�Y�}�l
	while(PE1_Rx);
   //���ݱ����ҩl�줸
	GCC_DELAY(T2);     //�ҩl�줸����1-bit�ɶ�
	GCC_DELAY(T2/2);  //�ҩl�줸�A����1/2-bit�ɶ��O��ƽu�W�H����í�w
	for(i=0; i<8; i++)  //����8-bit��C���
	{
		buff=buff >> 1;  //��ƽw�ĥk��1��bit
		if(PE1_Rx)  buff=buff | 0x80;//�Y������bit=1�A�h�O��ƪ�bit7=1
		GCC_DELAY(T2-15);  //���ݤ@�q�ɶ��A�����U�@��bit
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
	u8 i=0;   //��ƭp��=0�A�q�Y�}�l
	char buff=da; //Ū���C���ơA�P�ɸ�ƭp�ƥ[�@
	PE0_Tx=0; GCC_DELAY(T2);   //�o�g�ҩl�줸     
	for(i=0; i<8; i++)    //�o�g8-bit��C���
	{
		if(buff & 0x01) PE0_Tx=1; //�Ybuff���bit0=1�h�o�g1
		else  PE0_Tx=0;         //�Ybuff���bit0=0�h�o�g0
		buff >>= 1;           //��ƥk�����U1��bit
		GCC_DELAY(T2-15);           //����
	}
	PE0_Tx=1; GCC_DELAY(T2);      //�o�g����줸
}

void tx_string(char data[],int len)	
{
	char test=0;
	for(test=0;test<len;test++)
	{
		PC_TX(data[test]);
	}
}

//115200

char PC_RX_115200()//baudrate=9600  PE1
{
	u8 i=0,j=0,buff=0;   //��ƭp��=0�A�q�Y�}�l
	while(PE2_Rx);
   //���ݱ����ҩl�줸
	GCC_DELAY(T3);     //�ҩl�줸����1-bit�ɶ�
	GCC_DELAY(T3/2);  //�ҩl�줸�A����1/2-bit�ɶ��O��ƽu�W�H����í�w
	for(i=0; i<8; i++)  //����8-bit��C���
	{
		buff=buff >> 1;  //��ƽw�ĥk��1��bit
		if(PE2_Rx)  buff=buff | 0x80;//�Y������bit=1�A�h�O��ƪ�bit7=1
		GCC_DELAY(T3-15);  //���ݤ@�q�ɶ��A�����U�@��bit
	}
	return buff;
}
void rx_string_115200()
{
	char w=0;
	for(w=0;w<10;w++)
	{
		output[w] = (char)(PC_RX_115200() & 0x7f);
		if(output[w]==13) break;
	}
}
void PC_TX_115200(char da)//baudrate=9600  PE0
{
	u8 i=0;   //��ƭp��=0�A�q�Y�}�l
	char buff=da; //Ū���C���ơA�P�ɸ�ƭp�ƥ[�@
	PE3_Tx=0; GCC_DELAY(T3);   //�o�g�ҩl�줸     
	for(i=0; i<8; i++)    //�o�g8-bit��C���
	{
		if(buff & 0x01) PE3_Tx=1; //�Ybuff���bit0=1�h�o�g1
		else  PE3_Tx=0;         //�Ybuff���bit0=0�h�o�g0
		buff >>= 1;           //��ƥk�����U1��bit
		GCC_DELAY(T3-15);           //����
	}
	PE3_Tx=1; GCC_DELAY(T3);      //�o�g����줸
}

void tx_string_115200(char data[],int c)	
{
	char test=0;
	for(test=0;test<c;test++)
	{
		PC_TX_115200(data[test]);
	}
}