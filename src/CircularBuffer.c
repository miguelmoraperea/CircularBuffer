/*****************************************************************************
* Module name: CircularBuffer.c
*
* First written on 2018/12/27 by Miguel Mora Perea.
*
* Module Description:
* This module contains functions that implement a circular buffer.
*
*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "CircularBuffer.h"

enum {
    FALSE = 0,
    TRUE = 1,
};

typedef struct {
    char *buff;
    int  size;       // must be a power of two
    int  read;
    int  write;
} circular_buffer_t;

static circular_buffer_t cb;

static void ResetBuffer(void)
{
    cb.read = 0;
    cb.write = 0;
    memset(cb.buff, '\0', (size_t)cb.size);
}

static int IsPowerOfTwo(int value)
{
    if (value < 1)
    {
        return FALSE;
    }

    if (value & (value - 1))
    {
        return FALSE;
    }

    return TRUE;
}

result_t CircularBuffer_Init(int size)
{
    if (!IsPowerOfTwo(size))
    {
        return BUFFER_ERROR;
    }

    cb.buff = (char *)malloc((unsigned)size * sizeof(char));
    cb.size = size;

    ResetBuffer();

    return BUFFER_SUCCESS;
}

void CircularBuffer_Destroy(void)
{
    free(cb.buff);
}

static int IsPositionValid(int pos)
{
    if (pos < 0 || pos >= cb.size)
    {
        return FALSE;
    }

    return TRUE;
}

result_t CircularBuffer_ReadElem(char * data, int pos)
{
    if (!IsPositionValid(pos))
    {
        return BUFFER_ERROR;
    }

    *data = cb.buff[pos];
    return BUFFER_SUCCESS;
}

result_t CircularBuffer_WriteElem(char data, int pos)
{
    if (!IsPositionValid(pos))
    {
        return BUFFER_ERROR;
    }

    cb.buff[pos] = data;
    return BUFFER_SUCCESS;
}

result_t CircularBuffer_Clear()
{
    ResetBuffer();
    return BUFFER_SUCCESS;
}

int CircularBuffer_GetDataLength(void)
{
     return ((cb.write - cb.read) & (cb.size - 1));
}

result_t CircularBuffer_Write(char data)
{
    if(CircularBuffer_GetDataLength() == (cb.size - 1))
    {
        return BUFFER_FULL;
    }

    cb.buff[cb.write] = data;
    cb.write = (cb.write + 1) & (cb.size - 1);  // must be atomic

    return BUFFER_SUCCESS;
}

result_t CircularBuffer_Read(char *data)
{
    if(CircularBuffer_GetDataLength() == 0)
    {
        return BUFFER_EMPTY;
    }

    *data = cb.buff[cb.read];
    cb.read = (cb.read + 1) & (cb.size - 1);

    return BUFFER_SUCCESS;
}
