#include <HT66F70A.h>
//#define LED_Port  _pc								
//#define LED_PortC _pcc	
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
#define PE1_Rx  _pe0
#define PE0_Tx  _pe1
char output[50];
void rx_string();
void tx_string(char data[]);
void GPIO_init();
void Delay100us(u16);							//�禡�쫬�ŧi
char e[2];
char kk;
void main()
{
	_Bool stop = 0;
	UART_init();
	_wdtc=0b10101111;							//�����ݭ̪��p�ɾ�
//	LED_PortC=0;	      						//�]�wLED_Port����X�Ҧ�	   
	_adcr0=0b00000011;							//���AN1���ഫ,�]�w�ഫ���G�s��榡
	_adcr1=0b00000110;							//A/D Clock=fSYS/64,VREFS=0
	_pas1=0b00110000;							//�]�wPA1��AN1 A/D�q�D
	_ade=1; _emi=1;		//�Ұ�ADC���_�P���_�`�}�� 
	_start=1; 
	_start=0;	
		
	while(1)
	{
	//	PC_TX('Q');
	//	kk=PC_RX();
	//	if(kk=='X')
		{
			_ade=1;
			PC_TX('W');
			Delay100us(1000);
			PC_TX(e[1]);
			Delay100us(1000);
			PC_TX(e[0]);
			Delay100us(1000);
			PC_TX('A');
			Delay100us(1000);
		}
//		else
		{
//			PC_TX('A');
//			Delay100us(1000);
		}
	}
}
DEFINE_ISR(ISR_ADC,0x1C)
{	
	int d=0,dd=0,ddd=0;
	_start=1; 
	_start=0;						//�_�lADC�ഫ
			
	d=~_adrh;
	d=0-(d<<2);
	dd=~_adrl;
	dd=0-(dd>>6);
	ddd=d+dd;
	e[1]=ddd/100+48;
	ddd=ddd-(ddd/100)*100;
	if(ddd>=100)
		e[0]=52;
	else
		e[0]=ddd/10+48;
	_ade=0;
}
void Delay100us(u16 del)						//����del*200���O�g��
{	u16 i,j;									//@fSYS=8MH,����del*100us
	for(i=0;i<del;i++)
		for(j=0;j<=25;j++) GCC_NOP();
}




void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,����del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//���ب禡�A����2000���O�g��
}
void GPIO_init()
{
	//_pcpu1 = 1;		//  PC1 �]�w���ɹq��
	//_pcc1 = 1;		//	PC1 �]�w��J

}

void UART_init()
{
	_pec1 = 0;	
	_pec0 = 1;		//UART�}���V�W��		0=OUT 1=IN
}

char PC_RX()//baudrate=9600  PE1
{
	u8 i=0,buff=0;   //��ƭp��=0�A�q�Y�}�l
	while(PE1_Rx);    //���ݱ����ҩl�줸
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
void tx_string(char data[])	
{
	char test=0;
	for(test=0;test<=8;test++)
	{
		PC_TX(data[test]);
	}
}