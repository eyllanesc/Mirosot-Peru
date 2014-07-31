;***********************************Prueba_Mirosot v1.1*************************************

	list      p=16f628A           ; list directive to define processor
	#include <p16F628A.inc>       ; processor specific variable definitions

	errorlevel  -302              ; suppress message 302 from list file

	__CONFIG _CP_OFF & _LVP_OFF & _BOREN_OFF & _MCLRE_ON & _WDT_OFF & _PWRTE_ON & _INTOSC_OSC_NOCLKOUT

; El TMR0, TMR1 y TMR2 trabajan con un reloj interno de 4MHz entonces la unidad basica de tiempo es
; de 1us.

	CBLOCK  0x20			; Genera un lista de registros libres que comienza en 0x20
	TMR0_Repeticiones		; Numero de repeticiones del tiempo base(4MHz) para sumar mas prolongado
	TMR0_Registro			; Cambia por cada programacion el PWM que se quiere.
	TMR0_Programa			; Manipula quien prende o apaga es decir el PWM de los dos motores.
	TMR1_Repeticiones		; Numero de repeticiones del tiempo base(4MHz) para sumar mas prolongado
	TMR1_Registro			;
	TMR1_Programa			; Selecciona si se usa la programacion centrado, lado derecho o izquierdo
	TMR2_Repeticiones		; Numero de repeticiones del tiempo base(4MHz) para sumar mas prolongado
	TMR2_Lectura			; Tiempo que tarda en pasar una linea Horizontal
	TMR2_Programa			;
    DataStop
	ENDC

TMR0_MdOnMiOn		EQU d'4'			; Multiplo del tiempo base del TMR0 a ejecutar con MD_on_MI_on.
TMR0_MdOffMiOn		EQU d'2'			; Multiplo del tiempo base del TMR0 a ejecutar con MD_off_MI_on.
TMR0_MdOffMiOff		EQU d'1'			; Multiplo del tiempo base del TMR0 a ejecutar con MD_off_MI_off.
TMR0_Carga50ms		EQU	d'256'-d'195'	; Genera un tiempo base de 50ms para el TMR0.
TMR1H_Carga100ms	EQU	0xE7			; Genera un tiempo base de 100ms para el TMR1.
TMR1L_Carga100ms	EQU	0x96
;TMR1H_Carga100ms	EQU	0xCF			; Genera un tiempo base de 100ms para el TMR1.
;TMR1L_Carga100ms	EQU	0x2C
;TMR1H_Carga500ms	EQU	0x0B			; Genera un tiempo base de 500ms para el TMR1.
;TMR1L_Carga500ms	EQU	0xDC
;TMR2_ms				EQU d'1'
; COMENTARIOS GENERALES----------------------------------------------------------------------------------
;
;  - los sensores estan organizados de izquierda a derecha:
;
;            ___   ___   ___   ___   ___   ___
;            | |   | |   | |   | |   | |   | |
;      ___                                       ___
;      | |                 JUANK                 | |
;
;
;       B7    B6    B5    B4    B3    B2    B1    B0
;       ____________________    ____________________		NOTA: sensor en negro = 0 digital
;       Sensores    Derechos || Sensores  Izquierdos			  sensor en blanco = 1 digital
;
;
;  - los motores estan organizados de la siguiente manera:
;
;       A4    A3    A2    A1    A0      NOTA: 1 digital significa adelante en cado de motores
;       PO    EI    MI    MD    ED            y motor encendido en caso de eneibols
;
;       Donde: ED: eneibol derecho    EI: eneibol izquierdo    PO: led que mostrara
;              MD: motor derecho      MI: motor izquierdo          el arranque
;
;
; ZONA DE CÓDIGOS ********************************************************************

	ORG 	0
	goto	Inicio
	ORG		4						; Vector de interrupción.
	goto	Interrupcion

Inicio
	bsf		STATUS,RP0				; Acceso al Banco 1.
; --------------------------
	clrf    TRISA
	clrf    TRISB
	movlw   b'11111111'
	movwf   TRISB          			;configuramos el puerto B como entradas
	clrf   	TRISA
	movlw	b'00000101'
	movwf	OPTION_REG				; Prescaler de 256 asignado al TMR0
	movlw	b'11100000'				; Preautoriza interrupción del TMR1 con GIE y PEIE(INTCON,7,6)
	movwf	INTCON					; adicionalmente autoriza la interrupcion del TMR0 T1IE(INTCON,5).
	bsf		PIE1,TMR1IE		 		; Autoriza interrumcion del TMR1 con TMR1IE(PIE1,0)¡REGISTRO DEL BANCO 1!.
