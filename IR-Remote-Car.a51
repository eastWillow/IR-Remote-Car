SERVOMOTOR1 set P1.3
LIMITSWITCH set P3.2 
IR_RECEIVER set P3.3
IN1 set P0.1
IN2 set P0.2
IRdirection: DSB
servoMotorHighTime: DSW
org 0x00
jmp 0x50
org 0x50
setup:
mov sp,#0x80
setb EA
setb ET0
setb EX0
setb EX1
setb PX0
setb PX1
clr TR0 //reset Timer0 Switch
mov TMOD,#0x01; ;0000 0001
clr SERVOMOTOR1
loop:
mov A,IRdirection
ADD A,#0xfe
JZ case0x01
ADD A,#0xfd
JZ case0x02
ADD A,#0xfb
JZ case0x04
ADD A,#0xfa
JZ case0x05
ADD A,#0xf9
JZ case0x06
ADD A,#0xf7
JZ case0x08
ADD A,#0xf6s
JZ case0x09
ADD A,#0xf5
JZ case0x0A
case0x01:
setb IN1
clr IN2
case0x02:
clr IN1
setb IN2
case0x04:
clr TR0
case0x05:
setb IN1
clr IN2
case0x06:
setb IN1
clr IN2
case0x08:
setb IN1
clr IN2
case0x09:
setb IN1
clr IN2
case0x0A:
end