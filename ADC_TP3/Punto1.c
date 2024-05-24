/*
 * File:   Punto1.c
 * Author: Guille
 *
 * Created on 14 de mayo de 2024, 18:12
 */


#include "xc.h"
#define UMBRAL 50

int contador;
unsigned int val1;
signed int val2;
unsigned int val3;

void config(){
    AD1PCFGL = 0xFF9B;
    AD1PCFGH = 0xFFFF;
    TRISD = 0xFFFF;
    TRISE = 0x0000;
    AD1CON1bits.SSRC = 7;
}

void configPulsador(int valCH0SA, int valAD12B, int valFORM){
    AD1CHS0bits.CH0SA = valCH0SA;
    AD1CON1bits.AD12B = valAD12B;
    AD1CON1bits.FORM = valFORM;
    
    AD1CON1bits.ADON = 1;	
    AD1CON1bits.SAMP = 1;
}

int valor_abs (int x){
    if (x < 0) {
        x = x * -1;
    }
    return x;
}

void pinAN2 (void){
    configPulsador(2,0,0);
    while (!AD1CON1bits.DONE);
    val1 = ADC1BUF0;
    
    if (val1 > UMBRAL){
        contador++;
        PORTE = contador;
    }
}

void pinAN5 (void){
    configPulsador(5,1,1);
    while (!AD1CON1bits.DONE);
    val2 = ADC1BUF0;
    if (valor_abs(val2) > UMBRAL){
        contador++;
        PORTE = contador;  
    }
}

void pinAN6(void){
    configPulsador(6,1,0);
    while (!AD1CON1bits.DONE);
    val3 = ADC1BUF0;
    
    if (val3 > UMBRAL){
        contador++;
        PORTE = contador;  
    }
}

int main(void) {
    
    config();
    
    while (1){
        if (PORTDbits.RD6){
            pinAN2();
        }    
        else if (PORTDbits.RD7){
            pinAN5();
        }
        else if (PORTDbits.RD13){
            pinAN6();
        }
    }   
    return 0;
}

