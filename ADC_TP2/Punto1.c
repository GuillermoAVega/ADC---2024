/*
 * File:   Punto1.c
 * Author: Guille
 *
 * Created on 11 de abril de 2024, 16:02
 
*/

#include "xc.h"
#define VALOR_PR 500
#define PR_MAX 2000
#define PRODUCTOR_MAX 3
#define CONSUMIDOR_MAX  5
#define TRUE 1
#define FALSE 0

int PS1 = 0;
int PS2 = 0;
char Productor[PRODUCTOR_MAX] = "";
char Consumidor[CONSUMIDOR_MAX] = "";
int Contador = 0;
int i = 0;
int j = 0;
int elementosNuevos = 0;
int elementos = 0;


void __attribute__((interrupt, auto_psv)) _INT0Interrupt( void )
{
	/* reset INT0 interrupt flag */
 	IFS0bits.INT0IF = 0;
    PR1 = VALOR_PR;
    
    if (i == PRODUCTOR_MAX){
        i = 0;
    }
    Productor[i] = PORTA;
    if (elementos != PRODUCTOR_MAX)
        elementos++;
    i++;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
	/* reset INT0 interrupt flag */
 	IFS0bits.T1IF = 0;
    if (elementos != 0){
        PR1 = VALOR_PR;
        elementosNuevos = TRUE;
    }
    else{
        if (PR1 != PR_MAX){
            PR1 = (PR1 * 2);
        }
    }
}


int main(void) {
    config ();
    
    while (1){
        programa ();        
    }
    return 0;
}

void config()
{
    TRISBbits.TRISB0 = 0;  //Configura el pin B0 como salida 
    TRISBbits.TRISB1 = 0;  //Configura el pin B1 como salida
    TRISDbits.TRISD12 = 1; //Configura el pin D12 como entrada
    TRISDbits.TRISD13 = 1; //Configura el pin D13 como entrada
    AD1PCFGH = 0xFFFF; 
    AD1PCFGL = 0xFFFF;
    AD2PCFGL = 0xFFFF;
    TRISA = 0xFFFF;   //Configura el puerto A como entrada
    //Configuracion Timer1
    T1CONbits.TON = 0; //Apago el  timer
    T1CONbits.TCS = 0; //Clock interno
    T1CONbits.TCKPS = 0;  //Preescaler en 1:1
    TMR1 = 0;   //Seteo en 0 el registro del timer1
    PR1 = VALOR_PR;  
    // IPC0bits.T1IP = 0; VER EL TEMA DE LA PRIORIDAD
    IFS0bits.T1IF = 0;  //Bajo la flag por interrupcio de Timer1
    IEC0bits.T1IE = 1;  //Habilito la interrupcion por Timer1
    T1CONbits.TON = 1;  //Enciendo el timer
    
    //Configuracion INT0
    IFS0bits.INT0IF = 0; //Baja la flag de la interrupcion por INT0
    IEC0bits.INT0IE = 1; //Habilita la interrupcion por INT0
    INTCON2bits.INT0EP = 1; //Interrupcion por flanco descendente
}

void programa ()
{ 
    PS1 = PORTDbits.RD12; //Le asigna a la variable PS1 el valor de la lectura del pin RD12
    PS2 = PORTDbits.RD13; //Le asigna a la variable PS2 el valor de la lectura del pin RD13
    
    if ((PS1) && (PS2)){  //Si ninguno de los pulsadores esta presionado
        PORTBbits.RB0 = 1; //Enciende el LED LD1
        PORTBbits.RB1 = 1; //Enciende el LED LD2
    }
    else {
        if (!(PS1)) // Si se presiona el pulsador PS1
            PORTBbits.RB1 = 0; //Apaga el LD2
        if (!(PS2))   //Si se presiona el pulsador PS2
            PORTBbits.RB0 = 0; //Apaga el LD1
    }
    
    if (elementosNuevos){
        while (elementos !=  0){
            if (j == CONSUMIDOR_MAX){
                j = 0;
            }
            if (Contador == PRODUCTOR_MAX) {
                Contador = 0;
            }
            Consumidor[j] = Productor[Contador];
            Contador++;
            j++;
            elementos--;
        }
        elementosNuevos = FALSE;
    }
}
