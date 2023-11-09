 /*
 *ESIME Unidad Azcapotzalco
 *PRACTICA 6 y 7
 *ARELLANO FLORES RODRIGO EMMANUEL
 *MARTINEZ FAJARDO RODRIGO
 *MONTIEL ZUÑIGA EMMANUEL ESAÚ
 *IPP PROFESOR M. EN C. RAMÓN VALDÉS
 */ 
#include <avr/io.h>
#include <util/delay.h>
//#define F_CPU 8000000U
#define LECP6 (PINC & 0x0F) //Puerto C [2, 5] 4 bits de entrada
#define LCECP7 (PINC & 0X1F)//Puerto C [2, 6] 5 bits de entrada
#define AUX (PIND & 0x03)   //Puerto D [0, 1] ayuda en la relizacion de suma y reinicio de la misma
int SalDisp[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0xF7, 0xFF, 0x39, 0X3F, 0x79, 0X71};
void conv(int w);
int main(void){
    DDRB = 0XFF;    //Puerto B, (todo salida) a displays
    DDRC = 0x00;    //Puerto C seleccion [0,1], bits de entrada [2, 6]
    DDRD = 0xFC;    //Puerto D [0, 1] entrada
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    int valores[3] = {0, 0 , 0};        //Almacena valores para la suma y su resultado
    int paso[4] = {0, 0, 0x01, 0x02};
    while(1){
        conv(LECP6);
        while(PINC & (1<<6)){   //C6 Activa Practica 7
            for(int i = 0; i < 2; i++){
                do{
                    valores[i] = LCECP7;
                    conv(valores[i]);
                    if(AUX == paso[i+2]) paso[i]++; //Pulsar D0 para guardar un valor, D1 para guardar el siguiente valor
                }while(paso[i] == 0);
                paso[i] = 0;
            }
            valores[2] = valores [0] + valores[1];
            while(paso[0] == 0){
                conv(valores[2]);      //Muestra resulatdo y permite variar la decodificacion del mismo
                if(AUX == 0x03) paso[0]++;  //D0 y D1 para reininicar suma, soltar D0 y despues D1
            }
            paso[0] = 0;
        }
    }
}
void conv(int w){        //Funcion de conversion y muestra en display
    int d = 0, u = 0, c = 10;   //Decodificacion decimal por defecto
    if (PINC & (1<<5)) c = 16; //C5 para cambiar decodificacion a hexadecimal
    d = (int)w/c;
    u = w%c;
    PORTB = SalDisp[u] | (1<<7);
    _delay_ms(10);
    PORTB = SalDisp[d];
    _delay_ms(10);
}