#include <STC12C5A60S2.H>
#include <intrins.h>
sbit IR = P1^3;
void steup(){
	CMOD = 0x0a;
	CR = 0;
	CCAPM0 = 0x42;
	CCAP0L = 79;
	CCAP0H = 79;
}
void main(){
	steup();
	while(1){
		CR = 1;
	}
		
}