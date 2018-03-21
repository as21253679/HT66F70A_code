#include <HT66F70A.h>
#include "MyType.H"

#define PE1_Rx  _pe1
void PC_TX(char da);
void tx_string(char data[])	;
void UART_init();
void delay(double del);
//以下為I2C的部分
int RxFlag;//接收期標
int DeviceFlag;//裝置期標
#define	SCLC _pac5		//DEFINE SCL CONTROL BIT,PAC5
#define	SDAC _pac4		//DEFINE SDA CONTROL BIT,PAC4
/*efine	SCL  _pa5	                //DEFINE SCL SIGNAL PIN,PA5
#define	SDA  _pa4		        //DEFINE SCL SIGNAL PIN,PA4*/
#define	Voltage 0x09

unsigned char RxData;//接收資料
#define I2CSCK _pa5
#define I2CSDA _pa4
#define I2cCtrl _pbc
void i2cStart();
void i2cStop();
void i2cRestart();
void i2cSendHeight();
void i2cSendLow();
void i2cAck();
void i2cNack();
void i2cSendData(unsigned char i2cData);
unsigned char i2cGetData();
void i2cWrite(unsigned char i2cAddress , unsigned char i2cRegister , unsigned char i2cData);
unsigned char i2cRead(unsigned char i2cAddress , unsigned char i2cRegister);

//以上為I2C的部分

extern char output[];
char output1[4]={'0','1'};

void Delayms(u16 del)
{	
	u16 i;									//@fSYS=8MH,延遲del*1ms
	for(i=0;i<del;i++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期
}
void GPIO_init()
{
	_pcpu1 = 1;		//  PC1 設定提升電阻
	_pcc1 = 1;		//	PC1 設定輸入
}

void main()

{
	
_Bool stop = 0;
_wdtc=0b10101111;						//關閉看門狗計時器  
UART_init();
	
_sima=0b11000000;
RxFlag=0;


 while(1)
   { 
   	//	i2cSendData((0x16 << 1 | 0x00));
     i2cWrite(0x38,0x70,0b00101111);
    // RxData=i2cRead(0x38,0x71);
     
     
     PC_TX(0x0d);
     
     RxData=i2cRead(0x38,0x71);
     
     PC_TX(RxData);
     PC_TX('d');
     
     RxData=i2cRead(0x39,0x73);
     
     PC_TX(RxData);
     PC_TX('f');
  
   }
   
 	
}

/*void  delay(void)
{
	_nop();
	_nop();_nop();_nop();_nop();
	_nop();	_nop();_nop();_nop();
	_nop();	_nop();_nop();_nop();
	_nop();	_nop();_nop();_nop();
}*/
void delay(double del)
{	
	double j;					        //@fSYS=8MH,延遲del*1ms
	for(j=0;j<del;j++) GCC_DELAY(2000);		//內建函式，延遲2000指令週期
}
void i2cStart()
{ SCLC=0;
  SDAC=0;
	/*I2CSCK = 1;
	delay(0.001);
	I2CSDA = 1;
    delay(0.001);
	I2CSDA = 0;
    delay(0.001);
	I2CSCK = 0;
    delay(0.001);*/
    
    	I2CSDA = 1;
	delay(0.001);
	I2CSCK = 1;
	delay(0.001);
	I2CSDA = 0;
	delay(0.001);
	I2CSCK = 0;
	delay(0.001);
	
}

void i2cStop()
{
	SDAC = 0;	// set SDA as output
	SCLC = 0;	// set SCL as output
	
	/*I2CSCK = 1;
    delay(0.001);	
	I2CSDA = 0;
	delay(0.001);
	I2CSDA = 1;
    delay(0.001);*/
    
    I2CSDA = 0;
    I2CSCK = 0;
	delay(0.001);
	I2CSCK = 1;
	delay(0.001);
	I2CSDA = 1;
	delay(0.001);
    
}

void i2cReStart()
{
	i2cStart();
}

void i2cSendHeight()//nack
{
	SCLC=0;
  SDAC=0;
  
	I2CSCK = 0;
	delay(0.001);
	I2CSDA = 1;//(i2cData & i2cmask)不等於0=資料線輸出高為元
	delay(0.001);
	I2CSCK = 1;
    delay(0.001);
	I2CSCK = 0;
	delay(0.001);
}

void i2cSendLow()//ack
{
  SCLC=0;
  SDAC=0;
  
	I2CSCK = 0;
	delay(0.001);
	I2CSDA = 0;//(i2cData & i2cmask)等於0= 資料線輸出低為元
    delay(0.001);
	I2CSCK = 1;
    delay(0.001);
	I2CSCK = 0;
	delay(0.001);
}

void i2cAck()
{
	i2cSendLow();
}

void i2cNack()
{
	i2cSendHeight();
}

void i2cSendData(unsigned char i2cData)
{
	unsigned char i2cmask = 0x80;
	SDAC = 0;	// set SDA as output
	SCLC = 0;
	do
	{
		//	I2CSDA=i2cData & i2cmask;
		((i2cData & i2cmask) != 0) ? i2cSendHeight() : i2cSendLow();
		i2cmask >>= 1;
	}
	while(i2cmask);
}

unsigned char i2cGetData()
{
	unsigned char i2cmask = 0x80;
	unsigned char i2creaddata = 0x00;
	_pac4=1;
	I2CSDA = 1;
	do
	{
		I2CSCK = 0;
		delay(0.001);
	
// 		i2creaddata = (I2CSDA == 1) ? i2creaddata | i2cmask : i2creaddata | 0x00;
		i2creaddata |= (I2CSDA == 1) ? i2creaddata | i2cmask : i2creaddata | 0x00;
		I2CSCK = 1;
	delay(0.001);
		i2cmask >>= 1;
	}	while(i2cmask);
	_pbc2=0;
	return i2creaddata;

}

void i2cWrite(unsigned char i2cAddress , unsigned char i2cRegister , unsigned char i2cData)
{
	i2cStart();
	i2cSendData((i2cAddress << 1 | 0x00));
	i2cAck();
	i2cSendData(i2cRegister);
	i2cAck();
	i2cSendData(i2cData);
	i2cAck();
	i2cStop();
}


unsigned char i2cRead(unsigned char i2cAddress , unsigned char i2cRegister)
{
	unsigned char i2creaddata = 0x00;
	SDAC = 0;	// set SDA as output
	SCLC = 0;	
	i2cStart();

	i2cSendData((i2cAddress << 1 | 0x00));
// 	i2cSendData(00111000);
	i2cAck();
	i2cSendData(i2cRegister);
	i2cAck();
	i2cReStart();
	i2cSendData((i2cAddress << 1 | 0x01));
// 	i2cSendData(00111001);
	i2cAck();
	i2creaddata = i2cGetData();
    i2cNack();
	i2cStop();
	return i2creaddata;
}
