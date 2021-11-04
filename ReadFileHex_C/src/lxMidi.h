//
//  lxMidi.h
//  ReadFileHex_C
//
//  Created by Alexander Mathieu on 04.11.21.
//

#ifndef lxMidi_h
#define lxMidi_h

#include <stdio.h>
#include <stdlib.h> //malloc function
#include <stdbool.h>
#include <string.h>

#endif /* lxMidi_h */


// **********************
// *** TYPEDEF

typedef struct node
{
    int dataStart;
    int dataEnd;
    int statusByte;
    int dataByte;
    int velocityByte;
    int ticksFirst;
    int ticksSecond;
    char command[11];
    struct node *next; /* Zeiger auf das nächste Element */
} midi_t;


typedef struct
{
    int noteBefore;
    int noteAfter;
    char instrument[11];
    char pluginBefore[11];
    char pluginAfter[11];
} midiConv_t;


int readByteBigEndian(char* input, int startByte, int size);

int readByteLittleEndian(unsigned char* input, int startByte, int size);

int readText(char* input /*Eingabe-Parameter*/, char* output /*Rückgabe-Parameter*/, int outputLen /*Größe von output*/,int startByte, int size);

void print_list(midi_t * head);

void pushNew(bool firstLink, midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond);


