/*
UP 		--- INT0,P1^2
DOWN 	--- INT0,P3^5
RIGHT --- INT0,P1^1
LEFT 	--- INT0,P0^6
*/
#include <STC12C5A60S2.H>
#include <intrins.h>
#define FULL 65535
#define ADRESS 0xA5
sbit IR = P1^3;
sbit UP = P1^2;
sbit DOWN = P3^5;
sbit RIGHT = P1^1;
sbit LEFT = P0^6;
unsigned char lastDirection;
void IR_TR(unsigned char direction);
void delay(unsigned int time);
void steup(){
	CMOD = 0x08;
	CR = 0;
	CCAP0L = 0;
	CCAP0H = 128;
	CCAPM0 = 0x42;
	
	EA = 1;
	EX0 = 1;
	
	TMOD = 0x01;
	TL0 = 0;
	TH0 = 0;
	TR0 = 0;
	TF0 = 0;
}
void main(){
	steup();
	while(1){
		CCAP0L = 0;
		PCON = 0x02;//Power Down
	}
}
void selectDirection () interrupt 0{
	unsigned char direction = 0;//using 8bit to be a arrary and save direction
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
	EA = 0;
	delay(1000);
	if(!UP){
		direction = 0x01;
	}
	if(!DOWN){
		direction = direction | 0x02;
	}
	if(!LEFT){
		direction = direction | 0x04;
	}
	if(!RIGHT){
		direction = direction | 0x08;
	}
	IR_TR(direction);
	CCAP0L = 0;
	EA = 1;
}
void IR_TR(unsigned char direction){
	unsigned char i=0;
	unsigned char Adress = ADRESS;
	CCAP0L = 128;
	if(lastDirection != direction){
		lastDirection = direction;
		CR = 1;
		delay(8400);
		CR = 0;
		delay(4200);
	}
	for(i=0;i<8;i++){
		if((Adress & 0x01)){
			CR = 1;
			delay(526);
			CR = 0;
			delay(1574);
		}
		else{
			CR = 1;
			delay(526);
			CR = 0;
			delay(524);
		}
		Adress = Adress >> 1;
	}
	for(i=0;i<8;i++){
		if((direction & 0x01)){
			CR = 1;
			delay(526);
			CR = 0;
			delay(1574);
		}
		else{
			CR = 1;
			delay(526);
			CR = 0;
			delay(524);
		}
		direction = direction >> 1;
	}
	CR = 1;
	delay(526);
	CR = 0;
	delay(20000);
}
void delay(unsigned int time){
	TL0 = (FULL - time) %256;
	TH0 = (FULL - time) /256;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}