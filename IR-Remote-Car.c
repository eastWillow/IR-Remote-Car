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
#include <..\UD_Lib\8051\UART.h>
#define ALL_WIDTH 20000 //20ms
#define CENTER 10000 //10ms
#define CLOCK_LIMITER 5000 //5ms
#define COUNTER_CLOCKWISE_LIMITER 150000 //15ms
#define FULL 65535
sbit SERVOMOTOR1=P1^3; //PWM I/O
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
unsigned int servoMotorLowTime = 15000;
unsigned int servoMotorHighTime = 5000;
void Delay1ms();
void setup();
void main(){
	setup();
	TR0 = 1;// start Timer 0
	while(1){
	}
}
void setup(){
	EA = 1;
	ET0 = 1;
	TR0 = 0; //reset Timer0 Switch
	TMOD = 0x01; //0000 0010
	TL0 = servoMotorLowTime & 0xff;
	TH0 = servoMotorLowTime >> 8;
	SERVOMOTOR1 = 0;
}
void servoMotor () interrupt 1 {
	TR0 = 0;
	if(SERVOMOTOR1){
		TL0 = (FULL - servoMotorLowTime) & 0xff;
		TH0 = (FULL - servoMotorLowTime) >> 8;
		SERVOMOTOR1 = 0;
	}
	else{
		TL0 = (FULL - servoMotorHighTime) & 0x0f;
		TH0 = (FULL - servoMotorHighTime) >> 8;
		SERVOMOTOR1 = 1;
	}
	TR0 = 1;
}