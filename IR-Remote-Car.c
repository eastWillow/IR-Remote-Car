/*
TA7267
IN1 IN2 OUT1 OUT2
 1   1    L    L break
 0   1    L    H backward
 1   0    H    L forward
 0   0    Z    Z nothing
*/
/*
Wiring
	YELLOW
	BLACK
	RED
	YELLOW
	ORANGE
	BLUE
*/
#include <STC12C5A60S2.H>
#include <intrins.h>
#define ALL_WIDTH 20000 //20ms
#define FULL 65535
#define ADRESS 0xA5
#define IRHIGH 1844
#define IRLOW 524
sbit SERVOMOTOR1=P1^3; //PWM I/O
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
bit repeat;
unsigned int servoMotorHighTime = 1250;
void setup();
void Delay100ms();
unsigned char irReceiver();
void uartSend (char *string,char len,char EOL);
void UartInit(void);
void main(){
	char debug[1];
	setup();
	UartInit();
	while(1){
		uartSend("Wellcom",7,1);
		debug[0] = irReceiver();
		uartSend(debug,1,1);
		Delay100ms();
	}
}
void setup(){
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	PX0 = 1;
	TR0 = 0; //reset Timer0 Switch
	TR0 = 0; //reset Timer1 Switch
	TMOD = 0x11; //0010 0010
	TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
	TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
	SERVOMOTOR1 = 0;
	TR0 = 1;
}
void servoMotor () interrupt 1 {
	TR0 = 0;
	if(SERVOMOTOR1){
		TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
		TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 0;
	}
	else{
		TL0 = (FULL - servoMotorHighTime) %256;
		TH0 = (FULL - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 1;
	}
	TR0 = 1;
}
void limitSwitch () interrupt 0{
	IN1 =0;
	IN2 =1;
	Delay100ms();
	Delay100ms();
	Delay100ms();
	Delay100ms();
	Delay100ms();
}
void Delay100ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 144;
	k = 71;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
unsigned char irReceiver(){
	unsigned char adress;
	unsigned char direction;
	unsigned char counter;
	unsigned char timeCounter;
	while(!IR_RECEIVER) timeCounter++;
	if(timeCounter > 8400){
		while(IR_RECEIVER);
		while(!IR_RECEIVER);
	}
	//starting
	for(counter = 0;counter <8;counter++){
		while(IR_RECEIVER) timeCounter++;
		if(timeCounter > IRHIGH) adress | (0x01 << counter);
		else if(timeCounter > IRLOW) adress | (0x00 << counter);
		while(!IR_RECEIVER);
	}
	for(counter = 0;counter <8;counter++){
		while(IR_RECEIVER) timeCounter++;
		if(timeCounter > IRHIGH) direction | (0x01 << counter);
		else if(timeCounter > IRLOW) direction | (0x00 << counter);
		while(!IR_RECEIVER);
	}
	while(!IR_RECEIVER);
	if(adress == ADRESS) return direction;
}
void UartInit(void)		//9600bps@12.000MHz
{
	PCON &= 0x7F;		//Baudrate no doubled
	SCON = 0x50;		//8bit and variable baudrate
	AUXR |= 0x04;		//BRT's clock is Fosc (1T)
	BRT = 0xD9;		//Set BRT's reload value
	AUXR |= 0x01;		//Use BRT as baudrate generator
	AUXR |= 0x10;		//BRT running
}
void uartSend (char *string,char len,char EOL){
	int i;
	for(i=0;i<len;i++){
		SBUF = string[i];
		while(!TI);
		TI=0;
	}
	if(EOL == 1){
		SBUF = 0x0D;
		while(!TI);
		TI=0;
		SBUF = 0x0A;
		while(!TI);
		TI=0;
	}
}