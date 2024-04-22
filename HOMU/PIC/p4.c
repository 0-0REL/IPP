/*
 * File:   p4.c
 * Author: rodri
 *
 * Created on 22 de abril de 2024, 12:07 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
unsigned char invertirByte(unsigned char byte);
void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISC = 0xFF;   //Entradas C
    TRISD = 0X00;   //Salidas D
    PORTC = 0;
    PORTD = 0;
    unsigned char w = 0;
    while(1){
        w = PORTC;  //lee puerto c
        unsigned char byteInvertido = invertirByte(w);
        PORTD = byteInvertido;  //MAnda a puerto D
    }
    return;
}
unsigned char invertirByte(unsigned char byte) {
    unsigned char resultado = 0;

    // Iterar sobre cada bit del byte original
    for (int i = 0; i < 8; i++) {
        // Desplazar el bit del byte original hacia la izquierda y
        // obtener el bit menos significativo
        unsigned char bit = (byte >> i) & 0x01;
        // Insertar el bit en el byte resultado, pero en la posición
        // opuesta al bit original
        resultado |= (bit << (7 - i));
    }

    return resultado;
}
