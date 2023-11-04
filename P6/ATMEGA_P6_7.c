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
#define SEL (PINC & 0x06)   //Puerto C [1, 2] para escoger entre decodificacion o suma y decodificacion
#define LECP6 (PINC & 0xF8) //Puerto C [3, 6] 4 bits de entrada
#define LCECP7 (PINC & 0XF8)//Puerto C [3, 7] 5 bits de entrada
#define AUX (PIND & 0x03)   //Puerto D [0, 1] ayuda en la relizacion de suma y reinicio de la misma
int SalDisp[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x6A, 0x7D, 0x7C, 0x47, 0x7F, 0x67, 0x3F, 0xFF, 0x39, 0X3F, 0x79, 0X71};
void conv(int w, int s);
int main(void){
    DDRB = 0XFF;    //Puerto B, (todo salida) a displays
    DDRC = 0x01;    //Puerto C seleccion [1,2], bits de entrada [3, 7]
    DDRD = 0xFC;    //Puerto D [0, 1] entrada
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    int valores[3] = {0, 0 , 0};        //Almacena valores para la suma y su resultado
    int paso[4] = {0, 0, 0x01, 0x02};
    while(1){
        switch(SEL){
            case 0x00: case 0x02:   //Decodificacion decimal por defecto C1 cambia a hexadecimal
            conv(LECP6, SEL);       //Convierte y muestra en display la entrada
            break;
            case 0x05: case 0x06:   //C2 activa la suma + tipo de decodificacion (C1 hexadecimal o decimal [por defecto])
            for(int i = 0; i < 2; i++){
                do{
                    valores[i] = LCECP7;
                    conv(valores[i], SEL);
                    if(AUX == paso[i+2]) paso[i]++; //Pulsar D0 para guardar un valor, D1 para guardar el siguiente valor
                }while(paso[i] == 0);
                paso[i] = 0;
            }
            valores[2] = valores [0] + valores[1];
            while(paso[0] == 0){
                conv(valores[2], SEL);      //Muestra resulatdo y permite variar la decodificacion del mismo
                if(AUX == 0X01) paso[0]++;  //D0 para reininicar suma
            }
            paso[0] = 0;
            break;
        }
    }
    return 0;
}
void conv(int w, int s){        //Funcion de conversion y muestra en display
    int d = 0, u = 0, c = 10;   //Decodificacion decimal por defecto
    if (s == 0x02 || s == 0x06) c = 16; //C1 para cambiar decodificacion a hexadecimal
    d = (int)w/c;
    u = w%c;
    PORTB = SalDisp[u];
    PORTB = (1<<PB7);
    _delay_us(15);
    PORTB = SalDisp[d];
    _delay_us(15);
}