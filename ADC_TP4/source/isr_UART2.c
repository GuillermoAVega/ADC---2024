/* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Ricardo López    11/6/10  UART2C module interrupt handler
* Marcelo Gómez    09/03/14 Portado a MPLAB.X
* Marcelo Gómez    20/05/15 Portado a MPLAB.X
* Sebastián Wahler 04/06/19 Simplificado
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
*
**********************************************************************/

#include "p33FJ256GP710.h"

//UART
#define FCY 40000000
#define BAUDRATE 19200
#define BRGVAL ((FCY / BAUDRATE) / 4) -1
#define MAX 255 
#define TRUE 1
#define FALSE 0 

volatile unsigned char bufferA[MAX];
volatile unsigned char x;
int primerCaracter = TRUE;  //Bandera
int cant = 0;   //Cantidad de caracteres a enviar
int i = 0;
int contTimer = 0;



/*---------------------------------------------------------------------
  Function Name: UART2Interrupt
  Description:   UART2 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
//Interrupciones de la UART2

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void )
{
	IFS1bits.U2RXIF = 0;
    
    if (primerCaracter) {           //Si es el primer caracter recibido
        primerCaracter = FALSE;
        cant = U2RXREG;     //Asigna la cantidad de caracteres a transmitir
        i = 0;              //Inicializa el indice del arreglo en 0
    }
    
    if ((cant - 1) >= i){       //Si quedan caracteres a cargar en el arreglo
        bufferA[i] = U2RXREG;   //Cargo el caracter en el arreglo
        if ((cant - 1) == i)    //Si se transmitieron todos los caracteres
            T4CONbits.TON = 1;  
    }
    
    i = i + 1;                  //Incremento el indice del arreglo 
}

//Rutina de INT para transmisión (no hace nada)
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
	//Atención. Se debe modificar para que no emita reiteradamente
	//caracteres
    IFS1bits.U2TXIF = 0;            //Bajo la flag de interrupcion 
    if((cant != 0)){                //Si no transmitio todos los valores 
        U2TXREG = bufferA[i];  //Simple ECO
        i++;                   
        cant--;                //Cantidad de caracteres a transferir
    }
    else {
        primerCaracter = TRUE;
    }
}

/*---------------------------------------------------------------------
  Function Name: InitUART2
  Description:   Inicializar UART2
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void InitUART2(void)
{
	// The HPC16 board has a DB9 connector wired to UART2, 
	// so we will be configuring this port only
	// configure U2MODE
	U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	U2MODEbits.RTSMD = 1;	// Bit11 Simplex Mode
    U2MODEbits.PDSEL = 0;   //8 bits sin paridad
    U2MODEbits.STSEL = 0;   //1 bit de stop
    U2MODEbits.BRGH = 0;    //low speed 
    U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXISEL1 = 0; 
    
	// Load a value into Baud Rate Generator.  Example is for 9600.
	U2BRG = BRGVAL;	// 1Mhz osc, 19200 Baud

	IPC7 = 0x4400;	// Mid Range Interrupt Priority level, no urgent reason
    
    U2MODEbits.UARTEN = 1;	// And turn the peripheral on
	U2STAbits.UTXEN = 1;	// Empieza a transmitir. Se dispara el Flag TXIF
    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
    IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts
    IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}
