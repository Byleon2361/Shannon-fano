#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libShannon/coder.h>
static void swap(Value* a, Value* b)
{
    Value tmp = *a;
    *a = *b;
    *b = tmp;
}
static void oddEvenSort(Value* arr, int size)
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

static int filesize(char* fileName)
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

    Value* valueArr = malloc(sizeof(Value));
    int sizeValueArr = 0;
    char* text = malloc(sizeof(char) * filesize(firstFile) + 1);

    valueArr = writeDataStruct(valueArr, &sizeValueArr, firstFile, text);
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    printf("----------------------------------------------\n");
    oddEvenSort(valueArr, sizeValueArr);
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }

    uint8_t* codingText = malloc(sizeof(uint8_t) * strlen(text));
    encode(valueArr, sizeValueArr, text, codingText);
    printf("----------------------------------------------\n");
    for (int i = 0; i < strlen(text); i++) {
        printf("%d\n", codingText[i]);
    }

    fwrite(valueArr, sizeof(Value), sizeValueArr, data);
    fwrite(codingText, sizeof(uint8_t), strlen(text), out);

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
        Value newValue = {0, 0, 0, 0, 0};
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
            valueArr = realloc(
                    valueArr,
                    (sizeof(Value) * (*sizeValueArr)) + sizeof(Value));
        }
        isfind = false;
        text[sizeText] = *buf;
        sizeText++;
    }
    fclose(in);
    return valueArr;
}
static uint8_t toByte(char* string)
{
    uint8_t res = 0;
    uint8_t n = 0;
    for (int i = strlen(string) - 1; i > -1; i--) {
        res += (uint8_t)(string[i] - '0') * pow(2, n);
        n++;
    }
    return res;
}
static void CreateCode(Value* valueArr, int sizeValueArr)
{
    for (int i = 0; i < sizeValueArr; i++) {
        valueArr[i].lengthCode
                = strlen(valueArr[i].codeString); // Запись длины кода
        valueArr[i].code = toByte(
                valueArr[i].codeString); // перевод кода из текста в цифру
    }
}
int encode(Value* valueArr, int sizeValueArr, char* text, uint8_t* res)
{
    ShannonFano(&valueArr[sizeValueArr - 1], &valueArr[0]);
    CreateCode(valueArr, sizeValueArr);
    printf("----------------------------\n");
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d - %s - %d - %d\n",
               valueArr[i].symbol,
               valueArr[i].count,
               valueArr[i].codeString,
               valueArr[i].lengthCode,
               valueArr[i].code);
    }
    int k = 0;
    for (int i = 0; i < strlen(text); i++) {
        for (int j = 0; j < sizeValueArr; j++) {
            if (valueArr[j].symbol == text[i]) {
                res[k] = valueArr[j].code;
                k++;
                break;
            }
        }
    }
    return 0;
}
void charcat(char s[], char t)
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    s[i++] = t;
    s[i++] = '\0';
}
void ShannonFano(Value* low, Value* high)
{
    if (low - high == 0) {
        return;
    }
    Value* mid = midFunc(low, high);
    Value* nextMid = nextMidFunc(low, high, mid);
    int i = 0;
    for (; high[i].symbol != mid->symbol; i++) {
        charcat(high[i].codeString, '0');
    }
    int j = 0;
    for (j = i; high[j].symbol != low->symbol; j++) {
        charcat(high[j].codeString, '1');
    }
    charcat(high[j].codeString, '1');
    ShannonFano(low, mid);
    ShannonFano(nextMid, high);
}
// Поиск середины
Value* midFunc(Value* low, Value* high)
{
    int all = 0;
    int half = 0;
    for (int i = 0; i < low - high + 1; i++) {
        all += high[i].count;
    }
    for (int i = 0; i < low - high + 1; i++) {
        if (half >= all / 2)
            return &high[i];
        half += high[i].count;
    }

    return &high[0]; // Поменять
}
Value* nextMidFunc(Value* low, Value* high, Value* mid)
{
    for (int i = 0; i < low - high; i++) {
        if (high[i].symbol == mid->symbol)
            return &high[i - 1];
    }

    return &high[0];
}
int decompress(char* firstFile, char* secondFile, char* dataFile)
{
    FILE* out;

    out = fopen(secondFile, "w");
    if (!out) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }

    Value* valueArr = malloc(sizeof(Value) * 256);
    int sizeValueArr = 0;

    readDataStruct(valueArr, &sizeValueArr, dataFile);

    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    int sizeCompress = filesize(firstFile);
    uint8_t* compressData = malloc(sizeof(char) * sizeCompress + 1);
    printf("-------------------------------------------------------\n");
    readCompressFile(valueArr, sizeValueArr, compressData, firstFile);
    for (int i = 0; i < filesize(firstFile); i++) {
        printf("%d\n", compressData[i]);
    }

    char* text = malloc(sizeof(char) * 256);
    // decode(valueArr, sizeValueArr, text, compressData, sizeCompress);
    fclose(out);
    return 0;
}
Value* readDataStruct(Value* valueArr, int* sizeValueArr, char* dataFile)
{
    FILE* data = fopen(dataFile, "rb");
    if (!data) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return NULL;
    }
    while (fread(&valueArr[*sizeValueArr], sizeof(Value), 1, data) == 1)
        (*sizeValueArr)++;

    fclose(data);
    return valueArr;
}

// Возможно, это не работает
uint8_t* readCompressFile(
        Value* valueArr, int sizeValueArr, uint8_t* compressData, char* file)
{
    uint8_t buf[1] = {0};
    FILE* in = fopen(file, "rb");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return NULL;
    }
    for (size_t i = 0; fread(buf, sizeof(uint8_t), 1, in) == 1; i++) {
        compressData[i] = *buf;
    }

    return compressData;
}
/*
char* decode(Value* valueArr,int sizeValueArr,char* text,uint8_t*
compressData,int sizeCompress)
{
    int krat = 1;
    for (int i = 0; i < sizeCompress; i++) {
        text[i] =
    }

    return text;
}
*/