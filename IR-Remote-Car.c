#include <STC12C5A60S2.H>
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
unsigned int servoMotorHighTime = 1250;
void setup();
void Delay100ms();
void main(){
	int i;
	setup();
	TR0 = 1;
	while(1){
		IN1 =1;
		IN2 =0;
		if(servoMotorHighTime == 1000){
				IN1 =0;
				IN2 =0;
				Delay100ms();
			for(i=0;i<5;i++){
				IN1 =0;
				IN2 =1;
				Delay100ms();
			}
			servoMotorHighTime = 1250;
		}
	}
}
void setup(){
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	PX0 = 1;
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
void limitSwitch () interrupt 0{
		servoMotorHighTime = 1000;
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
