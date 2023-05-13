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
void charcat(char s[], char t);
void ShannonFano(Value* low, Value* high);
Value* nextMidFunc(Value* low, Value* high, Value* mid);
Value* midFunc(Value* low, Value* high);
// Decommpress
int decompress(char* firstFile, char* secondFile, char* dataFile);
Value* readDataStruct(Value* valueArr, int* sizeValueArr, char* firstFile);
uint8_t* readCompressFile(
        Value* valueArr, int sizeValueArr, uint8_t* compressData, char* file);
char* decode(
        Value* valueArr,
        int sizeValueArr,
        char* text,
        uint8_t* compressData,
        int sizeCompress);