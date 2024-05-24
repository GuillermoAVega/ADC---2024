#include "xc.h"
#include "config.h"

unsigned int BufferA[maxBuffer] __attribute__((space(dma)));
unsigned int BufferB[maxBuffer] __attribute__((space(dma)));




void initPORTD(void) {
    TRISB = 0x0000;
    TRISD = 0xFFFF;
    AD1PCFGH = 0xFFFF;
    AD1PCFGL = 0xFFFC;
    AD2PCFGL = 0xFFFF;
}

void initADC(void) {
    AD1CON1bits.SSRC = 7;
    AD1CON2bits.CSCNA = 1;
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.FORM = 0;
    AD1CON1bits.ASAM = 1;
    AD1CSSL = 0x0003;
    AD1CSSH = 0x0000;
    
    AD1CON3bits.ADCS = 8; // Tad = internal 4 Tcy
    
    AD1CON1bits.ADON = 1;
}

void initDMA(void) {
    DMA0REQ = 13;
    DMA0PAD = (unsigned int) &ADC1BUF0;
    DMA0STA = __builtin_dmaoffset(BufferA);
    DMA0STB = __builtin_dmaoffset(BufferB);
    DMA0CNT = maxBuffer - 1;
    DMA0CONbits.AMODE = 0;
    DMA0CONbits.MODE = 2;
    IFS0bits.DMA0IF = 0;
    IEC0bits.DMA0IE = 1;
    DMA0CONbits.CHEN = 1;
}

void config(void) {
    initPORTD();
    initADC();
    initDMA();
}
