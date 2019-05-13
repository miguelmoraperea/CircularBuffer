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
	const int cbSize = 8;

    void setup ()
    {
    	CircularBuffer_Init(cbSize);
    }

    void teardown()
    {
    	void CircularBuffer_Destroy(void);
    }
};

TEST(CircularBuffer, SizeMustBeAPowerOfTwo)
{
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_Init(3));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_Init(10));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(8));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(32));
}

TEST(CircularBuffer, InitializeWithPositiveSize)
{
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Init(8))
}

TEST(CircularBuffer, DoesNotInizializeWithNegativeSize)
{
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_Init(0));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_Init(-1));
}

TEST(CircularBuffer, InitializeBufferWithNull)
{
	for (int i = 0; i < cbSize; i++)
	{
		char value;
		LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&value, i));
		LONGS_EQUAL('\0', value);
	}
}

TEST(CircularBuffer, WriteAndReadToSinglePosition)
{
	char value = 'A';
	char readElement;
	int index = 0;

	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, index));
	CircularBuffer_ReadElem(&readElement, index);
	CHECK_EQUAL(value, readElement);

	index = cbSize - 1;
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, index));
	CircularBuffer_ReadElem(&readElement, index);
	CHECK_EQUAL(value, readElement);
}

TEST(CircularBuffer, ClearBuffer)
{
	char value;

	for (int i = 0; i < cbSize; i++)
	{
		value = 'A';
		CircularBuffer_WriteElem(value, i);
	}

	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Clear());

	char readValue;
	for (int i = 0; i < cbSize; i++)
	{
		CircularBuffer_ReadElem(&readValue, i);
		LONGS_EQUAL('\0', readValue);
	}
}

TEST(CircularBuffer, FirstInFirstOut)
{
	char values[] = {'A', 'B', 'C', 'D', 'E', 'F'};

	CircularBuffer_Clear();

	int i;

	for (i = 0; i < 6; i++)
	{
		LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[i]));
	}

	for (i = 0; i < 6; i++)
	{
		char readValue;
		LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, i));
		LONGS_EQUAL(values[i], readValue);
	}
}

TEST(CircularBuffer, ReadSingleElementToValidPosition)
{
	char readValue;
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 0));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 3));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_ReadElem(&readValue, 7));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, -1));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, 8));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_ReadElem(&readValue, 10));
}

TEST(CircularBuffer, WriteSingleElementToValidPosition)
{
	char value = 'A';
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 0));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 3));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_WriteElem(value, 7));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, -1));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, 8));
	LONGS_EQUAL(BUFFER_ERROR, CircularBuffer_WriteElem(value, 10));
}

TEST(CircularBuffer, BufferIsFull)
{
	char values[] = {'A', 'B', 'C', 'D'};

	CircularBuffer_Init(4);

	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[0]));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[1]));
	LONGS_EQUAL(BUFFER_SUCCESS, CircularBuffer_Write(values[2]));
	LONGS_EQUAL(BUFFER_FULL, CircularBuffer_Write(values[3]));
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
		LONGS_EQUAL(expectedValues[i], readValue);
	}
}