; --------------------------
	bcf		STATUS,RP0				; Acceso al Banco 0.
; --------------------------
	movlw  	b'00000111'          	; Desactivamos las opciones analogicas de los pines A0,A1,A3
	movwf	CMCON       			; quedando solo digital.
	movlw  	b'00110001'         	; Activa el TMR1 y preescaler en "8" con TMR1ON,T1CKPS1 y
	movwf	T1CON       			; T1CKPS2(CMCON,5,4,0).
; --------------------------
	clrf	TMR0_Repeticiones
	clrf	TMR0_Registro
	clrf	TMR0_Programa
	clrf	TMR1_Repeticiones
	clrf	TMR1_Registro
	clrf	TMR1_Programa
	clrf	TMR2_Repeticiones
	clrf	TMR2_Lectura
	clrf	TMR2_Programa
	clrf	PORTB
	clrf	PORTA
;--------------------------			; Aqui se hace la primera carga numero de repeticiones del tiempo
	movlw	d'1'					; base de 50ms del TMR0.
	movwf	TMR0_Repeticiones
;--------------------------			; Aqui se hace la primera carga numero de repeticiones del tiempo
	movlw	d'10'					; base de 100ms del TMR1.
	movwf	TMR1_Repeticiones
; --------------------------		; Carga el TMR1 con su valor inicial.
	movlw	TMR1H_Carga100ms
	movwf	TMR1H					; Recarga el TMR1H.
	movlw	TMR1L_Carga100ms
	movwf	TMR1L					; Recarga el TMR1L.
;--------------------------
	movlw	TMR0_Carga50ms			; Carga el TMR0 con su valor inicial.
	movwf	TMR0
;--------------------------

Principal							; Aqui se selecciona que porgramacion vamos a usar.
	movf	TMR1_Programa,W
	addwf	PCL,F
	goto	Partida
	goto	Programacion1
	goto	Programacion2
	goto	Programacion3

; Subrutina "Interrupcion" -------------------------------------------------------
;
; Como el PIC trabaja a una frecuencia de 20 MHz el TMR0 evoluciona cada microsegundo.
; El bucle central se hace en un tiempo de 10 ms. Para ello se utiliza un prescaler
; de 256 y el TMR0 tiene que contar 195. Efectivamente 0,2 x 195 x 256 = 9984 µs = 10 ms.
; Para conseguir una temporización de 2.5 s habrá que repetir 250 veces el lazo de 10 ms.
; Para conseguir una temporización de 800 ms habrá que repetir 16 veces el lazo de 10 ms.

Interrupcion
	btfsc	PIR1,TMR1IF
	goto	Interrupcion_TMR1

; Subrutina "Timer0_Interrupcion" -------------------------------------------------------
;
; Como el PIC trabaja a una frecuencia de 20 MHz el TMR0 evoluciona cada microsegundo.
; El bucle central se hace en un tiempo de 10 ms. Para ello se utiliza un prescaler
; de 256 y el TMR0 tiene que contar 195. Efectivamente 195 x 256 = 9984µs = 10ms.
; Para conseguir una temporización de 2.5 s habrá que repetir 250 veces el lazo de 10 ms.
; Para conseguir una temporización de 800 ms habrá que repetir 16 veces el lazo de 10 ms.

Interrupcion_TMR0
    btfsc   DataStop,0
    goto    TMR0_Stop

	movlw	TMR0_Carga50ms
	movwf	TMR0					; Recarga el TMR0.
	decfsz	TMR0_Repeticiones,F		; Decrementa el contador.
	goto	TMR0_FinInterrupcion
	movf	TMR0_Programa,W
	addwf	PCL,F
	goto	MD_on_MI_on
	goto	MD_off_MI_on
	goto	MD_off_MI_off
MD_on_MI_on
	incf	TMR0_Programa,F
	bsf		PORTA,0
	bsf		PORTA,3
	movlw	TMR0_MdOnMiOn
	goto	TMR0_CargaRepeticiones
