/*****************************************************************************
* Module name: CircularBuffer_Tests.cpp
*
* First written on 2019/05/08 by Miguel Mora Perea.
*
* Module Description:
* CircularBuffer TDD Tests.
*
*****************************************************************************/

#include "CppUTest/TestHarness.h"

extern "C"
{
#include "CircularBuffer.h"
}

TEST_GROUP(CircularBuffer)
{
};

TEST(CircularBuffer, SizeMustBeAPowerOfTwo)
{
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_Init(3));

    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_Init(10));

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(8));
    CircularBuffer_Destroy();

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(32));
    CircularBuffer_Destroy();
}

TEST(CircularBuffer, InitializeWithPositiveSize)
{
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(8));
    CircularBuffer_Destroy();
}

TEST(CircularBuffer, DoesNotInitializeWithNegativeSize)
{
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_Init(0));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_Init(-1));
}

TEST(CircularBuffer, InitializeBufferWithNull)
{
    int cbSize = 8;
    CircularBuffer_Init(cbSize);

    for (int i = 0; i < cbSize; i++)
    {
        char value;
        CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&value, i));
        CHECK_EQUAL('\0', value);
    }

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, WriteAndReadToSinglePosition)
{
    char value = 'A';
    char readElement;
    int index = 0;
    int cbSize = 8;

    CircularBuffer_Init(cbSize);

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, index));
    CircularBuffer_ReadElem(&readElement, index);
    CHECK_EQUAL(value, readElement);

    index = cbSize - 1;
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, index));
    CircularBuffer_ReadElem(&readElement, index);
    CHECK_EQUAL(value, readElement);

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, ClearBuffer)
{
    char value;
    int cbSize = 8;

    CircularBuffer_Init(cbSize);

    for (int i = 0; i < cbSize; i++)
    {
        value = 'A';
        CircularBuffer_WriteElem(value, i);
    }

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Clear());

    char readValue;
    for (int i = 0; i < cbSize; i++)
    {
        CircularBuffer_ReadElem(&readValue, i);
        CHECK_EQUAL('\0', readValue);
    }

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, FirstInFirstOut)
{
    char values[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    CircularBuffer_Init(8);
    CircularBuffer_Clear();

    int i;

    for (i = 0; i < 6; i++)
    {
        CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[i]));
    }

    for (i = 0; i < 6; i++)
    {
        char readValue;
        CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, i));
        CHECK_EQUAL(values[i], readValue);
    }

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, ReadSingleElementToValidPosition)
{
    char readValue;

    CircularBuffer_Init(8);

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 0));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 3));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 7));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, -1));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, 8));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, 10));

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, WriteSingleElementToValidPosition)
{
    char value = 'A';

    CircularBuffer_Init(8);

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 0));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 3));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 7));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, -1));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, 8));
    CHECK_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, 10));

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, BufferIsFull)
{
    char values[] = {'A', 'B', 'C', 'D'};

    CircularBuffer_Init(4);

    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[0]));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[1]));
    CHECK_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[2]));
    CHECK_EQUAL(BUFFER_FULL, CircularBuffer_Write(values[3]));

    CircularBuffer_Destroy();
}

TEST(CircularBuffer, WrapAround)
{
    char values[] = {'A', 'B', 'C', 'D'};

    CircularBuffer_Init(4);

    CircularBuffer_Write(values[0]);
    CircularBuffer_Write(values[1]);
    CircularBuffer_Write(values[2]);

    char readValue;
    CircularBuffer_Read(&readValue);
    CircularBuffer_Read(&readValue);
    CircularBuffer_Read(&readValue);

    CircularBuffer_Write(values[0]);
    CircularBuffer_Write(values[1]);
    CircularBuffer_Write(values[2]);

    char expectedValues[4] = {'B', 'C', 'C', 'A'};

    for (int i = 0; i < 4; i++)
    {
        CircularBuffer_ReadElem(&readValue, i);
        CHECK_EQUAL(expectedValues[i], readValue);
    }

    CircularBuffer_Destroy();
}
