#include <STC12C5A60S2.H>
/*
TA7267
IN1 IN2 OUT1 OUT2
 1   1    L    L
 0   1    L    H
 1   0    H    L
 0   0    Z    Z
*/
sbit SERVOMOTOR1=P1^0;
sbit LIMITSWITCH=P3^2; // INT0
sbit IR_RECEIVER=P3^3; // INT1
sbit IN1=P0^1;
sbit IN2=P0^2;
void main(){
	while(1){
		if(!LIMITSWITCH){
			IN1 = 0;
		  IN2 = 1;
		}
		else{
			IN1 = 1;
			IN2 = 0;
		}
	}
}