MD_off_MI_on
	incf	TMR0_Programa,F
	bcf		PORTA,0
	movlw	TMR0_MdOffMiOn
	goto	TMR0_CargaRepeticiones
MD_off_MI_off
	clrf	TMR0_Programa
	bcf		PORTA,3
	movlw	TMR0_MdOffMiOff
TMR0_CargaRepeticiones
	movwf	TMR0_Repeticiones
TMR0_FinInterrupcion
	bcf		INTCON,T0IF				; Repone flag del TMR0.
	retfie							; Retorno de interrupción.

TMR0_Stop
    clrf    PORTA
	movlw	d'256'-d'1'
	movwf	TMR0					; Recarga el TMR0.
    bcf		INTCON,T0IF				; Repone flag del TMR0.
    retfie

; Subrutina "Timer1_Interrupcion" -------------------------------------------------------
;
; Como el PIC trabaja a una frecuencia de 20 MHz el TMR0 evoluciona cada microsegundo.
; El bucle central se hace en un tiempo de 10 ms. Para ello se utiliza un prescaler
; de 256 y el TMR0 tiene que contar 195. Efectivamente 0,2 x 195 x 256 = 9984 µs = 10 ms.
; Para conseguir una temporización de 2.5 s habrá que repetir 250 veces el lazo de 10 ms.
; Para conseguir una temporización de 800 ms habrá que repetir 16 veces el lazo de 10 ms.

Interrupcion_TMR1
;	bcf		T1CON,TMR1ON			; Detenemos el TMR1 momentaneamente
    bcf		INTCON,T0IF             ; Desactivamos El TMR0 que es mas frecuente para qu no
                                    ; afecte el TMR1 que es menos frecuente.
	movlw	TMR1H_Carga100ms
	movwf	TMR1H					; Recarga el TMR1H.
	movlw	TMR1L_Carga100ms
	movwf	TMR1L					; Recarga el TMR1L.
	decfsz	TMR1_Repeticiones,F		; Decrementa el numero de repeticiones.
	goto	TMR1_FinInterrupcion
	movf	TMR1_Programa,W
	addwf	PCL,F
	goto	Configuracion1
	goto	Configuracion2
	goto	Configuracion3
	goto	ConfiguracionFinal		; Configuracion de cierre.
Configuracion1
	incf	TMR1_Programa,F
	movlw	d'12'					; Numero de veces a repetirse el tiempo base de 500ms
	goto	TMR1_CargaRepeticiones	; del TMR1 para la Programacion 1.
Configuracion2
	incf	TMR1_Programa,F
	movlw	d'3'				 	; Numero de veces a repetirse el tiempo base de 500ms
	goto	TMR1_CargaRepeticiones	; del TMR1 para la Programacion 2.
Configuracion3
	incf	TMR1_Programa,F
	movlw	d'12'					; Numero de veces a repetirse el tiempo base de 500ms
	goto	TMR1_CargaRepeticiones	; del TMR1 para la Programacion 3.
ConfiguracionFinal
	clrf	TMR1_Programa
	movlw	d'10'					; Numero de veces a repetirse el tiempo base de 500ms
TMR1_CargaRepeticiones				; del TMR1 reiniciando a la Programacion 1.
	movwf	TMR1_Repeticiones
TMR1_FinInterrupcion
	bcf		PIR1,TMR1IF				; Repone flag del TMR1.
    bsf		INTCON,T0IF             ; Activamos nuevamente el TMR0 desactivado previamente
                                    ; para no afectar ese proceso.
;    bsf		T1CON,TMR1ON        ; paso no necesario.
	retfie							; Retorno de interrupción.

; PROGRAMACIONES DEFINIDAS **************************************************************
; Aqui estaran definidos todos los posibles bucles a intervenir en cada tiempo controlado
; por el TMR1.
; ___________________________________________________________________
; PROGRAMACION CENTRADA

Partida
	call	Defrente
	goto	Principal

Programacion1
	call	Stop
	goto	Principal

Programacion2
	call	Derecha
	goto	Principal

Programacion3
	call    Stop
	goto	Principal

;ZONA DE ARCHIVOS INCLUIDOS EN LA PROGRAMACIÓN------------------------------------------------------------

	include<MovimientosPWM_Stop.asm>

	END
