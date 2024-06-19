
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
#include "valoresComunes.h"

//Caracteres para la comunicacion
#define A 0x41
#define B 0x42
#define C 0x43
#define D 0x44
#define E 0x45
#define F 0x46
#define G 0x47
#define Z 0x5A
//Valores del paquete
#define SEC 0x80
#define DST 2
#define SRC 3

//Valores para calculo de velocidad
#define DISTANCIA 40.0
#define FACTOR_CONVERSION (3600000/100000)

extern unsigned char bufferRX[TAM_ARREGLO];
extern unsigned char bufferTX [TAM_ARREGLO];
extern unsigned char qty; 
extern unsigned char j;
extern unsigned int contadorTMR6;


//Informacion de un vehiculo
struct vehiculo {
    unsigned char hora;
    unsigned char minutos;
    unsigned char segundos;
    unsigned char velocidad;
    unsigned char cantEjes;
}; 

//Arreglo de vehiculos
struct vehiculo registroVehiculos[TAM_ARREGLO];

//Contadores
unsigned int counterTog = 0;
unsigned char indice_vehiculos = 0;
unsigned char vehiculosDosEjes = 0;
unsigned char vehiculosMasEjes = 0;

//Datos DATALOGGER
unsigned char horaVehiculo = 0;
unsigned char minutoVehiculo = 0;
unsigned char segundoVehiculo = 0;
unsigned char ejes = 0;
unsigned int velocidad = 0;

//indices
unsigned char i = 0;
unsigned char k =0;
unsigned int contador =0;

//Banderas
int primeraMedicion = TRUE;
int estadoAplicacion = FALSE;
extern int estadoTransporte;

//Comunicacion UART
unsigned int BCCH_calculado = 0; 
unsigned int BCCL_calculado = 0; 
unsigned char BCCH_recibido = 0;
unsigned char BCCL_recibido = 0;
unsigned int checksum_calculado = 0;
unsigned char hora_recibida;


void cargarVehiculos(void){
    
    registroVehiculos[indice_vehiculos].segundos = 55;
    registroVehiculos[indice_vehiculos].minutos = 45;
    registroVehiculos[indice_vehiculos].hora = 15;
    registroVehiculos[indice_vehiculos].cantEjes = 2;
    registroVehiculos[indice_vehiculos].velocidad = 40; 
    indice_vehiculos++;
    vehiculosDosEjes++;

    
    registroVehiculos[indice_vehiculos].segundos = 15;
    registroVehiculos[indice_vehiculos].minutos = 28;
    registroVehiculos[indice_vehiculos].hora = 16;
    registroVehiculos[indice_vehiculos].cantEjes = 5;
    registroVehiculos[indice_vehiculos].velocidad = 33; 
    indice_vehiculos++;
    vehiculosMasEjes++;
    
    
    registroVehiculos[indice_vehiculos].segundos = 26;
    registroVehiculos[indice_vehiculos].minutos = 7;
    registroVehiculos[indice_vehiculos].hora = 21;
    registroVehiculos[indice_vehiculos].cantEjes = 2;
    registroVehiculos[indice_vehiculos].velocidad = 95; 
    indice_vehiculos++;
    vehiculosDosEjes++;

    
    registroVehiculos[indice_vehiculos].segundos = 12;
    registroVehiculos[indice_vehiculos].minutos = 35;
    registroVehiculos[indice_vehiculos].hora = 21;
    registroVehiculos[indice_vehiculos].cantEjes = 3;
    registroVehiculos[indice_vehiculos].velocidad = 50; 
    vehiculosMasEjes++;

    
  
    bufferTX[0] = 0xFE;
    bufferTX[1] = 0x08;    
    bufferTX[2] = 0x03;
    bufferTX[3] = 0x02;
    bufferTX[4] = 0x80;
    bufferTX[5] = 0x42;
    bufferTX[6] = 0x81;
    bufferTX[7] = 0x4C;    

}


void delay (void){
    for (contador = 0; contador < 100; contador++);
}

void actualizarDatosPuerto(void){
        PORTB = indice_vehiculos;
        delay();
        PORTB = velocidad;
        delay();
        PORTB = segundoVehiculo;
        delay();
        PORTB = minutoVehiculo;
        delay();
        PORTB = horaVehiculo;
}


