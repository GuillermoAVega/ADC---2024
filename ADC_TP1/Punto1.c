/*
 * File:   Practica 1.c
 * Author: Guillermo Vega, Juan Riquelme, Agustin Vargas
 *
 * Created on 14 de marzo de 2024, 14:13
*/

#include "xc.h"

int PS1 = 0;
int PS2 = 0;

int main(void) {
    config ();
    
    while (1){
        programa ();        
    }
    return 0;
}

void config()
{
    TRISAbits.TRISA0 = 0;  //Configura el pin A0 como salida 
    TRISAbits.TRISA1 = 0;  //Configura el pin A1 como salida
    TRISDbits.TRISD12 = 1; //Configura el pin D12 como entrada
    TRISDbits.TRISD13 = 1; //Configura el pin D13 como entrada
}

void programa ()
{ 
    PS1 = PORTDbits.RD12; //Le asigna a la variable PS1 el valor de la lectura del pin RD12
    PS2 = PORTDbits.RD13; //Le asigna a la variable PS2 el valor de la lectura del pin RD13
    
    if ((PS1) && (PS2)){  //Si ninguno de los pulsadores esta presionado
        PORTAbits.RA0 = 1; //Enciende el LED LD1
        PORTAbits.RA1 = 1; //Enciende el LED LD2
    }
    else {
        if (!(PS1)) // Si se presiona el pulsador PS1
            PORTAbits.RA1 = 0; //Apaga el LD2
        if (!(PS2))   //Si se presiona el pulsador PS2
            PORTAbits.RA0 = 0; //Apaga el LD1
    }
}
