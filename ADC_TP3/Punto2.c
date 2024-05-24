/*
 * File:   Punto2.c
 * Author: Vargas, Vega, Riquelme
 *
 * Created on May 14, 2019, 6:27 PM
 */


#include "xc.h"
#include "config.h"

int mayorAN0 = 0;
int menorAN0 = 0xFFFF;
int mayorAN1 = 0;
int menorAN1 = 0xFFFF;
int promedio = 0;
int i = 0;
int j = 1;
int procesarValores;
int entrada = 0;
int contBuffer = 0;

extern unsigned int BufferA[maxBuffer];
extern unsigned int BufferB[maxBuffer];

void __attribute__((interrupt, auto_psv)) _DMA0Interrupt(void) {
    IFS0bits.DMA0IF = 0; //Baja la flag de interrupcion por DMA
    procesarValores = 1;  //Hay valores para procesar
}


void proceso (unsigned int* arreglo){ 

    for (i = 0; i < maxBuffer; i++){
        
        if (entrada == 0) {
            arreglo[i] = arreglo[i] & 0x00FF;  //Filtar valores ingresados por AN0
        }
        else{
            arreglo[i] = (arreglo[i] & 0x000F)  | 0x0010; //Filtrar valores ingresados por AN1
        }
        
        promedio += arreglo[i];  //Acumula los valores del buffer

        if (i % 2 == 0) {
            if (mayorAN0 < arreglo[i]){
                mayorAN0 = arreglo[i];        //Actualiza el valor mayor obtenido en la entrada AN0 luego del filtro
            } 
            if (menorAN0 > arreglo[i]) {
                menorAN0 = arreglo[i];        //Actualiza el valor menor obtenido en la entrada AN0 luego del filtro
            }
        }
        else{
            if (mayorAN1 < arreglo[i]){
                mayorAN1 = arreglo[i];        //Actualiza el valor mayor obtenido en la entrada AN1 luego del filtro
            }
            if (menorAN1 > arreglo[i]) {
                menorAN1 = arreglo[i];        //Actualiza el valor menor obtenido en la entrada AN1 luego del filtro.      
            }
        }
        entrada ^= 1;                        //Variable para determinar a que entrada corresponde el valor leido.
    }
    promedio = promedio / maxBuffer;         //Promedio de los valores ingresados en el Buffer
    procesarValores = 0;                     //No quedan valores a procesar.
    contBuffer ^= 1;                         //Variable que indica con que Buffer se está trabajando
}

//Funcion para simular un delay
void delay(void){
    for(j = 0; j < 5; j++);               
}

int main(void) {
    config();
    while(1) {
        if (procesarValores){
            if (contBuffer == 0){
                proceso(BufferA);       //Proceso los valores del bufferA
            }
            else{
                proceso(BufferB);       //Proceso los valores del bufferB
            }
        }
        if (PORTDbits.RD6){             //Si se pulsa el pin 6 del puerto D
            if (promedio % 2 == 0){     //Muestra la secuencia menor/promedio/mayor de cada entrada si el promedio es impar
                PORTB = menorAN0;
                delay();
                PORTB = menorAN1;
                delay();
                PORTB = promedio;
                delay();
                PORTB = mayorAN0;
                delay();
                PORTB = mayorAN1;
            }
        }
        if (PORTDbits.RD13){            //Si se pulsa el pin 13 del puerto D
            if (promedio % 2 != 0){     //Muestra la secuencia menor/promedio/mayor de cada entrada si el promedio es impar
                PORTB = mayorAN0;
                delay();
                PORTB = mayorAN1;
                delay();
                PORTB = promedio;
                delay();
                PORTB = menorAN0;
                delay();
                PORTB = menorAN1;
            }
        }
        
    }
    return 0;
}