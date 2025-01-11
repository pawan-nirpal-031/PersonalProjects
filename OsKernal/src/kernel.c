#include "kernel.h"

void kernelMain(){
    char * videoMem = (char *) 0xb8000;
    
    for(int i =0; i<11; i+=2){
        videoMem[i] = (char)(i+65); // Byte 1 is the character
        videoMem[i+1] = 2; // Byte 2 is the color
    }
}