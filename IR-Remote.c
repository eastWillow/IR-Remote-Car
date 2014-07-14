/*
UP 		--- INT0,P1^2
DOWN 	--- INT0,P3^5
RIGHT --- INT0,P1^1
LEFT 	--- INT0,P0^6
*/
#include <STC12C5A60S2.H>
#include <intrins.h>
sbit IR = P1^3;
sbit UP = P1^2;
sbit DOWN = P3^5;
sbit RIGHT = P1^1;
sbit LEFT = P0^6;
void IR_TR(unsigned char DIRECTION);
void delay(unsigned int time);
void steup(){
	CMOD = 0x02;
	CR = 0;
	CCAPM0 = 0x42;
	CCAP0L = 128;
	CCAP0H = 128;
	
	EA = 1;
	EX0 = 1;
	
	TMOD = 0x01;
	TL0 = 0;
	TH0 = 0;
	TR0 = 0;
	TF0 = 0;
	
	PCON = 0x02;//Power Down
}
void main(){
	steup();
	while(1){
	}
}
void selectDirection () interrupt 0{
	PCON = 0x00;
	if(!DOWN){
		IR_TR(1);
		PCON = 0x02;
	}
	if(!UP){
		IR_TR(2);
		PCON = 0x02;
	}
	if(!LEFT){
		IR_TR(3);
		PCON = 0x02;
	}
	if(!RIGHT){
		IR_TR(4);
		PCON = 0x02;
	}
}
void IR_TR(unsigned char DIRECTION){
	switch(DIRECTION){
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	}
}
void delay(unsigned int time){
	TL0 = time %256;
	TH0 = time /256;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}