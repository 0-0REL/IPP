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
 	Uni		;Guarda valor de display unidades
 	Dec		;Guarda valor de display decenas
 	Val1	;Valor 1
 	Val2	;Valor 2
 	Reta	;Numero de ciclos para crear retardo
 	ENDC
 	BANKSEL	PORTA
 	clrf	PORTA
 	clrf	PORTB
 	clrf	PORTC
 	clrf	Uni
 	clrf	Dec
 	clrf	Val1
 	clrf	Val2
 	clrf	Reta
 	BANKSEL	ANSEL
 	clrf	ANSEL
 	clrf	ANSELH
 	BANKSEL	TRISA
 	movlw	0x7F
 	movwf	TRISA	;Puerto A [0,4] bits entrada, 5 Hex, 6 P7
 	movlw	0x00
 	movwf	TRISB	;Puerto B, todo salida a display 7 seg
 	movlw	0x03
 	movwf	TRISC	;Puerto C, C0 guarda Val1, C1 suma Val 1 y 2, C0 reinicia
 	BANKSEL	PORTA
;***P R A C T I C A * 6***
Pra6	
	movfw	PORTA		;Lectura Puerto A
 	andlw	0x0F		;[0,3] 4 bits
 	movwf	Uni
	clrf	Dec
	call	Conv		;Convierte y muestra en display
	btfss	PORTA,6		;Si se activa A6 inicia Pra7
	goto	Pra6
;***P R A C T I C A * 7***
Pra7
	btfss	PORTA,6		;Termina
	goto	Pra6		;Pra7
	movfw	PORTA
	andlw	0x1F
	movwf	Val1	;Guarda primer valor
	movwf	Uni
	clrf	Dec
	call	Conv	;Muestra en display valor 1
	btfss	PORTC,0	;Espera boton [C0] para continuar
	goto	Pra7
Vals	;Valor 1 guardado, espera valor 2
	btfss	PORTA,6		;Termina Pra7 y regresa
	goto	Pra6		;a Pra6
	movfw	PORTA
	andlw	0x1F
	movwf	Val2	;Guarda valor 2
	movwf	Uni
	clrf	Dec
	call	Conv	;Muestra valor 2 en display
	btfss	PORTC,1	;Espera boton [C1] para sumar y continuar
	goto	Vals
Resultado			;Muestra resulatado de la suma
	btfss	PORTA,6		;Termina
	goto	Pra6		;P7
	movfw	Val1
	addwf	Val2,W	;Se suma valor 1 y 2
	movwf	Uni
	clrf	Dec
	call	Conv
	btfss	PORTC,0		;[C0] y
	goto	Resultado
	btfss	PORTC,1
	goto	Resultado	;[C1] para reiniciar Pra7
	goto	Pra7		;Soltar primero C0 y despues C1
;**********SUBRUTINAS**********
Conv		;Inicia conversion
 	movlw	.10		;Decodificacion en decimal por defecto
 	btfsc	PORTA,5	;Si A5 activado cambia decodificacion
 	movlw	0x10	;a hexadecimal
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
 	call	Rtad
 	movfw	Dec
 	call	SalDisp
 	movwf	PORTB
 	call	Rtad
	return	;Termina conversion
SalDisp		;Config para impresion en 7 seg en hexadecimal
	addwf	PCL,F
	DT	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0xF7,0xFF,0x39,0X3F,0x79,0X71
	return
Rtad		;Retardo
	movlw	.10
	movwf	Reta
Rtd
	decfsz	Reta,F
	goto	Rtd
	return
  	END	