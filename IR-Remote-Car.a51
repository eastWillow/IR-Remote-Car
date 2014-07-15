SERVOMOTOR1 set P1.3
LIMITSWITCH set P3.2 
IR_RECEIVER set P3.3
IN1 set P0.1
IN2 set P0.2
org 0x00
jmp 0x50
org 0x50
setup :
setb EA
setb ET0
setb EX0
setb EX1
setb PX0
setb PX1
clr TR0 //reset Timer0 Switch
mov TMOD,#0x01; ;0000 0001
clr SERVOMOTOR1
end