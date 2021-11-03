#include <stdio.h>
#include <stdlib.h> //malloc function
#include <stdbool.h>
#include <string.h>

#define SAVE_MIDI_COMMANDS true
#define PRINT_MIDI_COMMANDS false
#define CONVERT_MIDI true

#define OK 0
#define ERROR_BUFFER_TOO_SMALL 1


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




// ***********************
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
    ptr = fopen("/Users/alexandermathieu/Coding/TestArea/Midi/new.mid","rb");  // r for read, b for binary
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
    
    
    /*
    for (int y = 18; y < 22; y++){
        
        printf("%d\n",(int)(unsigned char)buffer[y]);
    };*/
    
     
    //***********************************************************
    // READ FILE HEADER
    char headerChunk[4];
    readText(buffer, headerChunk, 5, 0, 4);
    
    int headerSize          = readByteBigEndian(buffer, 4, 4);
    int headerFileFormat    = readByteBigEndian(buffer, 8, 2);
    int headerTracks        = readByteBigEndian(buffer, 10, 2);
    int headerDeltaTime     = readByteBigEndian(buffer, 12, 2);

    // READ TRACK HEADER
    char trackChunk[4];
    readText(buffer, trackChunk, 5, 14, 4);
    
    int trackLength         = readByteBigEndian(buffer, 18, 4);
    
    
    int noteWhole           = headerDeltaTime * 4;
    int noteHalf            = headerDeltaTime * 2;
    int noteQuarter         = headerDeltaTime;
    int noteEighth          = headerDeltaTime / 2;
    int noteSixteenth       = headerDeltaTime / 4;
    int noteThirtySecond    = headerDeltaTime / 8;
    int noteSixtyFourth     = headerDeltaTime / 16;
    
    int notesBarAbsolutOld  = 0;
    int notesBarAbsolutNew  = 0;
    

    
    
    // READ MIDI COMMAND STUFF
    int dataStart       = 0;
    int dataEnd         = 0;
    int statusByte      = 0;
    int dataByte        = 0;
    int velocityByte    = 0;
    int ticksFirst      = 0;
    int ticksSecond     = 0;
    
    int ticksAbsolut    = 0;
    int ticksFirstTime  = 0;
    
    
    // skip header in loop
    for (int i = bufferSize-trackLength; i<bufferSize; i++) // buffersize variable einsetzen
    {
        // exit condition funzt nicht, da mehrmals möglich
        if (buffer[i] == 0xFF && buffer[i+1] == 0x2F) // midibefehl "Ende Midi-Datei"
        {
            printf("End of Track\n\n");
            //break;
        }

        if (buffer[i] == 0x80 || buffer[i] == 0x90) // note on || note off
        {
            //printf("%d\n",i);
            
            if (convertData == true)
            {
                for (int u = 0; u < (int)(sizeof(midiConvArray)/sizeof(midiConvArray[0])); u++)
                {
                    if (buffer[i+1] == midiConvArray[u].noteBefore)
                    {
                        buffer[i+1] = midiConvArray[u].noteAfter;
                        //printf("found note: %s \n",midiConvArray[u].instrument);
                        break;
                    }
                }
            }
            dataStart = i-1;
            dataEnd = dataStart+3;

            statusByte      = buffer[i];
            dataByte        = buffer[i+1];
            velocityByte    = buffer[i+2];
            
            
            if (buffer[i] == 0x80 || buffer[i] == 0x90)
            {
   
                // check first ticks value
                int checkFirstValue = (int)(unsigned char)buffer[i+3]; // gibt es zweiten Wert für Ticks?
                        
                switch(checkFirstValue)
                {
                    case 0x00:
                        ticksFirst = 0;
                        break;
                    case 0x40:
                        // /2 delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        ticksFirstTime = 128/2;
                        dataEnd += 1;
                        break;
                    case 0x80:
                        ticksFirst = 0;
                        break;
                    case 0x81:
                        // *delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128;
                        break;
                    case 0x82:
                        // *2*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*2;
                        break;
                    case 0x83:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*3;
                        break;
                    case 0x84:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*4;
                        break;
                    case 0x85:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*5;
                        break;
                    case 0x86:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*6;
                        break;
                    case 0x87:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*7;
                        break;
                    case 0x88:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*8;
                        break;
                    case 0x89:
                        // *3*delta ticks
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        dataEnd += 1;
                        ticksFirstTime = 128*9;
                        break;
                    case 0x90:
                        ticksFirst = 0;
                        break;
                    case 0xFF:
                        ticksFirst = 0;
                        break;
                    default:
                        ticksFirst = (int)(unsigned char)buffer[i+3];
                        break;
                }
                
                // check second ticks value
                int checkSecondValue = (int)(unsigned char)buffer[i+4]; // gibt es zweiten Wert für Ticks?
        
                switch(checkSecondValue)
                {
                    case 0x00:
                        ticksSecond = 0;
                        break;
                    case 0x80:
                        ticksSecond = 0;
                        break;
                    case 0x90:
                        ticksSecond = 0;
                        break;
                    case 0xFF:
                        ticksSecond = 0;
                        break;
                    default:
                        ticksSecond = (int)(unsigned char)buffer[i+4];
                        break;
                }
                
                ticksAbsolut += ticksSecond + ticksFirstTime;
                printf("TicksAbsolut: %d\n",ticksAbsolut);
                printf("bars: %d  %d\n",ticksAbsolut/noteWhole,buffer[i]);
                
                notesBarAbsolutNew = ticksAbsolut/noteWhole;
                
                if (notesBarAbsolutNew > notesBarAbsolutOld){
                    
                    printf("new bar!!");
                    notesBarAbsolutOld = notesBarAbsolutNew;
                }
                
            }
            
            if (saveMidiCommands == true)
            {
                pushNew(firstLink,head,dataStart,dataEnd,statusByte,dataByte,velocityByte,ticksFirst,ticksSecond);
            }
            

                
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
            ticksFirstTime  = 0;
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

