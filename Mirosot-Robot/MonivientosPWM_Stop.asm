;********************************MonivientosPWM_Stop****************************
;COMENTARIOS GENERALES-----------------------------------------------------------------------------------
;
;  - los motores estan organizados de la siguiente manera:
;
;       A4    A3    A2    A1    A0      NOTA: 1 digital significa adelante en cado de motores
;       PO    EI    MI    MD    ED            y motor encendido en caso de enibols
;
;       Donde: ED: eneibol derecho    EI: eneibol izquierdo    PO: led que mostrara
;              MD: motor derecho      MI: motor izquierdo          el arranque
;
;
;ZONA DE CODIGOS-----------------------------------------------------------------------------------------

Defrente
        bcf     DataStop,0
		bsf		PORTA,1
		bsf		PORTA,2
	    return

Stop
        bsf     DataStop,0
	    return

;Giros sobre un eje central
Derecha
        bcf     DataStop,0
		bcf		PORTA,1
		bsf		PORTA,2
	    return

Izquierda
        bcf     DataStop,0
		bsf		PORTA,1
		bcf		PORTA,2
	    return

;Giros con eje en una llanta
DerechaLA							;longitud de arco con
		movlw   	b'00011100'		;eje en llanta derecha
	    movwf   	PORTA
	    return

IzquierdaLA
		movlw   	b'00010011'		;longitud de arco con
	    movwf   	PORTA			;eje en llanta izquierda
	    return
