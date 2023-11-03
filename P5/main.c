/*
 *Instituto Politécnico Nacional 
 *ESIME Unidad Azcapotzalco
 *PRACTICA 5 MECANISMO DE 4 SENSORES
 *ARELLANO FLORES RODRIGO EMMANUEL
 *MARTINEZ FAJARDO RODRIGO
 *MONTIEL ZUÑIGA EMMANUEL ESAÚ
 *IPP PROFESOR M. EN C. RAMÓN VALDÉS
 */ 
#include <avr/io.h>
#define BP (PINB & 0x20)	//Boton de paro
#define BA (PINB & 0x21)	//Boton de arranque
#define SEN (PINB & 0x1E)	//Solo sensores
int enc = 0, ord = 1;
int giro(int dir);			//Indica la direccion del giro, derecha o izquierda
int pp(void);				//Pone en pausa motor

int main(void){
    DDRB = 0xC0;			//Arranque B0; Paro B5; Sensores [B1, B4]; MOTOR [B6, B7]
	PORTB = 0x00;			//Limpia puerto B
    while (1){
		if(BA == 0x01){		//Inicia la secuencia (S3->S1->S4->S2->S3->S1->Repite)
			enc = 1;
		}
		while(enc == 1){
			while(ord == 1){
				giro(1);
				if(enc == 0) pp();
				if(SEN == 0x08) ord = 2;//S3 para siguiente paso
			}
			while(ord == 2){
				giro(0);
				if(enc == 0) pp();
				if(SEN == 0x02) ord = 3;//S1 para siguiente paso
			}
			while(ord == 3){
				giro(1);
				if(enc == 0) pp();
				if(SEN == 0x10) ord = 4;//S4 para siguiente paso
			}
			while(ord == 4){
				giro(0);
				if(enc == 0) pp();
				if(SEN == 0x04) ord = 5;//S2 para siguiente paso
			}
			while(ord == 5){
				giro(1);
				if(enc == 0) pp();
				if(SEN == 0x08) ord = 6;//S3 para siguiente paso
			}
			while(ord == 6){
				giro(0);
				if(enc == 0) pp();
				if(SEN == 0x02) ord = 1;//S1 para siguiente paso
			}
		}
    }
}
int giro(int dir){
	PORTB = dir == 1? 0x40 : 0x80;	//Indica direccion de giro del motor
	if(BP == 0x20){					//Pregunta por boton de paro
		PORTB = 0x00;
		enc = 0;
	}
	return enc;
}
int pp(void){
	while(enc == 0){				//Pausa
		PORTB = 0x00;
		if(BA == 0x01) enc = 1;
	}
	return enc;
}