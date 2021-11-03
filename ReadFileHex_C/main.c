#include <stdio.h>
#include <stdlib.h> //malloc function
#include <stdbool.h>

#define SAVE_MIDI_COMMANDS true
#define PRINT_MIDI_COMMANDS false
#define CONVERT_MIDI true

// **********************
// typdefinitionen
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

// ***********************
// funktionen
int readByte()
{
    int result = 0;
    int exponent = 0;
    int value = 0;

    return result;
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

void push(midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    midi_t * current = head;
    while (current->next != NULL) {
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

void firstPush(midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    midi_t * current = head;

    /* now we can add a new variable */
    //current->next = (midi_t *) malloc(sizeof(midi_t));
    
    current->dataStart    = dataStart;
    current->dataEnd      = dataEnd;
    current->statusByte   = statusByte;
    current->dataByte     = dataByte;
    current->velocityByte = velocityByte;
    current->ticksFirst   = ticksFirst;
    current->ticksSecond  = ticksSecond;
    current->next = NULL;
}

// ***********************
// main
int main()
{
    // linked list how to
    // https://www.youtube.com/watch?v=DneLxrPmmsw
    // https://www.c-howto.de/tutorial/arrays-felder/speicherverwaltung/
    // https://www.codevscolor.com/c-program-create-iterate-linked-list

    bool saveMidiCommands   = SAVE_MIDI_COMMANDS;
    bool convertData        = CONVERT_MIDI;
    bool printMidiCommands  = PRINT_MIDI_COMMANDS;
    bool saveMidiData       = SAVE_MIDI_COMMANDS;

    midiConv_t midiConvArray[20] =
    {
        {36,24,"KICK","SSD5","GGD4"},
        {38,26,"SNARE","SSD5","GGD4"},
        {46,26,"TOM_LO","SSD5","GGD4"},
        {57,56,"TOM_HI","SSD5","GGD4"},
        {55,54,"HIHAT","SSD5","GGD4"},
        {42,23,"CRASH_LE","SSD5","GGD4"},
        {48,41,"CRASH_RI","SSD5","GGD4"},
        {50,28,"RIDE","SSD5","GGD4"},
        {43,41,"RIDE","SSD5","GGD4"},
        {24,24,"KICK","SSD5","GGD4"},
        {26,26,"SNARE","SSD5","GGD4"},
        {60,43,"HH_CL_2","SSD5","GGD4"},
        {66,42,"HH_CL_1","SSD5","GGD4"},
        {51,61,"RIDE","SSD5","GGD4"},
        {54,42,"HH_CL_1","SSD5","GGD4"},
        {56,61,"RIDE","SSD5","GGD4"},
        {40,47,"RIDE","SSD5","GGD4"},
        {65,56,"RIDE","SSD5","GGD4"},
        {59,54,"RIDE","SSD5","GGD4"},
        {47,47,"RIDE","SSD5","GGD4"}
    };

    // zeiger auf ersten speicherwert
    midi_t *head = NULL, *current;
    head = (midi_t *)malloc(sizeof(midi_t));
   
    FILE *ptr;
    ptr = fopen("/Users/alexandermathieu/Coding/TestArea/Midi/Coding.mid","rb");  // r for read, b for binary
    if (ptr == NULL)
    {
        perror("Error opening file");
        return(-1);
    }

    // get fileSize
    fseek(ptr, 0, SEEK_END);
    int bufferSize = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);

    printf("filesize: %d\n", bufferSize);

    unsigned char buffer[bufferSize];
    
    // erster Link muss current beschreiben, ab dem zweiten current -> next
    bool firstLink = true;
    fread(buffer,sizeof(buffer),1,ptr);
    
    // funktion schreiben!
    double headerDeltaTime = 256*buffer[12]+buffer[13];
    
    
    
    
    
    // read midi command stuff
    int dataStart       = 0;
    int dataEnd         = 0;
    int statusByte      = 0;
    int dataByte        = 0;
    int velocityByte    = 0;
    int ticksFirst      = 0;
    int ticksSecond     = 0;
    
    int ticksAbsolut    = 0;
    int ticksSecondTime = 0;

    for (int i = 0; i<bufferSize; i++) // buffersize variable einsetzen
    {
        // exit condition funzt nicht, da mehrmals möglich
        if (buffer[i] == 0xFF && buffer[i+1] == 0x2F) // midibefehl "Ende Midi-Datei"
        {
            printf("End of Track\n\n");
            //break;
        }

        if (buffer[i] == 0x80 || buffer[i] == 0x90) // note on || note off
        {
            if (convertData == true)
            {
                for (int u = 0; u < (int)(sizeof(midiConvArray)/sizeof(midiConvArray[0])); u++)
                {
                    if (buffer[i+1] == midiConvArray[u].noteBefore)
                    {
                        buffer[i+1] = midiConvArray[u].noteAfter;
                        printf("found note: %s \n",midiConvArray[u].instrument);
                        break;
                    }
                }
            }
            dataStart = i-1;
            dataEnd = dataStart+3;

            statusByte      = buffer[i];
            dataByte        = buffer[i+1];
            velocityByte    = buffer[i+2];
            ticksFirst      = buffer[i-1];
            
            int checkValue = (int)(unsigned char)buffer[i+3]; // gibt es zweiten Wert für Ticks?
                    
            switch(checkValue)
            {
                case 0x00:
                    ticksSecond = 0;
                    break;
                case 0x40:
                    // /2 delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    ticksSecondTime = headerDeltaTime/2;
                    dataEnd += 1;
                    break;
                case 0x80:
                    ticksSecond = 0;
                    break;
                case 0x81:
                    // *delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    ticksSecondTime = headerDeltaTime;
                    break;
                case 0x82:
                    // *2*delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    ticksSecondTime = headerDeltaTime*2;
                    break;
                case 0x83:
                    // *3*delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    ticksSecondTime = headerDeltaTime*3;
                    break;
                case 0x90:
                    ticksSecond = 0;
                    break;
                case 0xFF:
                    ticksSecond = 0;
                    break;
                default:
                    ticksSecond = (int)(unsigned char)buffer[i+2];
                    break;
            }

            if (saveMidiCommands == true)
            {
                pushNew(firstLink,head,dataStart,dataEnd,statusByte,dataByte,velocityByte,ticksFirst,ticksSecond);
            }
            
 
            
            
            ticksAbsolut += ticksFirst + ticksSecondTime;
            printf("TicksAbsolut: %d\n",ticksAbsolut);
                
            // writing first time in list
            if (firstLink == true)
            {
                firstLink = false;
            }

            dataStart       = 0;
            dataEnd         = 0;
            statusByte      = 0;
            dataByte        = 0;
            velocityByte    = 0;
            ticksFirst      = 0;
            ticksSecond     = 0;
            ticksSecondTime = 0;
        }
    }

    if (printMidiCommands == true)
    {
        print_list(head);
    }

       

    if (saveMidiData == true)
    {
        // print that shit
        /*
        for (int i = 0; i<bufferSize; i++)
        {
            printf("%02x\n", buffer[i]); // prints a series of bytes
        }*/

        FILE *write_ptr;
        write_ptr = fopen("/Users/alexandermathieu/Coding/TestArea/Midi/test3.mid","wb");  // w for write, b for binary
        fwrite(buffer,sizeof(buffer),1,write_ptr);
        fclose(write_ptr);
        
    }
    return 0;
}

