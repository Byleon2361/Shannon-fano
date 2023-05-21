#pragma once
#include <libShannonFano/compress.h>
int decompress(char *firstFile, char *secondFile, char *dataFile);
Value *readDataStruct(Value *valueArr, int *sizeValueArr, char *dataFile);
uint8_t *readCompressFile(Value *valueArr, int sizeValueArr, uint8_t *compressData, char *file);
char *decode(Value *valueArr, int sizeValueArr, char *text, uint8_t *compressData, int sizeCompress);