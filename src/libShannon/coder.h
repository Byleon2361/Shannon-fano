#pragma once
typedef struct value {
    char symbol;
    int count;
    int code;
    int lengthCode;
} Value;
#define MAX_LENGTH_TEXT 1000
// Сортировка
void merge(Value* arr, int low, int mid, int high);
void mergeSort(Value* arr, int low, int high);

// Commpress
int compress(char* firstFile, char* secondFile);
Value* writeDataStruct(
        Value* valueArr, int* sizeValueArr, char* firstFile, char* text, int* sizeText);

int writeArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
int readArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
int encode(Value* valueArr, int sizeValueArr, char* text, char* res);
