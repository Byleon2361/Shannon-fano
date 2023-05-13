#pragma once
#include <stdint.h>

typedef struct value {
    int count;
    char symbol;
    char codeString[100];
    uint8_t lengthCode;
    uint8_t code;
} Value;

// Commpress
int compress(char* firstFile, char* secondFile);
Value* writeDataStruct(
        Value* valueArr, int* sizeValueArr, char* firstFile, char* text);
int encode(Value* valueArr, int sizeValueArr, char* text, uint8_t* res);
void ShannonFano(Value* low, Value* high);
Value* midFunc(Value* low, Value* high);
Value* nextMidFunc(Value* low, Value* high, Value* mid);