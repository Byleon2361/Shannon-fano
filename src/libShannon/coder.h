#pragma once
typedef struct value {
    char symbol;
    int count;
    int code;
    int lengthCode;
} Value;
#define MAX_LENGTH_TEXT 1000
void merge(Value* arr, int low, int mid, int high);
void mergeSort(Value* arr, int low, int high);
int writeArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
int readArrayInFile(Value* arrayValue, char* firstFile, char* secondFile);
