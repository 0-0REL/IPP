PROCESSOR 16F887
; CONFIG1
  CONFIG  FOSC = INTRC_NOCLKOUT ; Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
  CONFIG  PWRTE = ON            ; Power-up Timer Enable bit (PWRT enabled)
  CONFIG  MCLRE = ON            ; RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
  CONFIG  CP = OFF              ; Code Protection bit (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Code Protection bit (Data memory code protection is disabled)
  CONFIG  BOREN = ON            ; Brown Out Reset Selection bits (BOR enabled)
  CONFIG  IESO = ON             ; Internal External Switchover bit (Internal/External Switchover mode is enabled)
  CONFIG  FCMEN = ON            ; Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
  CONFIG  LVP = OFF             ; Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

; CONFIG2
  CONFIG  BOR4V = BOR40V        ; Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
  CONFIG  WRT = OFF             ; Flash Program Memory Self Write Enable bits (Write protection off)
#include <xc.inc>
  PSECT	code
	tem EQU	20
	result	EQU 21
	count	EQU 22
	BANKSEL	ANSEL
	clrf	ANSEL
	clrf	ANSELH
	BANKSEL	TRISA
	movlw	0xFF
	movwf	TRISC	;ENTRADA
	clrf	TRISD	;SALIDA
	BANKSEL	PORTA
	clrf	PORTC
	clrf	PORTD
	main:
    movf    PORTC,W        ; Byte original
    movwf tem        ; Guardar el byte original en un registro temporal
    clrf result       ; Limpiar el registro donde se guardará el byte invertido
    movlw 0x08        ; Contador para iterar sobre los bits del byte original
    movwf count
    loop:
	rrf tem, f   ; Rotar el bit menos significativo del byte original al bit de carry
	rlf result, f ; Rotar el bit de carry al byte resultado
	decfsz count, f ; Decrementar el contador y saltar si es cero
	goto loop   ; Repetir el proceso hasta que se procesen los 8 bits
	movf	result,w
	movwf	PORTD
    goto main
END
	


