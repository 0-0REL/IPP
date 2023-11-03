;Instituto Politécnico Nacional 
;ESIME Unidad Azcapotzalco
;PRACTICA 6 y 7
;ARELLANO FLORES RODRIGO EMMANUEL
;MARTINEZ FAJARDO RODRIGO
;MONTIEL ZUÑIGA EMMANUEL ESAÚ
;IPP PROFESOR M. EN C. RAMÓN VALDÉS
;*****************************************************************
	__CONFIG _CONFIG1, _FOSC_INTRC_NOCLKOUT & _WDTE_OFF & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _IESO_ON & _FCMEN_ON & _LVP_OFF
 	__CONFIG _CONFIG2, _BOR4V_BOR40V & _WRT_OFF
 	LIST P=P16F8887
 	#INCLUDE <P16F887.INC>
 	ORG 0x00
 	CBLOCK	0x20
 	Sel		;Ayuda en la seleccion de modo
 	Uni		;Guarda valor de display unidades
 	Dec		;Guarda valor de display decenas
 	Tdeco	;Indica el tipo de decodificacion
 	Val1	;Valor 1
 	Val2	;Valor 2
 	ENDC
 	BANKSEL	PORTA
 	clrf	PORTA
 	clrf	PORTB
 	clrf	PORTC
 	clrf	Uni
 	clrf	Dec
 	clrf	Tdeco
 	clrf	Val1
 	clrf	Val2
 	BANKSEL	ANSEL
 	clrf	ANSEL
 	clrf	ANSELH
 	BANKSEL	TRISA
 	movlw	0xFF
 	movwf	TRISA	;Puerto A [0,4] bits entrada, 5 Hex, 6 Dec, 7 P7
 	movlw	0x00
 	movwf	TRISB	;Puerto B, todo salida a display 7 seg
 	movlw	0x03
 	movwf	TRISC	;Puerto C, C0 guarda Val1, C1 suma Val 1 y 2, C0 reinicia
 	BANKSEL	PORTA
;**********MACROS**********
ErrorP6	MACRO		;Error P6, dos metodos de decodificacion escogidos
	movfw	PORTA	;solo se puede uno
	andlw	0x60
	movwf	Sel
	movlw	0x60
	subwf	Sel,W
	ENDM
FinP6	MACRO		;Termina P6 de no estar seleccionado algun modo
	movfw	PORTA	;de decodificacion
	andlw	0x60
	movwf	Sel
	movlw	0x00
	subwf	Sel,W
	ENDM
;**********SELECCION DE PRACTICA 6 O 7**********
Seleccion	;Selcciona que se va a hacer
	call	SelMDco
	btfsc	Tdeco,0		;Conversion a hexadecimal
 	goto	Pra6
 	btfsc	Tdeco,1		;Conversion a decimal
 	goto	Pra6
 	btfsc	PORTA,7		;Prac 7
 	goto	Pra7
 	goto	Seleccion
;***P R A C T I C A * 6***
Pra6
	ErrorP6				;Pregunta si hex [A5] y dec [A6] estan encendidos
	btfsc	STATUS,Z	;De ser asi
	goto	Seleccion	;Error, solo se puede escoger una decodificacion
	FinP6
	btfsc	STATUS,Z	;Termina si hex [A5] o
	goto	Seleccion	;dec [A6] estan apagados
	movfw	PORTA		;Lectura Puerto A
 	andlw	0x0F		;[0,3]
 	movwf	Uni
	clrf	Dec
	call	Conv		;Convierte y muestra en display
	goto	Pra6
;***P R A C T I C A * 7***
Pra7
	FinP6				;Error, no se
	btfsc	STATUS,Z	;escogio tipo
	goto	Pra7		;de decodificacion
	call	SelMDco
	ErrorP6				;Pregunta si hex [A5] y dec [A6] estan encendidos
	btfsc	STATUS,Z	;De ser asi
	goto	Pra7		;Error, solo se puede escoger una decodificacion 
	btfss	PORTA,7		;Termina
	goto	Seleccion	;Pra7
	movfw	PORTA
	andlw	0x1F
	movwf	Val1	;Guarda primer valor
	movwf	Uni
	clrf	Dec
	call	Conv	;Muestra en display valor 1
	btfss	PORTC,0	;Espera boton [C0] para continuar
	goto	Pra7
Vals	;Valor 1 guardado, espera valor 2
	btfss	PORTA,7		;Termina Pra7 y regresa
	goto	Seleccion	;a inicio (seleccion)
	movfw	PORTA
	andlw	0x1F
	movwf	Val2	;Guarda valor 2
	movwf	Uni
	clrf	Dec
	call	Conv	;Muestra valor 2 en display
	btfss	PORTC,1	;Espera boton [C1] para sumar y continuar
	goto	Vals
Resultado			;Muestra resulatado de la suma
	btfss	PORTA,7		;Termina
	goto	Seleccion	;P7
	movfw	Val1
	addwf	Val2,W	;Se suma valor 1 y 2
	movwf	Uni
	FinP6				;Error, no se
	btfsc	STATUS,Z	;escogio tipo
	goto	Resultado	;de decodificacion
	call	SelMDco
	ErrorP6				;Pregunta si hex [A5] y dec [A6] estan encendidos
	btfsc	STATUS,Z	;De ser asi
	goto	Resultado	;Error, solo se puede escoger una decodificacion 
	clrf	Dec
	call	Conv
	btfss	PORTC,0		;[C0] para reiniciar Pra7
	goto	Resultado
	goto	Pra7
;**********SUBRUTINAS**********
SelMDco		;Selecciona modo de conversion, hexadecimal o decimal
	clrf	Tdeco
	movfw	PORTA
	andlw	0x60
	movwf	Sel
	movlw	0x20
	subwf	Sel,W
	btfsc	STATUS,Z
	movlw	0x01	;Idetificacion de que convierte a hexadecimal
	btfsc	STATUS,Z
	movwf	Tdeco	;Variable que controla la conversion
	movlw	0x40
	subwf	Sel,W
	btfsc	STATUS,Z
	movlw	0x02	;Idetificacion de que convierte a decimal
	btfsc	STATUS,Z
	movwf	Tdeco	;Variable que controla la conversion
	return
Conv		;Inicia conversion
	btfsc	Tdeco,0	;Seleccion de la conversion a hexadecimal
	movlw	0x10
	btfsc	Tdeco,1	;Seleccion de la conversion a decimal
 	movlw	.10
 	subwf	Uni,W
 	btfss	STATUS,C
 	goto	Disp
	movwf	Uni
 	incf	Dec,F
 	goto	Conv
Disp
	movfw	Uni
 	call	SalDisp
 	movwf	PORTB
 	bsf		PORTB,7	
 	nop
 	nop
 	nop
 	movfw	Dec
 	call	SalDisp
 	movwf	PORTB
 	nop
 	nop
 	nop
	return	;Termina conversion
SalDisp		;Config para impresion en 7 seg en hexadecimal
	addwf	PCL,F
	DT	0x3F,0x06,0x5B,0x4F,0x6A,0x7D,0x7C,0x47,0x7F,0x67,0x3F,0xFF,0x39,0X3F,0x79,0X71
	return
  	END	
