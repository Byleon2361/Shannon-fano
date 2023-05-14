#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libShannon/decompress.h>
#include <libShannon/compress.h>
int decompress(char *firstFile, char *secondFile, char *dataFile)
{
    FILE *out;

    out = fopen(secondFile, "w");
    if (!out)
    {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }

    Value *valueArr = malloc(sizeof(Value) * 256);
    int sizeValueArr = 0;

    readDataStruct(valueArr, &sizeValueArr, dataFile);

    for (int i = 0; i < sizeValueArr; i++)
    {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    int sizeCompress = filesize(firstFile);
    uint8_t *compressData = malloc(sizeof(char) * sizeCompress + 1);
    printf("-------------------------------------------------------\n");
    readCompressFile(valueArr, sizeValueArr, compressData, firstFile);
    for (int i = 0; i < filesize(firstFile); i++)
    {
        printf("%d\n", compressData[i]);
    }

    char *text = malloc(sizeof(char) * 10);
    text = decode(valueArr, sizeValueArr, text, compressData, sizeCompress);

    fprintf(out, "%s", text);
    /*
        for (int i = 0; i < strlen(text); i++)
        {
            fwrite(text, sizeof(char), 1, out);
        }
    */
    fclose(out);

    free(compressData);
    free(valueArr);
    free(text);
    return 0;
}
Value *readDataStruct(Value *valueArr, int *sizeValueArr, char *dataFile)
{
    FILE *data = fopen(dataFile, "rb");
    if (!data)
    {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return NULL;
    }
    while (fread(&valueArr[*sizeValueArr], sizeof(Value), 1, data) == 1)
        (*sizeValueArr)++;

    fclose(data);
    return valueArr;
}
uint8_t *readCompressFile(Value *valueArr, int sizeValueArr, uint8_t *compressData, char *file)
{
    uint8_t buf[1] = {0};
    FILE *in = fopen(file, "rb");
    if (!in)
    {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return NULL;
    }
    for (int i = 0; fread(buf, sizeof(uint8_t), 1, in) == 1; i++)
    {
        compressData[i] = *buf;
    }

    return compressData;
}
char *decode(Value *valueArr, int sizeValueArr, char *text, uint8_t *compressData, int sizeCompress)
{
    int krat = 2;
    int shift = 0;
    int length = 0;
    int a = 1;
    int k = 0;
    int q = 0;
    uint8_t newCode = 0;
    uint8_t temp = 0;
    for (int i = 0; i < valueArr[0].sizeString; i++)
    {
        if (i % 10 == 0)
        {
            text = realloc(text, sizeof(char) * 10 * krat);
            krat++;
        }
        newCode <<= 1;
        if (i % 8 == 0 && i != 0)
        {
            q++;
            shift = 0;
        }
        temp = compressData[q] << shift;
        newCode |= (temp >> 7);
        for (int j = 0; j < sizeValueArr; j++)
        {
            if (newCode == valueArr[j].code)
            {
                text[k] = valueArr[j].symbol;
                newCode = 0;
                k++;
                break;
            }
        }
        shift++;
    }
    text[k] = '\0';

    return text;
}