void calcularVelocidad (void){
    velocidad = ((DISTANCIA / contadorTMR6) * FACTOR_CONVERSION);       
    
    if (velocidad > 60){
        PORTAbits.RA0 ^= 1;  //Accionamos camara fotografica
    }
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt( void ){
    IFS1bits.CNIF = 0;
    
    if (PORTDbits.RD13){
      
        if (PORTDbits.RD6){
            ejes++;
            if (primeraMedicion){
                T6CONbits.TON = 1;
                segundoVehiculo = seconds;
                minutoVehiculo = minutes;
                horaVehiculo = hours;            
            }
        }
        if (PORTDbits.RD7){               
            if (primeraMedicion){
                T6CONbits.TON = 0;
                calcularVelocidad();
                TMR6 = 0;
                contadorTMR6 = 0;
                primeraMedicion = FALSE;
            }
        }
    }   
    else if (! primeraMedicion){  //Si se realizo la primer medicion.
        
        registroVehiculos[indice_vehiculos].segundos = segundoVehiculo;
        registroVehiculos[indice_vehiculos].minutos = minutoVehiculo;
        registroVehiculos[indice_vehiculos].hora = horaVehiculo;
        registroVehiculos[indice_vehiculos].cantEjes = ejes;
        registroVehiculos[indice_vehiculos].velocidad = velocidad;
        
        if (ejes == 2)
            vehiculosDosEjes++;
        else
            vehiculosMasEjes++;
        
        ejes = 0;
        indice_vehiculos++;
        primeraMedicion = TRUE;   
        actualizarDatosPuerto();
    }           
}



void calcularChecksum(unsigned char* arreglo){
    BCCH_calculado = 0;
    BCCL_calculado = 0;
    
    for(i = 0; i < (arreglo[1] - 2) ; i++){
        if ((i % 2) == 0){
            BCCH_calculado += arreglo[i];
        }
        else{
            BCCL_calculado += arreglo[i];
        }
    }

    checksum_calculado = BCCH_calculado << 8;
    checksum_calculado = checksum_calculado + BCCL_calculado;

    BCCH_calculado = (checksum_calculado & 0xFF00) >>8;
    BCCL_calculado = checksum_calculado & 0x00FF;
}


void capaTransporte (void){
    BCCH_recibido = bufferRX[qty - 2];
    BCCL_recibido = bufferRX[qty -1];   
    
    if (bufferRX[2] == 3){
        calcularChecksum (bufferRX);
        if (BCCH_calculado != BCCH_recibido || BCCL_calculado != BCCL_recibido){   
            bufferRX[5] = Z; //Asignamos valor invalido para enviar un NACK.
        }
        estadoAplicacion = TRUE; 
    }
 
    estadoTransporte = FALSE;
}

void capaAplicacion (void){
    if (bufferRX[5] != F && bufferRX[5] != G){
        bufferTX[0] = SOF;
        bufferTX[2] = bufferRX[3];
        bufferTX[3] = SRC;
        bufferTX[4] = SEC;
        j = 5;

        switch (bufferRX[5]){
        case A:
            bufferTX[j] = bufferRX[j];
            j++;
            bufferTX[j] = vehiculosDosEjes;
            break;
        case B:
            indice_vehiculos = 0;
            vehiculosDosEjes = 0;
            vehiculosMasEjes = 0;
            bufferTX[j] = F; //Asignamos un ACK
            break;

        case C:
            bufferTX[j] = C;
            j++;
            bufferTX[j] = vehiculosMasEjes;
            break;
        
        case D:

            hora_recibida = bufferRX[j + 1];
            if (hora_recibida > 24){
                bufferTX[j] = G; //Asignamos un NACK
            }
            else {
                bufferTX[j] = D;
                k = 0;

                while (k <= indice_vehiculos && registroVehiculos[k].hora <= hora_recibida){
                    if (registroVehiculos[k].hora == hora_recibida){
                        j++;
                        bufferTX[j] = hora_recibida; 
                        j++;
                        bufferTX[j] = registroVehiculos[k].minutos;
                        j++;     
                        bufferTX[j] = registroVehiculos[k].segundos;
                        j++;    
                        bufferTX[j] = registroVehiculos[k].velocidad;
                        j++; 
                        bufferTX[j] = registroVehiculos[k].cantEjes;         
                    }
                    k++;
                }
            }
     
            break;
        
        case E:
              PORTAbits.RA0 ^= 1;   
              bufferTX[j] = F;
            break;    
                
        default:
            bufferRX[j] = G; //ENVIAR NACK 
        }
        
        bufferTX[1] = j + 3;  //2 posiciones del BCC y 1 dado que el arreglo comienza en 0
        calcularChecksum (bufferTX);
        bufferTX[j + 1] = BCCH_calculado;
        bufferTX[j + 2] = BCCL_calculado; 
        qty = bufferTX[1];       
    }
    if (bufferRX[5] != F){  //Si es distinto de un ACK comienza la transmision
        IFS1bits.U2TXIF = 1;
    }
    k = 0;
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
    cargarVehiculos();
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



