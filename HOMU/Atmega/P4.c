#include <avr/io.h>
unsigned char invertirByte(unsigned char byte);
int main(void){
    unsigned char w = 0;
    DDRD = 0x00;    //Entrada D
    DDRB = 0xFF;    //Salida B
    PORTD = 0;
    PORTB = 0;
    while (1)
    {
        w = PIND;
        unsigned char byteInvertido = invertirByte(w);
        PORTB = byteInvertido;
    }
    
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