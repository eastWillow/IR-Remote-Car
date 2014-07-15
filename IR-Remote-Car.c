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
<<<<<<< HEAD
unsigned char IRdirection;
unsigned int servoMotorHighTime = 1250;
void setup();
void Delay100ms();
=======
unsigned int servoMotorHighTime;
unsigned char IRdirection;
void setup();
>>>>>>> parent of d4569ae... It is Lag And Wired
void UartInit (void);
void uartSend (unsigned char number);
void main(){
	setup();
	UartInit();
	while(1){
<<<<<<< HEAD
		if(IRdirection == 0x04) servoMotorHighTime = 1000;
		if(IRdirection == 0x04) servoMotorHighTime = 1500;
	}
=======
		switch(IRdirection){
			case 0x01:
				IN1 = 1;
				IN2 = 0;
				break;
			case 0x02:
				IN1 = 0;
				IN2 = 1;
				break;
			case 0x04:
				TR0 = 0;
				servoMotorHighTime = 1000;
				break;
			case 0x05:
				break;
			case 0x06:
				break;
			case 0x08:
				TR0 = 0;
				servoMotorHighTime = 1500;
				break;
			case 0x09:
				break;
			case 0x0A:
				break;
			default:
				TR0 = 0;
				servoMotorHighTime = 1250;
				IN1 = 0;
				IN2 = 0;
				break;
		}
		TR0 = 1;
		}
>>>>>>> parent of d4569ae... It is Lag And Wired
}
void setup(){
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	EX1 = 1;
	PX0 = 1;
<<<<<<< HEAD
	TR0 = 0; //reset Timer0 Switch
	TR0 = 0; //reset Timer1 Switch
	TMOD = 0x11; //0010 0010
	TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
	TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
	SERVOMOTOR1 = 0;
	TR0 = 1;
}
void servoMotor () interrupt 1 {
=======
	PX1 = 1;
	IPH = 0x30;
	IT1 = 1;
	TR0 = 0; //reset Timer0 Switch
	TMOD = 0x01; //0010 0010
	SERVOMOTOR1 = 0;
}
void servoMotor ()interrupt 1 {
>>>>>>> parent of d4569ae... It is Lag And Wired
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
<<<<<<< HEAD
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
void irReceiver() interrupt 2{
	unsigned int counter;
	while(IR_RECEIVER == 0) counter++;
	if(counter >> 8 == 0x3D){
		uartSend(counter >> 8);
		uartSend(counter);
	}
	/*unsigned int counter;
	unsigned char adress;
	int i;
	IRdirection = 0;
	while(IR_RECEIVER == 0) counter++;
	uartSend(counter);
	if (counter >= HEAD){
		counter = 0;
		while(IR_RECEIVER == 1);
		while(IR_RECEIVER == 0);
		while(IR_RECEIVER == 1) counter++;
	}
	else{
		counter = 0;
		while(IR_RECEIVER == 1) counter++;
	}
	//Start Coding
	if (counter >= IRHIGH) adress = adress | 0x01;
	for(i=0;i<7;i++){
		while(IR_RECEIVER == 0);
		counter = 0;
		while(IR_RECEIVER == 1) counter++;
		if (counter >= IRHIGH) adress = adress | (0x02 << i);
	}
	for(i=0;i<8;i++){
		while(IR_RECEIVER == 0);
		counter = 0;
		while(IR_RECEIVER == 1)	counter++;
		if (counter >= IRHIGH) IRdirection = IRdirection | (0x01 << i);
	}*/
=======
}
void irReceiver()interrupt 2{
	unsigned int counter;
	unsigned char i;
	while(IR_RECEIVER == 0) counter++;
	if(counter >=15656){
		counter =0;
		while(IR_RECEIVER == 1)counter++;
		if(counter >= 3000){
			for(i=0;i<8;i++){
				counter = 0;
				while(IR_RECEIVER == 0)counter++;
				if(counter >1844) IRdirection = IRdirection | _crol_(0x01,i);
				while(IR_RECEIVER == 1);
			}
		}
	}
>>>>>>> parent of d4569ae... It is Lag And Wired
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