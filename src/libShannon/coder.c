#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libShannon/coder.h>
void swap(Value* a, Value* b)
{
    Value tmp = *a;
    *a = *b;
    *b = tmp;
}
void oddEvenSort(Value* arr, int size)
{
    int sorted = 0;
    do {
        sorted = 0;
        for (int i = 1; i < size - 1; i += 2) {
            if (arr[i].count < arr[i + 1].count) {
                swap(&arr[i], &arr[i + 1]);
                sorted = 1;
            }
        }
        for (int i = 0; i < size - 1; i += 2) {
            if (arr[i].count < arr[i + 1].count) {
                swap(&arr[i], &arr[i + 1]);
                sorted = 1;
            }
        }

    } while (sorted);
}

int filesize(char* fileName)
{
    FILE* in = fopen(fileName, "r");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return -1;
    }

    fseek(in, 0L, SEEK_END);
    int size = ftell(in);
    rewind(in);

    fclose(in);
    return size;
}
uint8_t findValueInArray(Value* valueArr, int sizeValueArr, char sym)
{
    for (uint8_t i = 0; i < sizeValueArr; i++) {
        if (valueArr[i].symbol == sym) {
            return i;
        }
    }
    return -1;
}
int compress(char* firstFile, char* secondFile)
{
    FILE* data; // Хранит массив структур Value
    FILE* out;  // Результат

    data = fopen("data.dat", "wb");

    if (!data) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }
    out = fopen(secondFile, "wb");

    if (!out) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }

    Value* valueArr = malloc(sizeof(Value) * 100);
    int sizeValueArr = 0;
    char* text = malloc(sizeof(char) * filesize(firstFile) + 1);

    writeDataStruct(valueArr, &sizeValueArr, firstFile, text);
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    printf("----------------------------------------------\n");
    oddEvenSort(valueArr, sizeValueArr);
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }

    uint8_t* codingText = malloc(sizeof(uint8_t) * sizeValueArr);
    encode(valueArr, sizeValueArr, text, codingText);
    printf("----------------------------------------------\n");
    for (int i = 0; i < strlen(text); i++) {
        printf("%d\n", codingText[i]);
    }

    fwrite(valueArr, sizeof(Value), sizeValueArr, data);
    // write(codingText, sizeof(uint8_t), sizeValueArr, out);

    free(valueArr);
    free(text);
    free(codingText);

    fclose(data);
    fclose(out);

    return 0;
}
Value*
writeDataStruct(Value* valueArr, int* sizeValueArr, char* firstFile, char* text)
{
    int sizeText = 0;
    char buf[1] = {0};
    FILE* in = fopen(firstFile, "r");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return NULL;
    }

    int isfind = false;
    while (fread(buf, sizeof(char), 1, in) == 1) {
        Value newValue = {'\0', 0, 0};
        for (int i = 0; i < *sizeValueArr; i++) {
            if (valueArr[i].symbol == *buf) {
                valueArr[i].count++;
                isfind = true;
                break;
            }
        }
        if (!isfind) {
            newValue.symbol = *buf;
            newValue.count++;
            valueArr[*sizeValueArr] = newValue;
            (*sizeValueArr)++;
        }
        isfind = false;
        text[sizeText] = *buf;
        sizeText++;
    }
    fclose(in);
    return valueArr;
}
int encode(Value* valueArr, int sizeValueArr, char* text, uint8_t* res)
{
    for (int i = 0; i < strlen(text); i++) {
        res[i] = findValueInArray(valueArr, sizeValueArr, text[i]);
    }

        return 0;
}
int decompress(Value* valueArr, char* firstFile, char* secondFile)
{
    FILE* in;
    FILE* out;
    in = fopen(firstFile, "rb");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return -1;
    }

    out = fopen(secondFile, "w");

    if (!out) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }
    int sizeValueArr = 0;
    while (fread(&valueArr[sizeValueArr], sizeof(Value), 1, in) == 1)
        sizeValueArr++;

    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    fclose(in);
    fclose(out);
    return 0;
}
