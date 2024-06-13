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
#define BAUDRATE 9600
#define BRGVAL ((FCY / BAUDRATE) / 16) -1
#define TAM_ARREGLO 256
#define SOF 0xFE
#define TRUE 1
#define FALSE 0

int primerCaracter = TRUE;  //Bandera
int estadoTransporte = FALSE;
int cant = 0;   //Cantidad de caracteres a enviar
int i = 0;
unsigned char bufferRX [TAM_ARREGLO];
unsigned char bufferTX [TAM_ARREGLO];
volatile unsigned char uart_update;


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
    if (primerCaracter)
        bufferRX[0] = U2RXREG;
        
    if (bufferRX[0] == SOF){    
        primerCaracter = FALSE;
        if (i == 1) {           //Si es el primer caracter recibido
            cant = U2RXREG;     //Asigna la cantidad de caracteres a transmitir
        }
        
        if ((cant - 1) >= i){       //Si quedan caracteres a cargar en el arreglo
            bufferRX[i] = U2RXREG;   //Cargo el caracter en el arreglo
            if ((cant - 1) == i){    //Si se transmitieron todos los caracteres
                primerCaracter = TRUE;
                
            }
        }           
        i = i + 1;                  //Incremento el indice del arreglo  
    }
}

//Rutina de INT para transmisión (no hace nada)
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
	//Atención. Se debe modificar para que no emita reiteradamente
	//caracteres
    IFS1bits.U2TXIF = 0;            //Bajo la flag de interrupcion 
    if((cant != 0)){                //Si no transmitio todos los valores 
        U2TXREG = bufferRX[i];  //Simple ECO
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

	// Load a value into Baud Rate Generator.  Example is for 9600.
	U2BRG = BRGVAL;	// 40Mhz osc, 9600 Baud

	IPC7 = 0x4400;	// Mid Range Interrupt Priority level, no urgent reason

	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on
	U2STAbits.UTXEN = 1;	// Empieza a transmitir. Se dispara el Flag TXIF

	IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}
