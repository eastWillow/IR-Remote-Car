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
#define IRHIGH 
#define IRLOW IRLOW
sbit SERVOMOTOR1=P1^3; //PWM I/O
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
bit repeat;
unsigned int servoMotorHighTime = 1250;
void setup();
void Delay100ms();
void main(){
	unsigned int TimePass;
	unsigned char adress;
	unsigned char direction;
	unsigned char counter;
	setup();
	while(1){
		if(!IR_RECEIVER){
			TH1 = 0;
			TL1 = 0;
			TR1 = 1;
			while(!IR_RECEIVER) TimePass = TL1 | (TH1 << 8);
			if(TimePass > 8400){
				TR1 = 0;
				TH1 = 0;
				TL1 = 0;
				TR1 = 1;
				while(IR_RECEIVER) TimePass = TL1 | (TH1 << 8);
				if(TimePass > 4200){
					repeat = 0;
				}
			}
			else{
				TR1 = 0;
				TH1 = 0;
				TL1 = 0;
				repeat = 1;
			}
			if(!repeat){
				for(counter = 0;counter < 8;counter++){
					while(!IR_RECEIVER);
					TR1 = 1;
					while(IR_RECEIVER) TimePass = TL1 | (TH1 << 8);
					TR1 = 0;
					TL1 = 0;
					TH1 = 0;
					if(TimePass > IRHIGH) adress | (1 << counter);
					else if (TimePass > IRLOW) adress | (0 << counter);
				}
			}
			for(counter = 0;counter < 8;counter++){
					while(!IR_RECEIVER);
					TR1 = 1;
					while(IR_RECEIVER) TimePass = TL1 | (TH1 << 8);
					TR1 = 0;
					TL1 = 0;
					TH1 = 0;
					if(TimePass > IRHIGH) direction | (1 << counter);
					else if (TimePass > IRLOW) direction | (0 << counter);
			}
		}
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
