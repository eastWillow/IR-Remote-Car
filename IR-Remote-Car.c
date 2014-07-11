#include <STC12C5A60S2.H>
/*
TA7267
IN1 IN2 OUT1 OUT2
 1   1    L    L
 0   1    L    H
 1   0    H    L
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
unsigned int servoMotorHighTime = 1250;
void setup();
void main(){
	setup();
	TR0 = 1;
	while(1){
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
	}
	else{
		TL0 = (FULL - servoMotorHighTime) %256;
		TH0 = (FULL - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 1;
	}
	TR0 = 1;
}