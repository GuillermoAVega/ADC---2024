
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

//Valores del paquete
#define SEC 0x80
#define DST 2
#define SRC 3

//Valores para calculo de velocidad
#define DISTANCIA 40
#define FACTOR_CONVERSION (3600000/100000)


struct vehiculo {
    unsigned char hora;
    unsigned char minutos;
    unsigned char segundos;
    unsigned char velocidad;
    unsigned char cantEjes;
}; 


struct vehiculo registroVehiculos[TAM_ARREGLO];
extern unsigned char bufferRX[TAM_ARREGLO];
extern unsigned char bufferTX [TAM_ARREGLO];
extern unsigned char qty; 
extern unsigned char k;
extern int contadorTMR6;

int counterTog = 0;
unsigned char indice_vehiculos = 0;
unsigned char vehiculosDosEjes = 0;
unsigned char vehiculosMasEjes = 0;
unsigned char cantidadCaracteres = 0;

//Datos 
unsigned char horaAuto = 0;
unsigned char minutoAuto = 0;
unsigned char segundoAuto = 0;
unsigned char ejes = 0;
unsigned char velocidad = 0;

//indices
unsigned char j = 0;
unsigned char i = 0;
unsigned char p =0;

//Banderas
int primeraMedicion = TRUE;
int checksumCorrecto = FALSE;
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
    registroVehiculos[indice_vehiculos].hora = 20;
    registroVehiculos[indice_vehiculos].cantEjes = 2;
    registroVehiculos[indice_vehiculos].velocidad = 95; 
    indice_vehiculos++;
    vehiculosDosEjes++;

    
    registroVehiculos[indice_vehiculos].segundos = 12;
    registroVehiculos[indice_vehiculos].minutos = 35;
    registroVehiculos[indice_vehiculos].hora = 20;
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




void calcularVelocidad (void){
    velocidad = (DISTANCIA / contadorTMR6) * FACTOR_CONVERSION;       
    
    if (velocidad > 60){
        PORTAbits.RA0 ^= 1;
    }
}

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

    checksum_calculado = checksum_calculado;
    checksum_calculado = BCCH_calculado << 8;
    checksum_calculado = checksum_calculado + BCCL_calculado;

    BCCH_calculado = (checksum_calculado & 0xFF00) >>8;
    BCCL_calculado = checksum_calculado & 0x00FF;
}



void capaTransporte (void){
    k = 0;
    cantidadCaracteres = bufferRX[1];
    BCCH_recibido = bufferRX[qty - 2];
    BCCL_recibido = bufferRX[qty -1];   //cantidadCaracteres
    
    if (bufferRX[2] == 3){
        calcularChecksum (bufferRX);
        if (BCCH_calculado != BCCH_recibido || BCCL_calculado != BCCL_recibido){   
            bufferRX[5] = G; //Asignamos un NACK
        }
    }
    estadoTransporte = FALSE;
    estadoAplicacion = TRUE; 
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
        case E:
              PORTAbits.RA0 ^= 1;            
        case B:
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
                p = 0;

                while (p <= indice_vehiculos && registroVehiculos[p].hora <= hora_recibida){
                    if (registroVehiculos[p].hora == hora_recibida){
                        j++;
                        bufferTX[j] = hora_recibida; 
                        j++;
                        bufferTX[j] = registroVehiculos[p].minutos;
                        j++;     
                        bufferTX[j] = registroVehiculos[p].segundos;
                        j++;    
                        bufferTX[j] = registroVehiculos[p].velocidad;
                        j++; 
                        bufferTX[j] = registroVehiculos[p].cantEjes;         
                    }
                    p++;
                }
            }
     
            break;
        default:
            bufferRX[j] = G; //ENVIAR NACK 
        }

        bufferTX[1] = j + 3;
        calcularChecksum (bufferTX);
        bufferTX[j + 1] = BCCH_calculado;
        bufferTX[j + 2] = BCCL_calculado; 
        qty = bufferTX[1];       

    }
    if (bufferRX[5] != F){
        IFS1bits.U2TXIF = 1;
    }    
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

// AGREGAR UN SEGUN CON LOS POSIBLES ESTADOS


