#include <p33FJ256GP710.h>
#include "kernel.h"
#define MAX 8

int procA[MAX];
int procB[MAX];
int procC[MAX];
int quantum = 0;
extern int *direccionB;
extern int *direccionC;
int *puntero;
int proceso = 0;
int i;

void init(void)
{
    procB[0] = direccionB;
    procC[0] = direccionC;
}

void confReloj(void)
{
    //Configuracion Timer1
    T1CONbits.TON = 0; //Apago el  timer
    T1CONbits.TCS = 0; //Clock interno
    T1CONbits.TCKPS = 1;  //Preescaler en 1:8
    TMR1 = 0;   //Seteo en 0 el registro del timer1
    PR1 = 500;  
    IFS0bits.T1IF = 0;  //Bajo la flag por interrupcio de Timer1
    IEC0bits.T1IE = 1;  //Habilito la interrupcion por Timer1
    T1CONbits.TON = 1;  //Enciendo el timer
}

void boot(void)
{
    init();
    confReloj();
    return;
}

void planificador(void)
{
    proceso++; 
    puntero = WREG15 - 0x0020;
    
    switch (proceso){
        
        case 1: 
            for (i = 0; i < MAX ; i++){
                procA[i] = *puntero;
                *puntero = procB[i];
                puntero++;
            }
            break;
        
        case 2:

            for (i = 0; i < MAX; i++){
                procB[i] = *puntero; 
                *puntero = procC[i];
                puntero++;
            }
            break;
            
        case 3:
            for (i = 0; i < MAX; i++){
                procC[i] = *puntero; 
                *puntero = procA[i];
                puntero++;
            }
            proceso = 0;
            break;
    }

}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
    IFS0bits.T1IF = 0;
    quantum++;
    if (quantum == 2){
        quantum = 0;
        planificador();
    }
    TMR1 = 0;
}
