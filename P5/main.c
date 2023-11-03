/*
 * P5 ATMEGA.c
 *
 * Created: 17/10/2023 08:53:02 a. m.
 * Author : rodri
 */ 

#include <avr/io.h>
int w=0;
int enc=0, ord=0;
int giro(int dir);
int lec();

int main(void){
    DDRB=0xC0; //ARRANQUE B0, PARO B5, SENSORES [B1, B4], MOTOR [B6, B7]
	PORTB=0x00;
    while (1){
		w=PINB & 0x3F;
		if(w==0x01){
			enc=1;
			ord=1;
		}
		while(enc==1){
			switch(ord){
				case 1:
					do{
						giro(1);
						if(w==0x08) ord=2;
					}while(ord==1);
					break;
				case 2:
					do{
						giro(0);
						if(w==0x02) ord=3;
					}while(ord==2);
					break;
				case 3:
					do{
						giro(1);
						if(w==0x10) ord=4;
					}while(ord==3);
					break;
				case 4:
					do{
						giro(0);
						if(w==0x04) ord=1;
					}while(ord==4);
					break;
			}
		}
    }
}
int giro(int dir){
	PORTB = dir==1? 0x40 : 0x80;
	w=PINB & 0x3F;
	if(w==0x20){
		PORTB=0x00;
		enc=0;
		break;
	}
	return enc, w;
}
