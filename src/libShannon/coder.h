#pragma once
#include <stdint.h>
typedef struct value {
    char symbol;
    int count;
    int code;
    int lengthCode;
} Value;
#define MAX_LENGTH_TEXT 1000
// Сортировка
void swap(Value* a, Value* b);
void oddEvenSort(Value* arr, int size);
int filesize(char* fileName);
uint8_t findValueInArray(Value* valueArr, int sizeValueArr,char sym);

// Commpress
int compress(char* firstFile, char* secondFile);
Value* writeDataStruct(
        Value* valueArr, int* sizeValueArr, char* firstFile, char* text);

int writeArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
int readArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
int encode(Value* valueArr, int sizeValueArr, char* text, uint8_t* res);
