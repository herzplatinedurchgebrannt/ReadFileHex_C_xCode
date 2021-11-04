//
//  lxMidi.c
//  ReadFileHex_C
//
//  Created by Alexander Mathieu on 04.11.21.
//

#include "lxMidi.h"
#include <stdio.h>
#include <stdlib.h> //malloc function
#include <stdbool.h>
#include <string.h>

#define OK 0
#define ERROR_BUFFER_TOO_SMALL 1


// ***********************
// *** FUNCTIONS

int readByteBigEndian(char* input, int startByte, int size)
{
    int result = 0;
    
    /*
    for (int y = 18; y < 22; y++){
        
        printf("%d\n",(int)(unsigned char)input[y]);
    }*/
    
    if (size == 4)
    {
        // big endian
        result = (int)(unsigned char)input[startByte+3] | (int)(unsigned char)input[startByte+2]<<8 | (int)(unsigned char)input[startByte+1]<<16 | (int)(unsigned char)input[startByte]<<24;
    }
    else if (size == 2)
    {
        // big endian
        result = (int)(unsigned char)input[startByte+1] | (int)(unsigned char)input[startByte]<<8;
    }
    else
    {
        // ....
        return 1;
    }
    /*
    for (int z = startByte; z < startByte+size; z++)
    {
        printf("%d\n", (int)(unsigned char)input[z]);
        result += (int)(unsigned char)input[z];
    }*/
    
    return result;
}

int readByteLittleEndian(unsigned char* input, int startByte, int size)
{
    int result = 0;
    
    
    if (size == 4)
    {
        // big endian
        result = (int)(unsigned char)input[startByte] | (int)input[startByte+1]<<8 | (int)input[startByte+2]<<16 | (int)input[startByte+3]<<24;
    }
    else if (size == 2)
    {
        // big endian
        result = (int)(unsigned char)input[startByte] | (int)(unsigned char)input[startByte+1]<<8;
    }
    else
    {
        // ....
        return 1;
    }
    /*
    for (int z = startByte; z < startByte+size; z++)
    {
        printf("%d\n", (int)(unsigned char)input[z]);
        result += (int)(unsigned char)input[z];
    }*/
    
    return result;
}




int readText(char* input /*Eingabe-Parameter*/, char* output /*Rückgabe-Parameter*/, int outputLen /*Größe von output*/, int startByte, int size)
{
    
    char temp[13] = "";
    
    int loopCount = 0;
    
    for (int z = startByte; z < startByte+size; z++)
    {
        //printf("%c\n", input[z]);
        temp[loopCount] = (unsigned char)input[z];
        loopCount += 1;
    }
        
    if (strlen(temp) < outputLen)    // passt temp in output?
    {
        strncpy(output, temp, size);            // temp in output kopieren
        return OK;                       // OK zurückgeben
    }
    else
    {
        return ERROR_BUFFER_TOO_SMALL;   // Fehler zurückgeben
    }
}



void print_list(midi_t * head) {
    midi_t * current = head;

    while (current != NULL) {
        printf("dataStart:      %d\n", current->dataStart);
        printf("dataEnd:        %d\n", current->dataEnd);
        printf("statusByte:     %d\n", current->statusByte);
        printf("dataByte:       %d\n", current->dataByte);
        printf("velocityByte:   %d\n", current->velocityByte);
        printf("ticksFirst:     %d\n", current->ticksFirst);
        printf("ticksSecond:    %d\n", current->ticksSecond);
        printf("------------------\n");
        current = current->next;
    }
}



void pushNew(bool firstLink, midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    if (firstLink == false)
    {
        midi_t * current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        /* now we can add a new variable */
        current->next = (midi_t *) malloc(sizeof(midi_t));

        current->next->dataStart    = dataStart;
        current->next->dataEnd      = dataEnd;
        current->next->statusByte   = statusByte;
        current->next->dataByte     = dataByte;
        current->next->velocityByte = velocityByte;
        current->next->ticksFirst   = ticksFirst;
        current->next->ticksSecond  = ticksSecond;
        current->next->next = NULL;
    }
    else
    {
        midi_t * current = head;
        
        current->dataStart    = dataStart;
        current->dataEnd      = dataEnd;
        current->statusByte   = statusByte;
        current->dataByte     = dataByte;
        current->velocityByte = velocityByte;
        current->ticksFirst   = ticksFirst;
        current->ticksSecond  = ticksSecond;
        current->next = NULL;
    }
}

