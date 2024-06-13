
/**********************************************************************
 FileName:        main.c
 Dependencies:    p33FJ256GP710.h
 Processor:       dsPIC33F
 Compiler:        MPLAB® C30 v2.01 or higher

 Ejemplo de funcionamiento de:
 	Timer1
 	LCD
 	PORT A Output
 	PORT D Input

 REVISION HISTORY:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Author            Date      Comments on this revision
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Richard Fischer   07/14/05  Initial Release
 Priyabrata Sinha  01/27/06  Ported to non-prototype devices 
 Ricardo López     05/24/10  Adaptación para uso general
 Sebastián Wahler  08/05/13  Nuevas adaptaciones
 Marcelo Gómez     09/03/14  Portado a MPLAB.X
 Sebastián Wahler  04/06/19  Simplificado sin Placa

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**********************************************************************/

#include "common.h"
#include <xc.h>
#include "confbits.h"
#include "p33FJ256GP710.h"
#include "config.h"

#define TRUE	1
#define FALSE 0
#define SEC 0x80
#define SOF 0xFE
#define NACK 0x47
#define DST 2
#define SRC 3
#define CANTIDAD_AUTOS 256
#define DISTANCIA 0,4
#define MINUTOS_EN_SEG 60
#define HORA_EN_SEG 3600


struct vehiculo {
    unsigned int hora;
    unsigned int minutos;
    unsigned int segundos;
    unsigned int velocidad;
    unsigned int cantEjes;
}; 


struct vehiculo registroVehiculos[CANTIDAD_AUTOS];
extern int estadoTransporte;
extern unsigned int bufferRX[256];

int counterTog = 0;
int indice_vehiculos = 0;
int velocidad = 0;
int tiempo = 0;
int vehiculosDosEjes = 0;
int vehiculosMasEjes = 0;
int horaAuto = 0;
int minutoAuto = 0;
int segundoAuto = 0;
int ejes = 0;
int primeraMedicion = TRUE;
int checksumCorrecto = FALSE;
int estadoAplicacion = FALSE;
char j = 0;
//Comunicaciones
unsigned int BCCH_calculado = 0; 
unsigned int BCCL_calculado = 0; 
unsigned int BCCH_recibido = 0;
unsigned int BCCL_recibido = 0;
unsigned int checksum_calculado = 0;
unsigned int qty_recibido = 0;

void __attribute__((interrupt, auto_psv)) _CNInterrupt( void ){
    
    if (PORTDbits.RD13){
      
        if (PORTDbits.RD6){
            ejes++;
            if (primeraMedicion){
                T4CONbits.TON = 1;
                segundoAuto = seconds;
                minutoAuto = minutes;
                horaAuto = hours;            
            }
        }
        if (PORTDbits.RD7){               
            if (primeraMedicion){
                T4CONbits.TON = 0;
                calcularVelocidad();
                primeraMedicion = FALSE;
            }
        }
    }   
    else{
        
        registroVehiculos[indice_vehiculos].segundos = segundoAuto;
        registroVehiculos[indice_vehiculos].minutos = minutoAuto;
        registroVehiculos[indice_vehiculos].hora = horaAuto;
        registroVehiculos[indice_vehiculos].cantEjes = ejes;
        registroVehiculos[indice_vehiculos].velocidad = velocidad;

        if (ejes == 2)
            vehiculosDosEjes++;
        else
            vehiculosMasEjes++;
        
        ejes = 0;
        indice_vehiculos++;
        primeraMedicion = TRUE;   
    }          
    
}

void calcularVelocidad (void){
    velocidad = contadorTMR6 ;
    // contadorTMR6 / DISTANCIA      
    
    if (velocidad > 60){
        PORTAbits.RA0 ^= 1;
    }
}

void calcularChecksum(void){
    
    for(i = 0; i < (qty_recibido - 2) ; i++){
        if ((i % 2) == 0){
            BCCH_calculado = bufferRX[i] + BCCH_calculado;
        }
        else{
            BCCL_calculado = bufferRX[i] + BCCL_calculado;
        }
    }

    checksum_calculado = checksum_calculado;
    checksum_calculado = BCCH_calculado << 8;
    checksum_calculado = checksum_calculado + BCCL_calculado;

    BCCH_calculado = (checksum_calculado & 0xFF00) >>8;
    BCCL_calculado = checksum_calculado & 0x00FF;
}



void capaTransporte (void){
    
    if (bufferRX[2] == 3){
        calcularChecksum ();
        if (BCCH_calculado != BCCH_recibido || BCCL_calculado != BCCL_recibido){   
            bufferRX[5] = NACK;
        }
    }
    estadoTransporte = FALSE;
    estadoAplicacion = TRUE; 
}

void capaAplicacion (void){
    bufferTX[0] = SOF;
    bufferTX[2] = bufferRX[3];
    bufferTX[3] = SRC;
    bufferTX[4] = SEC;
    bufferTX[5] = bufferRX[5];
    j = 6;
    
    switch (bufferRX[5]){
    
    case 0x41:
    
    case 0x42:
        
    case 0x43:
        bufferTX[j] = 
    case 0x44:
        
    case 0x45:
        
    case 0x46:
        
    case 0x47:
    
    default:
        bufferRX[5] = bufferRX[5];//ENVIAR NACK comentario por el error
    }
    //bufferTX[1] =  CARGAR EL QTY
    //calcular BCC;
    
    
    IFS1.U2TXIF = 1;
    estadoAplicacion = FALSE;
}


void UpdateClock (void)
{
    counterTog = 0;
    /* Actualizar Hora */
    seconds=seconds+1;
    if (seconds > 59)
    {
        seconds=0;
        minutes=minutes+1;
    }
    if (minutes > 59)
    {
        minutes=0;
        hours=hours+1;
    }
    if (hours == 24)
    {
        hours=0;
        indice_vehiculos = 0;
        vehiculosDosEjes = 0;
        vehiculosMasEjes = 0;
    }

}




int main ( void )
{
    config();
    // AGREGAR UNA FUNCION QUE PRECARGUE AUTOS DE ANTEMANO
    /* Loop infinito */
    while (TRUE)
    {
        if (counterTog > 3)
            UpdateClock();  //Actualizar hora del dispositivo
        
        if (estadoTransporte){
          capaTransporte ();
        }
        if (estadoAplicacion){
            capaAplicacion();
        }
    }
}

// AGREGAR UN SEGUN CON LOS POSIBLES ESTADOS


