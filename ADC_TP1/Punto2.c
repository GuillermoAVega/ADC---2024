/*
 * File:   Automatismo.c
 * Author: Guillermo Vega, Juan Riquelme, Agustin Vargas
 *
 * Created on 19 de marzo de 2024, 18:11
*/ 

#include "xc.h"
#define TRUE 1
#define FALSE 0
#define UMBRAL_TEMPERATURA 15728


int esta_apagado = TRUE; //Declaro bandera de estado del sistema de calefaccionado
int accesos_cerrados;  //Declaro variable contador de accesos cerrados
int temperatura;  //Declaro variable del termostato.
int condiciones;  //Declaro bandera de estado de las condiciones para encender el sistema de calefaccionado

typedef union {
    struct {
        //Sensores de los accesos
        unsigned int sensor_1:1; 
        unsigned int sensor_2:1;
        unsigned int sensor_3:1;
        unsigned int sensor_4:1;
        unsigned int sensor_5:1;

        unsigned int gas:1;  //Sensor de presion de gas
        unsigned int energia:1; //Sensor de deteccion de energia
        unsigned int :9;
    };
    unsigned int completo;  //Acceso al registro completo.
} t_sistemaSensado;

t_sistemaSensado sensado;


int main(void) {
    config (); //Configuracion de puertos
    
    while (1){
        entradas (); //Lee las entradas
        procesamiento(); //Evalua las entradas
        salidas(); //Escribe las salidas
    }
    return 0;
}


void config()
{
    TRISA = 0xFFFF; //Configura el puerto A como entrada
    TRISB = 0xFFFF;  //Configura el puerto B como entrada 
    TRISCbits.TRISC1 = 0; //Configura el Pin 1 del puerto C como salida
    TRISCbits.TRISC2 = 0; //Configura el Pin 2 del puerto C como salida
    AD1PCFGH = 0xFFFF; 
    AD1PCFGL = 0xFFFF;
    AD2PCFGL = 0xFFFF;
}

void entradas() 
{
    sensado.completo = PORTA; //Copia la lectura de los sensores en la variable temperatura
    temperatura = PORTB; //Copia la temperatura del termostato en la variable temperatura
}

void procesamiento(){
    accesos_cerrados = 0; //Setea en 0 el contador de accesos cerrados
    
    if (sensado.sensor_1) //Verifica el estado del sensor de apertura de accesos 1
        accesos_cerrados++;  //Incrementa el contador de accesos cerrados
    if (sensado.sensor_2) //Verifica el estado del sensor de apertura de accesos 2
        accesos_cerrados++; //Incrementa el contador de accesos cerrados
    if (sensado.sensor_3) //Verifica el estado del sensor de apertura de accesos 3
        accesos_cerrados++; //Incrementa el contador de accesos cerrados
    if (sensado.sensor_4) //Verifica el estado del sensor de apertura de accesos 4
        accesos_cerrados++; //Incrementa el contador de accesos cerrados
    if (sensado.sensor_5) //Verifica el estado del sensor de apertura de accesos 5
        accesos_cerrados++; //Incrementa el contador de accesos cerrados
    
    if (accesos_cerrados >= 3 && temperatura < UMBRAL_TEMPERATURA && sensado.gas && sensado.energia) //Verifica que se cumplan las condiciones para encender el sistema de calefaccionado
        condiciones = TRUE; //Pone en estado alto la bandera de condiciones
    else
        condiciones = FALSE; //Pone en estado bajo la bandera de condiciones
    
}  
    

void salidas (){
    if (esta_apagado && condiciones){
        PORTCbits.RC1 = 1;  //Prende el motor que impulsa aire caliente
        PORTCbits.RC2 = 1;  //Abre la valvula de salida de gas
        esta_apagado = FALSE; //Almacena la variable de estado anterior 
    }
    else if (!condiciones && !esta_apagado){
        PORTCbits.RC1 = 0;  //Apaga el motor que impulsa aire caliente
        PORTCbits.RC2 = 0;  //Apaga la valvula de salida de gas
        esta_apagado = TRUE;//Almacena la variable del estado anterior
    }  
}
 
    
    
