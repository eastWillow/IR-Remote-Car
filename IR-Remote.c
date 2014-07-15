/*
UP 		--- P1^2
DOWN 	--- P3^5
RIGHT --- P1^1
LEFT 	--- P0^6
*/
#include <STC12C5A60S2.H>
#include <intrins.h>
#define FULL 65535
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
	CCAP0H = 0;
	CCAPM0 = 0x42;
	TMOD = 0x01;
	TL0 = 0;
	TH0 = 0;
	TR0 = 0;
	TF0 = 0;
}
void main(){
	unsigned char direction = 0;//using 8bit to be a arrary and save direction
	steup();
	while(1){
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
	direction = 0;
	if(UP == 0){
		direction = 0x01;
	}
	delay(1000);
	if(DOWN == 0){
		direction = direction | 0x02;
	}
	delay(1000);
	if(LEFT == 0){
		direction = direction | 0x04;
	}
	delay(1000);
	if(RIGHT == 0){
		direction = direction | 0x08;
	}
	delay(1000);
	if(!UP || !DOWN || !LEFT || !RIGHT){
		IR_TR(direction);
		direction = 0;
		}
		delay(1000);
	}
}
void IR_TR(unsigned char direction){
	unsigned char i=0;
	bit j;
	CCAP0L = 128;
	CCAP0H = 128;
	CR = 1;
	delay(16800);
	CR = 0;
	delay(8400);
	for(i=0;i<8;i++){
		j = (_cror_(direction,i)&0x01);
		if(j == 1){
			CR = 1;
			delay(1844);
			CR = 0;
			delay(500);
		}
		if(j == 0){
			CR = 1;
			delay(524);
			CR = 0;
			delay(500);
		}
	}
	CR = 1;
	delay(526);
	CR = 0;
	delay(20000);
	CCAP0L = 0;
	CCAP0H = 0;
}
void delay(unsigned int time){
	TL0 = (FULL - time) %256;
	TH0 = (FULL - time) /256;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}