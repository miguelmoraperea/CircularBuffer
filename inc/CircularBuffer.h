/*****************************************************************************
* Module name: CircularBuffer.h
*
* First written on 2018/12/27 by Miguel Mora Perea.
*
* Module Description:
* This is the interface of the CircularBuffer module.
*
*****************************************************************************/

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

typedef enum {
    BUFFER_SUCCESS,
    BUFFER_ERROR,
    BUFFER_EMPTY,
    BUFFER_FULL
} result_t;

result_t CircularBuffer_Init(int size);
void CircularBuffer_Destroy(void);
result_t CircularBuffer_ReadElem(char * data, int pos);
result_t CircularBuffer_WriteElem(char data, int pos);
result_t CircularBuffer_Clear(void);
int CircularBuffer_GetDataLength(void);
result_t CircularBuffer_Write(char data);
result_t CircularBuffer_Read(char * data);

#endif /* CIRCULAR_BUFFER_H */
