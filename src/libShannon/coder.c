#include <stdbool.h>
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
int writeArrayInFile(Value* arrayValue, char* firstFile, char* secondFile)
{
    FILE* in;
    FILE* out;
    in = fopen(firstFile, "r");
    if (!in) {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return -1;
    }

    out = fopen(secondFile, "wb");

    if (!out) {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }
    int sizeArray = 0;
    int isfind = false;
    char* text = malloc(sizeof(char) * 100);
    while (fread(text, sizeof(char), 1, in) == 1) {
        Value newValue = {'\0', 0, 0};
        for (int i = 0; i < sizeArray; i++) {
            if (arrayValue[i].symbol == *text) {
                arrayValue[i].count++;
                isfind = true;
                break;
            }
        }
        if (!isfind) {
            newValue.symbol = *text;
            newValue.count++;
            arrayValue[sizeArray] = newValue;
            sizeArray++;
        }
        isfind = false;
    }
    for (int i = 0; i < sizeArray; i++) {
        printf("%c - %d\n", arrayValue[i].symbol, arrayValue[i].count);
    }
    printf("-------------------------------------------\n");
    // sort
    mergeSort(arrayValue, 0, sizeArray);
    for (int i = 0; i < sizeArray; i++) {
        printf("%c - %d\n", arrayValue[i].symbol, arrayValue[i].count);
    }
    encdode(arrayValue, sizeArray);
    fwrite(arrayValue, sizeof(Value), sizeArray, out);
    free(text);
    fclose(in);
    fclose(out);
    return 0;
}

int readArrayInFile(Value* arrayValue, char* firstFile, char* secondFile)
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
    int sizeArray = 0;
    while (fread(&arrayValue[sizeArray], sizeof(Value), 1, in) == 1)
        sizeArray++;

    for (int i = 0; i < sizeArray; i++) {
        printf("%c - %d\n", arrayValue[i].symbol, arrayValue[i].count);
    }
}

int decode(Value* arrayValue, int size)
{
    int number = 0;
    int lengthNumber = 0;
    for (int i = 0; i < size; i++) {
        arrayValue[i].code = number;
        if (lengthNumber % 2 == 0) {
            lengthNumber++;
        }
        number++;
    }
}

int encode(Value* arrayValue, int size)
{
    int number = 0;
    int lengthNumber = 0;
    for (int i = 0; i < size; i++) {
        arrayValue[i].code = number;
        if (lengthNumber % 2 == 0) {
            lengthNumber++;
        }
        number++;
    }
}
*/