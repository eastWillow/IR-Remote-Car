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
/*
	direction:
	0x01: //UP
	0x05: //UP+LEFT
	0x09: //UP+RIGHT
	0x02: //DOWN
	0x06: //DOWN+RIGHT
	0x0A: //DOWN+RIGHT
	0x04: //LEFT
	0x08: //RIGHT
*/
#include <STC12C5A60S2.H>
#include <intrins.h>
#define ALL_WIDTH 20000 //20ms
#define FULL 65535
#define ADRESS 0xA5
#define IRHIGH 1844
#define IRLOW 524
#define HEAD 8400
sbit SERVOMOTOR1=P1^3; //PWM I/O
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
bit IrFlag;
unsigned char data IRdirection=0x00;
unsigned int data servoMotorHighTime = 1250;//volatile 
void setup();
void UartInit (void);
void uartSend (unsigned char number);
void main(){
	setup();
	UartInit();
	while(1){
			uartSend(IRdirection);
			switch(IRdirection){
			case 0x01:
				IN1 = 1;
				IN2 = 0;
				servoMotorHighTime = 1250;
				break;
			case 0x02:
				IN1 = 0;
				IN2 = 1;
				servoMotorHighTime = 1250;
				break;
			case 0x04:
				servoMotorHighTime = 1050;
				break;
			case 0x05:
				IN1 = 1;
				IN2 = 0;
				servoMotorHighTime = 1050;
				break;
			case 0x06:
				IN1 = 0;
				IN2 = 1;
				servoMotorHighTime = 1050;
				break;
			case 0x08:
				servoMotorHighTime = 1400;
				break;
			case 0x09:
				IN1 = 1;
				IN2 = 0;
				servoMotorHighTime = 1400;
				break;
			case 0x0A:
				IN1 = 0;
				IN2 = 1;
				servoMotorHighTime = 1400;
				break;
			case 0x00:
			case 0x03:
				IN1 = 1;
				IN2 = 1;
			break;
		}
	}
}
void setup(){
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	EX1 = 1;
	PX1 = 1;
	TR0 = 0; //reset Timer0 Switch
	TR0 = 0; //reset Timer1 Switch
	TMOD = 0x11; //0010 0010
	TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
	TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
	SERVOMOTOR1 = 0;
	TR0 = 1;
}
void servoMotor ()interrupt 1 {
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
	IN1 =1;
	IN2 =1;
}
void irReceiver()interrupt 2{
	unsigned int i;
	unsigned int counter;
	while(IR_RECEIVER == 0) counter++;
	if(counter <15656) return;
	if(counter >=15656){
		counter =0;
		while(IR_RECEIVER == 1)counter++;
		if(counter >= 3000){
			IRdirection = 0;
			for(i=0;i<8;i++){
				counter = 0;
				while(IR_RECEIVER == 0)counter++;
				if(counter >1000) IRdirection = IRdirection | _crol_(0x01,i);
				while(IR_RECEIVER == 1);
			}
		}
	}
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
void uartSend (unsigned char number){
		SBUF = number;
		while(!TI);
		TI=0;
}