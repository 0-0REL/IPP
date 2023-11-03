;Instituto Politécnico Nacional 
;ESIME Unidad Azcapotzalco
;PRACTICA 5 MECANISMO DE 4 SENSORES
;ARELLANO FLORES RODRIGO EMMANUEL
;MARTINEZ FAJARDO RODRIGO
;MONTIEL ZUÑIGA EMMANUEL ESAÚ
;IPP PROFESOR M. EN C. RAMÓN VALDÉS
;*****************************************************************
		__CONFIG _CONFIG1, _FOSC_INTRC_NOCLKOUT & _WDTE_OFF & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _IESO_ON & _FCMEN_ON & _LVP_OFF
		__CONFIG _CONFIG2, _BOR4V_BOR40V & _WRT_OFF
		LIST P=P16F887
		#INCLUDE <P16F887.INC> 
		ORG 0x00
		BANKSEL	PORTA
		clrf 	PORTB
 		BANKSEL	ANSEL
 		clrf 	ANSEL
 		clrf 	ANSELH
 		BANKSEL	TRISA
 		movlw 	3Fh		;ARRANQUE B0, PARO B5, SENSORES [B1, B4] Y MOTOR [B6, B7]
 		movwf 	TRISB
 		BANKSEL	PORTA
Inicio					;NO HACE NADA, ESPERA ARRANQUE
		btfss	PORTB,0	;ESPERA A QUE SE PRESIONE ARRANQUE PARA SALIR DEL BUCLE
		goto Inicio
Pso1	call 	G_der	;GIRO DERECHA
		call	Paroo	;PREGUNTA POR PARO
		btfss	PORTB,3	;TERMINA Pso1 CUANDO S3->1
		goto 	Pso1
Pso2	call	G_izq	;GIRO IZQUIERDA
		call	Paroo	;PREGUNTA POR PARO
		btfss	PORTB,1	;TERMINA Pso2 CUANDO S1->1
		goto 	Pso2
Pso3	call	G_der	;GIRO DERECHA
		call	Paroo	;PREGUNTA POR PARO
		btfss	PORTB,4	;TERMINA Pso3 CUANDO S4->1
		goto 	Pso3
Pso4	call	G_izq	;GIRO IZQUIERDA
		call	Paroo	;PREGUNTA POR PARO
		btfss	PORTB,2	;TERMINA Pso4 CUANDO S2->1
		goto 	Pso4
Pso5	call	G_der	;GIRO DDERECHA
		call	Paroo	;PREGUNTA POR PARO
		btfss	PORTB,3	;TERMINA Pso5 CUANDO S3->1
		goto 	Pso5
Pso6	call	G_izq	;GIRO IZQUIERDA
		call	Paroo	;PREGUNTA POR PARO
		btfsc	PORTB,1	;TERMINA Pso6 CUANDO S1->1
		goto	Pso1	;Y REGRESA A Pso1
		goto 	Pso6
G_der				;GIRO A LA DERECHA
		movlw	40h
		movwf	PORTB
		return
G_izq				;GIRO A LA IZQUIERDA
		movlw	80h
		movwf	PORTB
		return
Paroo				;PREGUNTA POR PARO
		btfsc	PORTB,5
		call	Alto
		return
Alto	movlw	00h	;PARO->1, SE PAUSA
		movwf	PORTB
		btfss	PORTB,0
		goto	Alto
		return
 END