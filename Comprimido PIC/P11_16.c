/*
 * File:   P11_16.c
 * Author: rodri
 *
 * Created on 10 de diciembre de 2023, 05:37 AM
 */
// CONFIG1
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
#define _XTAL_FREQ 8000000
#define LCD_Dir  TRISB			/* Define LCD data port direction */
#define LCD_Port PORTB			/* Define LCD data port */
#define RS 0			/* Define Register Select pin */
#define EN 1 				/* Define Enable signal pin */

void LCD_Init(void);                   /*Initialize LCD*/
void LCD_Command(unsigned char);  /*Send command to LCD*/
void LCD_Char(unsigned char x);    /*Send data to LCD*/
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);	/* Send string to LCD with xy position */
void LCD_Clear(void);                  /*Clear LCD Screen*/

void ConvImp(int *unidades, int *decenas, int *centenas, int Tp, int valor);

void main(void) {
    char* Prac[] = {"11 E1", "11 E2", "12   ", "13   ", "14   ", "15   ", "16   "};
    char* num[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int s = 0, SelPrac = 0, AntSel = 0, Enter = 0, AntEnt = 0, Ent = 0;
    int i = 0, est = 0;
    int Val1PD03 = 0, Val2PD03 = 0, u = 0, d = 0, c = 0;
    int CicTrab = 0;
    float rpmpc = 0, rpm = 0, volt = 0;
    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISA = 0xFF;
    TRISC = 0x00;
    PORTC = 0x00;
    PORTA = 0x00;
    LCD_Init();
    while(1){
        while(!Enter){
            LCD_String_xy(0,0,"Seleccionar prac");
            LCD_String_xy(1,0,"Practica");
            SelPrac = RA1;
            if (SelPrac == 1 && AntSel == 0){
                s++;
                if (s>6) s = 0;
            }
            LCD_String_xy(1,9,Prac[s]);
            AntSel = SelPrac;
            Ent = RA2;
            if (Ent == 1 && AntEnt == 0){//RA2 (ENTER) seleccionada la practica
                Enter = !Enter;
                LCD_Clear();
            }
            AntEnt = Ent;
        }
        switch(s){
            case 0://P11 E1
                while(!RA3){//Inicia con boton de arranque
                    /*if(RA4){
                        est = 1;
                        break;
                    }*/
                    PORTC = 0x00;
                    LCD_String_xy(0,0,"M1   Apagado");
                    LCD_String_xy(1,0,"M2   Apagado");
                    Ent = RA2;
                    if (Ent == 1 && AntEnt == 0){//RA2 (ENTER) seleccionada la practica
                        Enter = !Enter;
                        break;
                    }
                    AntEnt = Ent;
                }
                if(Enter == 0){
                    //Enter = !Enter;
                    //est = 0;
                    LCD_Clear();
                    AntEnt = 1;
                    break;
                }
                RC4 = 1;//Prende motor 1 en C4
                LCD_String_xy(0,0,"M1   Encendido");
                for(i = 0;i<85;i++){
                    if(RA4){//Boton de paro
                        est = 1;
                        break;
                    }
                    __delay_ms(10);
                }
                if(est == 1){//Termina/reinicia P11
                    est = 0;
                    LCD_Clear();
                    break;
                }
                RC5 = 1;//Prende motor 2 en C5
                LCD_String_xy(1,0,"M2   Encendido");
                for(i = 0;i<100;i++){
                    if(RA4){//boton de paro
                        est = 1;
                        break;
                    }
                    __delay_ms(2);
                }
                if(est == 1){
                    est = 0;
                    LCD_Clear();
                    break;
                }
                PORTC = (1<<5);//Solo motor 1 encendido
                LCD_String_xy(0,0,"M1   Apagado   ");
                while(!RA4);//Boton de paro para reiniciar P11 E1
                LCD_Clear();
                break;
            case 1://P11 E2
                LCD_String_xy(0,0,"Suma en octal");
                while(!RA3){//boton arranque para grabar valor 1
                    Val1PD03 = PORTD & 0x0F;
                    ConvImp(&u, &d, &c, 8,Val1PD03);
                    LCD_String_xy(1,0,num[d]);
                    LCD_String_xy(1,1,num[u]);
                    Ent = RA2;
                    if (Ent == 1 && AntEnt == 0){//RA2 (ENTER) seleccionada la practica
                        Enter = !Enter;
                        break;
                    }
                    AntEnt = Ent;
                }
                if(Enter == 0){
                    LCD_Clear();
                    AntEnt = 1;
                    break;
                }
                LCD_String_xy(1,2,"+");
                while(PORTA != (0x03<<3)){//boton arranque y paro para grabar valor 2
                    Val2PD03 = PORTD & 0x0F;
                    ConvImp(&u, &d, &c, 8, Val2PD03);
                    LCD_String_xy(1,3,num[d]);
                    LCD_String_xy(1,4,num[u]);
                    Ent = RA2;
                    if (Ent == 1 && AntEnt == 0){//RA2 (ENTER) seleccionada la practica
                        Enter = !Enter;
                        break;
                    }
                    AntEnt = Ent;
                }
                if(Enter == 0){
                    LCD_Clear();
                    AntEnt = 1;
                    break;
                }
                LCD_String_xy(1,5,"=");
                Val1PD03 += Val2PD03;
                ConvImp(&u, &d, &c, 8, Val1PD03);
                LCD_String_xy(1,6,num[d]);
                LCD_String_xy(1,7,num[u]);
                while(!RA1);//Boton de seleccion de paractica para reiniciar P11 E2
                LCD_Clear();
                break;
            case 2://P12
                PR2 = 124;//Perido para Fpwm 1kHz prescaler 16
                CCPR2L = 0x00;//ciclo de trabajo 0 al inicio
                T2CON = 0x02;//Prescaler 16
                CCP2CON = 0x0C;//Activa modo pwm
                TMR2 = 0;//Timer 2 en 0
                TMR2ON = 1;//Inicia PWM
                LCD_Clear();//Limpia pantalla de mesajes anteriores
                while(Enter){
                    if(RA5){
                        CicTrab += 1;
                        if(CicTrab > 125) CicTrab = 125;
                    }
                    if(RA6){
                        CicTrab -= 1;
                        if(CicTrab <= 0) CicTrab = 0;
                    }
                    CCPR2L = CicTrab;
                    LCD_String_xy(0,6,"RPM");
                    rpm = CicTrab*207/125;//Calculo de RPM del motor
                    ConvImp(&u, &d, &c, 10, (int)rpm);
                    LCD_String_xy(1,6,num[c]);
                    LCD_String_xy(1,7,num[d]);
                    LCD_String_xy(1,8,num[u]);
                    LCD_String_xy(0,0,"%RPM");
                    rpmpc = rpm*100/207;//207 rpm 6v motor amarillo
                    ConvImp(&u, &d, &c, 10, (int)rpmpc);
                    LCD_String_xy(1,1,num[c]);
                    LCD_String_xy(1,2,num[d]);
                    LCD_String_xy(1,3,num[u]);
                    LCD_String_xy(0,11,"Volt");
                    volt = CicTrab*6/125;//Calculo de voltaje del motor
                    ConvImp(&u, &d, &c, 10, (int)volt);
                    LCD_String_xy(1,12,num[d]);
                    LCD_String_xy(1,13,num[u]);
                    Ent = RA2;
                    if (Ent == 1 && AntEnt == 0){//RA2 (ENTER) seleccionada la practica
                        Enter = !Enter;
                    }
                    AntEnt = Ent;
                }
                if (Enter == 0){
                    LCD_Clear();//Limpia LCD
                    CCPR2L = 0x00;//PWM en 0
                    TMR2ON = 0;//Apaga timer 2
                }
                break;
            case 3://P13
                
                break;
        }
    }
    return;
}
/****************************Funciones LCD********************************/
void LCD_Command( unsigned char cmnd )
{
    LCD_Port = ((0<<RS | 1<<EN)) | (0xF0 & cmnd);
    __delay_us(1);
    LCD_Port &= ~(1<<EN);
    __delay_us(20);
    LCD_Port = ((0<<RS | 1<<EN)) | (cmnd<<4);
    __delay_us(1);
    LCD_Port &= ~(1<<EN);
    __delay_ms(1);
}
void LCD_Char( unsigned char data )
{
	LCD_Port = ((1<<RS | 1<<EN)) | (0xF0 & data);
    __delay_us(1);
    LCD_Port &= ~(1<<EN);
    __delay_us(20);
    LCD_Port = ((1<<RS | 1<<EN)) | (data<<4);
    __delay_us(1);
    LCD_Port &= ~(1<<EN);
    __delay_ms(1);
}
void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0x00;			/* Make LCD port direction as o/p */
	__delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	__delay_ms(2);
}
void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}
void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}
void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	__delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}
/*************************Funcion Conversion a Octal********************************/
void ConvImp(int *unidades, int *decenas, int *centenas, int Tp, int valor){
    int conv = 8;
    if (Tp == 10){
    conv = 10;
    *centenas = valor/(10*conv);
    valor -= *centenas*100;
    *decenas = valor/conv;
    *unidades = valor%conv;
    } 
    *decenas = valor/conv;
    *unidades = valor%conv;    
    return;
}