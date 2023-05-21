#pragma once
#include <stdint.h>

typedef struct value
{
    int count;
    int code;
    char codeString[31];
    char symbol;
} Value;

void oddEvenSort(Value *arr, int size);
int toByte(char *string);
int filesize(char *fileName);
// Commpress
void compress(char *firstFile, char *secondFile);
Value *createDataStruct(Value *valueArr, int *sizeValueArr, char *firstFile, char *text);
int encode(Value *valueArr, int sizeValueArr, char *text, uint8_t *res);
void ShannonFano(Value *low, Value *high);
Value *midFunc(Value *low, Value *high);
Value *nextMidFunc(Value *low, Value *high, Value *mid);