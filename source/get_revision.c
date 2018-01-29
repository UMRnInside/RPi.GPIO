#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "get_revision.h"

#define REVISION_FILENAME "/proc/device-tree/system/linux,version"
#define REVISION_SIZE 4
#define REVISION_OFFSET 1

char hexToAlpha[16] = {'0', '1', '2', '3','4', '5', '6', '7', \
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void generateReadableRevisionBit(char* buffer, char* binaryRevision);

char* get_cpu_revision(char* buffer)
{
    // Return NULL if failed
    char revBuffer[REVISION_SIZE];

    FILE* revFile = fopen(REVISION_FILENAME, "r");
    if (revFile == NULL)
        return NULL;
    
    fread( (void*)revBuffer, REVISION_SIZE, 1, revFile);
    fclose(revFile);

    // Need skip offset, as the first byte is unused.

    size_t limit = REVISION_SIZE - REVISION_OFFSET;
    for (size_t i=0;i<limit;i++)
    {
        generateReadableRevisionBit(buffer+i, revBuffer + REVISION_OFFSET + i);
    }

    return buffer;
}

void generateReadableRevisionBit(char* buffer, char* binaryRevision)
{
    // Read High & Low 4-Bit
    char high4Bit = (*binaryRevision) >> (char)4;
    char low4Bit = (*binaryRevision) & (char)0x0F;

    buffer[0] = hexToAlpha[high4Bit];
    buffer[1] = hexToAlpha[low4Bit];
}

