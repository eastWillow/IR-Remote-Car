/*
TA7267
IN1 IN2 OUT1 OUT2
 1   1    L    L
 0   1    L    H backward
 1   0    H    L forward
 0   0    Z    Z
*/
/*
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
sbit SERVOMOTOR1=P1^3; //PWM I/O
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
unsigned int servoMotorHighTime = 15000;
void Delay100us();
void setup();
void main(){
	/*setup();
	TR0 = 1;// start Timer 0*/
	while(1){
	  unsigned int x,i;
    for( x = 0; x < 100; x++){
    SERVOMOTOR1 = 0;
    for ( i = 0;i < 100;i++) Delay100us();
    SERVOMOTOR1 = 1;
    for ( i = 0;i < 15;i++) Delay100us();
    }
	}
}
void setup(){
	EA = 1;
	ET0 = 1;
	TR0 = 0; //reset Timer0 Switch
	TMOD = 0x01; //0000 0010
	TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
	TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
	SERVOMOTOR1 = 0;
}
void servoMotor () interrupt 1 {
	TR0 = 0;
	if(SERVOMOTOR1){
		TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
		TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 0;
		Delay1ms();
	}
	else{
		TL0 = (FULL - servoMotorHighTime) %256;
		TH0 = (FULL - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 1;
		Delay1ms();
	}
	TR0 = 1;
}
void Delay100us()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 38;
	do
	{
		while (--j);
	} while (--i);
}
