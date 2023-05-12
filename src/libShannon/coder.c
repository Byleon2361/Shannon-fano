#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libShannon/coder.h>
void merge(Value* arr, int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[low + i].count;
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j].count;

    int l = 0;
    int r = 0;
    int i = low;
    while ((l < n1) && (r < n2)) {
        if (L[l] <= R[r]) {
            arr[i].count = L[l];
            l++;
        } else {
            arr[i].count = R[r];
            r++;
        }
        i++;
    }
    while (l < n1) {
        arr[i].count = L[l];
        l++;
        i++;
    }
    while (r < n2) {
        arr[i].count = R[r];
        r++;
        i++;
    }
}
void mergeSort(Value* arr, int low, int high)
{
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
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
    char* text = malloc(sizeof(char));
    int sizeText = 0;
    // uint8_t* codingText = malloc(sizeof(uint8_t) * sizeValueArr);

    writeDataStruct(valueArr, &sizeValueArr, firstFile, text, &sizeText);
    printf("!%s!\n", text);

    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    printf("-------------------------------------------\n");
    mergeSort(valueArr, 0, sizeValueArr);
    for (int i = 0; i < sizeValueArr; i++) {
        printf("%c - %d\n", valueArr[i].symbol, valueArr[i].count);
    }
    // encode(valueArr, sizeValueArr, text, codingText);

    fwrite(valueArr, sizeof(Value), sizeValueArr, data);
    // write(codingText, sizeof(uint8_t), sizeValueArr, out);

    free(valueArr);
    free(text);
    // free(codingText);

    fclose(data);
    fclose(out);

    return 0;
}
Value* writeDataStruct(
        Value* valueArr,
        int* sizeValueArr,
        char* firstFile,
        char* text,
        int* sizeText)
{
    char* ptr = malloc(sizeof(char));
    FILE* in = fopen(firstFile, "r");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return NULL;
    }

    int isfind = false;
    while (fread(ptr, sizeof(char), 1, in) == 1) {
        Value newValue = {'\0', 0, 0};
        for (int i = 0; i < *sizeValueArr; i++) {
            if (valueArr[i].symbol == *ptr) {
                valueArr[i].count++;
                isfind = true;
                break;
            }
        }
        if (!isfind) {
            newValue.symbol = *ptr;
            newValue.count++;
            valueArr[*sizeValueArr] = newValue;
            (*sizeValueArr)++;
        }
        isfind = false;
        // text = realloc();
        text[*sizeText] = *ptr;
        (*sizeText)++;
    }
    free(ptr);
    fclose(in);
    return valueArr;
}
int encode(Value* valueArr, int sizeValueArr, char* text, char* res)
{
    printf("%s", text);
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
