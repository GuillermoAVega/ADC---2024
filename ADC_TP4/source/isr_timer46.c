#include "p33FJ256GP710.h"
#include "config.h"
#define VALPR4	39063
#define ValPR6	0

volatile unsigned char hours;
volatile unsigned char minutes;
volatile unsigned char seconds;
extern int counterTog;
extern int i;
extern int contTimer;

/*---------------------------------------------------------------------
  Function Name: _T4Interrupt
  Description:   Timer4 Interrupt Handler

Comments: 39062
-----------------------------------------------------------------------*/
void __attribute__((interrupt, auto_psv)) _T4Interrupt( void )
{
	/* reset Timer 4 interrupt flag */
    IFS1bits.T4IF = 0;          //Bajo la flag de interrupcion
    contTimer++;                //Numero de interrupcion del timer
    if (contTimer == 4){        //Si paso 1 segundo
        T4CONbits.TON = 0;      
        IFS1bits.U2TXIF = 1;    
        i = 0;                  //Inicializo el indice del arreglo en 0
        TMR4 = 0;               //Limpio el registro del timer
        contTimer = 0;          //Limpio la variable de numero de interrupcion del timer
    } 

    //T4CONbits.TON = 0; 	//Deshabilito Timer
	//counterTog ++;
}

/*---------------------------------------------------------------------
  Function Name: Init_Timer4
  Description:   Initialize Timer4

-----------------------------------------------------------------------*/
void Init_Timer4( void )
{
    T4CONbits.TON = 0;
    T4CONbits.TCS = 0;
    T4CONbits.TCKPS = 3;
    TMR4 = 0;
    PR4 = VALPR4;
    IPC6bits.T4IP = 5;
    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 1;
}


/*---------------------------------------------------------------------
  Function Name: _T6Interrupt
  Description:   Timer6 Interrupt Handler

-----------------------------------------------------------------------*/
void __attribute__((interrupt, auto_psv)) _T6Interrupt( void )
{
	/* reset Timer 6 interrupt flag */
 	IFS2bits.T6IF = 0;
	T6CONbits.TON = 0;	//Deshabilito Timer6

}

/*---------------------------------------------------------------------
  Function Name: Init_Timer6
  Description:   Initialize Timer6

-----------------------------------------------------------------------*/
void Init_Timer6( void )
{
	
	/* ensure Timer 6 is in reset state */
	T6CON = 0;
	T6CONbits.TCKPS = 1; //Prescaler 8

	/* reset Timer 6 interrupt flag */
 	IFS2bits.T6IF = 0;
 	
 	/* set Timer interrupt priority level */
	IPC11bits.T6IP = 5;

	/* enable Timer interrupt */
 	IEC2bits.T6IE = 1;
 	  	
	/* set Timer period register */
	PR6 = ValPR6;
	T6CONbits.TON = 0; 	//habilito Timer

}
