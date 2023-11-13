//#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <lcd_4pin.h>//libreria personalizada
#include <cov.h>//libreria personalizada
#define BP (PINB & (1<<5))	//Boton de paro B5
#define BA (PINB & (1<<0))	//Boton de arranque B0
#define CAMB (PINC & (1<<0))//Cambio de funciones C0
#define MSGS (PINC & (1<<5))//Cambio de mensajes C5
#define SEN (PINB & (0x0F<<1))//Solo sensores [B1, B4]
int paso = 0, g = 0, cn = 0;
int paro = 0, paAnt = 0, et = 0, p = 0, PSO = 1;
void giro(int dir, int sensor, int *paso, int *et, int *cn);//Indica la direccion del giro, derecha o izquierda
int main(){
    int cambio = 0, camAnt = 0;
    int msg = 0, msgAnt = 0, nMsg = 0;
    int bin = 0;
    char* msgss[] = {"7RM3   Equipo 5", "Hola prof       ", "Ponganos 10  :) "};//Mensajes
    DDRB = 0xC0;    //Arranque B0; Paro B5; Sensores [B1, B4]; MOTOR [B6, B7]
    DDRC = 0X1E;    //C0 Y C5 entradas; [C1, C4] salidas
    PORTB = 0x00;
    PORTC = 0x00;
    LCD_Init();     //Inicializacion de LCD
    while (1){
        msg = MSGS;//Lee boton de intercambio de mensajes
        if(msg == (1<<5) && msgAnt == 0){//Hace la funcion usar un boton como interrupor
            nMsg++;
            if(nMsg > 3) nMsg = 0;//Limita el numero de mensajes
            else if(nMsg == 3) LCD_String_xy(0,0,"                ");
        }
        msgAnt = msg;
        if(nMsg == 3){//Activa el contador
            LCD_String_xy(0,0,convb(bin));
            //_delay_ms(500);
            bin++;
            if(bin>255){
            bin = 0;
            }
        }
        else{
            LCD_String_xy(0,0,msgss[nMsg]);//Imprime los mensajes
        }
        cambio = CAMB;
        if (cambio == 1 && camAnt == 0){//Hace intercambio entre control de giro y servomecanismo
            g = 0;
            p++;
            if(p>3) p = 0;
        }
        camAnt = cambio;
        if (p == 0 || p == 2 || p == 3){//Funcion de control de giro y motor paso a paso
        TCCR0B = 0;//Apaga Contador mientras no se usa
            if(BA && BP){
                PORTB = 0x00;
                LCD_String_xy(1,0,"No pulsar ambos");
            }
            else if(BA){
                if(p == 0){     //Motor
                    LCD_String_xy(1,0,"Giro a la der   ");
                    PORTB = (1<<6);
                    g = 1;
                }
                else if(p == 2){    //Paso medio
                    LCD_String_xy(1,0,"Giro a la der   ");
                    PORTC = (1<<PSO);
                    _delay_ms(10);
                    PORTC = (0x03<<(PSO));
                    _delay_ms(10);
                    PSO++;
                    if(PSO > 4) PSO = 1;
                    g = 1;
                }
                else{           //Paso completo
                    LCD_String_xy(1,0,"Giro a la der   ");
                    PORTC = (1<<PSO++);
                    _delay_ms(10);
                    if(PSO > 4) PSO = 1;
                    g = 1;
                }
            }
            else if(BP){
                if(p == 0){     //Motor
                    LCD_String_xy(1,0,"Giro a la izq   ");
                    PORTB = (1<<7);
                    g = 1;
                }
                else if(p == 2){    //Medio paso
                    LCD_String_xy(1,0,"Giro a la izq   ");
                    PORTC = (1<<PSO);
                    _delay_ms(10);
                    PORTC = (0x03<<(PSO));
                    _delay_ms(10);
                    PSO--;
                    if(PSO < 1) PSO = 4;
                    g = 1;
                }
                else{               //Paso completo
                    LCD_String_xy(1,0,"Giro a la izq   ");
                    PORTC = (1<<PSO--);
                    _delay_ms(10);
                    if(PSO < 1) PSO = 4;
                    g = 1;
                }
            }
            else{
                PORTB = 0x00;
                if (g == 1) LCD_String_xy(1,0,"Alto            ");
                else if(p == 0) LCD_String_xy(1,0,"Control de giro");
                else if(p == 2)LCD_String_xy(1,0,"Paso-paso MedioP");
                else LCD_String_xy(1,0,"Paso Completo   ");
            }
        }
        else{   //Servo mecanismo
        TCCR0B = (1<<CS02) | (1<<CS00);//Prescaler 1024
        TCNT0 = 178;    //10ms
            if(g == 0 && p == 1){
                LCD_String_xy(1,0,"Servo mecanismo");
                g = 1;
            }
            if (paso == 0) if(BA) paso = 1;
            switch(paso){
                case 1:
                    LCD_String_xy(1,0,"Hacia S3        ");
                    giro(1, (1<<3), &paso, &et, &cn);
                    break;
                case 2:
                    if(et == 0){
                        PORTB = 0x00;
                        LCD_String_xy(1,0,"Llego a S3");
                        cn = 0;
                        et++;
                    }
                    if(TIFR0 & (1<<TOV0)){
                        cn++;
                        TCNT0 = 178;
                        TIFR0 |= (1<<TOV0);
                    }
                    if(cn == 300){  //3 segundos
                        LCD_String_xy(1,0,"Hacia S2  ");
                        giro(0, (1<<2), &paso, &et, &cn);
                    }
                    break;
                case 3:
                    if(et == 0){
                        PORTB = 0x00;
                        LCD_String_xy(1,0,"Llego a S2");
                        cn = 0;
                        et++;
                    }
                    if(TIFR0 & (1<<TOV0)){
                        cn++;
                        TCNT0 = 178;
                        TIFR0 |= (1<<TOV0);
                    }
                    if(cn == 500){//5 segundos
                        LCD_String_xy(1,0,"Hacia S1  ");
                        giro(0, (1<<1),&paso, &et, &cn);
                    }
                    break;
                case 4:
                    if(et == 0){
                        PORTB = 0x00;
                        LCD_String_xy(1,0,"Llego a S1");
                        cn = 0;
                        et++;
                    }
                    if(TIFR0 & (1<<TOV0)){
                        cn++;
                        TCNT0 = 178;
                        TIFR0 |= (1<<TOV0);
                    }
                    if(cn == 100){//1 segundo
                        LCD_String_xy(1,0,"Hacia S4  ");
                        giro(1, (1<<4),&paso, &et, &cn);
                    }
                    break;
                case 5: case 6:
                    if(paso == 6) LCD_String_xy(1,0,"Reinicio! -> S1");
                    else
                    LCD_String_xy(1,0,"Hacia S1  ");
                    giro(0, (1<<1),&paso, &et, &cn);
                    break;
            }
        }
    }
    return 0;
}
void giro(int dir, int sensor, int *paso, int *et, int *cn){
    int c = 0;
	PORTB = (dir == 1)? (1<<6) : (1<<7);	//Indica direccion de giro del motor
	if(BP){					//Pregunta por boton de paro
		PORTB = 0x00;
        while(1){				//Pausa
            paro = BP;
            if(paro == (1<<5) && paAnt == 0){
                if(c == 0) LCD_String_xy(1,0,"ALTO!           ");
                c++;
            }
            paAnt = paro;
            if(c == 2){
                *paso = 6;
                break;
            }
            if (BA && BP);
            else if(BA) break;
	    }
	}
    if (SEN == sensor && *paso == 5) {
        *paso = 1;
        *cn = 0;
        }
    else if (SEN == sensor && *paso == 6){
        *paso = 0;
        *cn = 0;
        PORTB = 0x00;
        LCD_String_xy(1,0,"                ");
        g = 0;
    }
    else if (SEN == sensor){
        *paso+=1;
        *et = 0;
        *cn = 0;
    }
}