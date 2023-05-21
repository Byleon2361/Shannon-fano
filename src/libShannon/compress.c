#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libShannon/compress.h>
static void swap(Value *a, Value *b)
{
    Value tmp = *a;
    *a = *b;
    *b = tmp;
}
static void oddEvenSort(Value *arr, int size)
{
    int sorted = 0;
    do
    {
        sorted = 0;
        for (int i = 1; i < size - 1; i += 2)
        {
            if (arr[i].count < arr[i + 1].count)
            {
                swap(&arr[i], &arr[i + 1]);
                sorted = 1;
            }
        }
        for (int i = 0; i < size - 1; i += 2)
        {
            if (arr[i].count < arr[i + 1].count)
            {
                swap(&arr[i], &arr[i + 1]);
                sorted = 1;
            }
        }

    } while (sorted);
}
int filesize(char *fileName)
{
    FILE *in = fopen(fileName, "r");
    if (!in)
    {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return -1;
    }

    fseek(in, 0L, SEEK_END);
    int size = ftell(in);
    rewind(in);

    fclose(in);
    return size;
}
static int toByte(char *string)
{
    int res = 0;
    int n = 0;
    for (int i = strlen(string) - 1; i > -1; i--)
    {
        res += (int)(string[i] - '0') * pow(2, n);
        n++;
    }
    return res;
}
static void charcat(char s[], char t)
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    s[i++] = t;
    s[i++] = '\0';
}
int compress(char *firstFile, char *secondFile)
{
    FILE *data; // Хранит массив структур Value
    FILE *out;  // Результат

    data = fopen("data.dat", "wb");
    if (!data)
    {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }

    out = fopen(secondFile, "wb");
    if (!out)
    {
        printf("\nОшибка. Не удалось открыть второй файл. ");
        return -1;
    }

    Value *valueArr = malloc(sizeof(Value));
    int sizeValueArr = 0;
    char *text = malloc(sizeof(char) * filesize(firstFile) + 1);

    valueArr = createDataStruct(valueArr, &sizeValueArr, firstFile, text);

    oddEvenSort(valueArr, sizeValueArr);
    uint8_t *codingText = calloc(strlen(text), sizeof(uint8_t));
    encode(valueArr, sizeValueArr, text, codingText);

    for (int i = 0; i < sizeValueArr; i++)
    {
        printf("%c - %d - %s\n", valueArr[i].symbol, valueArr[i].count, valueArr[i].codeString);
    }
    fwrite(valueArr, sizeof(Value), sizeValueArr, data);

    int size = 0;
    for (int i = 0; i < sizeValueArr; i++)
    {
        size += strlen(valueArr[i].codeString) * valueArr[i].count;
    }
    fwrite(codingText, sizeof(uint8_t), (size / 8) + 2, out);
    fclose(data);
    fclose(out);

    free(valueArr);
    free(text);
    free(codingText);
    return 0;
}
Value *createDataStruct(Value *valueArr, int *sizeValueArr, char *firstFile, char *text)
{
    int sizeText = 0;
    char buf[1] = {0};
    FILE *in = fopen(firstFile, "r");
    if (!in)
    {
        printf("\nОшибка. Не удалось открыть первый файл. ");
        return NULL;
    }
    int isfind = false;
    while (fread(buf, sizeof(char), 1, in) == 1)
    {
        Value newValue = {0, 0, 0, 0, 0, 0};
        for (int i = 0; i < *sizeValueArr; i++)
        {
            if (valueArr[i].symbol == *buf)
            {
                valueArr[i].count++;
                isfind = true;
                break;
            }
        }
        if (!isfind)
        {
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
static void CreateCode(Value *valueArr, int sizeValueArr)
{
    for (int i = 0; i < sizeValueArr; i++)
    {
        valueArr[i].lengthCode = strlen(valueArr[i].codeString); // Запись длины кода
        valueArr[i].code = toByte(
            valueArr[i].codeString); // перевод кода из текста в цифру
    }
}

int encode(Value *valueArr, int sizeValueArr, char *text, uint8_t *res)
{
    ShannonFano(&valueArr[sizeValueArr - 1], &valueArr[0]);
    CreateCode(valueArr, sizeValueArr);
    uint8_t shift = 0;
    uint8_t temp = 0;
    uint8_t tempValue = 0;
    int is = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        for (int j = 0; j < sizeValueArr; j++)
        {
            if (valueArr[j].symbol == text[i])
            {
                temp = valueArr[j].lengthCode;
                shift += temp;
                while (shift > 8)
                {
                    shift -= 8;
                    temp -= shift;
                    *res <<= temp;
                    *res |= valueArr[j].code >> shift;
                    res++;
                    tempValue = valueArr[j].code;
                    tempValue <<= 8 - shift;
                    tempValue >>= 8 - shift;
                    *res |= tempValue;
                    is = 0;
                    // break;
                }
                if (is)
                {
                    *res <<= temp;
                    *res |= valueArr[j].code;
                }

                is = 1;
                break;
            }
        }
    }

    if (shift != 0)
    {
        *res <<= 8 - shift;
    }

    return 0;
}
void ShannonFano(Value *low, Value *high)
{
    if (low - high == 0)
    {
        return;
    }
    if (low - high == 1)
    {
        charcat(high->codeString, '0');
        charcat(low->codeString, '1');
        return;
    }
    Value *mid = midFunc(low, high);
    Value *nextMid = nextMidFunc(low, high, mid);
    int i = 0;
    for (; high[i].symbol != mid->symbol; i++)
    {
        charcat(high[i].codeString, '0');
    }
    int j = 0;
    for (j = i; high[j].symbol != low->symbol; j++)
    {
        charcat(high[j].codeString, '1');
    }
    charcat(high[j].codeString, '1');
    ShannonFano(low, mid);
    ShannonFano(nextMid, high);
}
// Поиск середины
Value *midFunc(Value *low, Value *high)
{
    int all = 0;
    int half = 0;
    if (low - high == 2)
    {
        return &high[1];
    }
    for (int i = 0; i < low - high + 1; i++)
    {
        all += high[i].count;
    }
    for (int i = 0; i < low - high + 1; i++)
    {
        if (half >= all / 2)
            return &high[i];
        half += high[i].count;
    }

    return &high[0]; // Поменять
}
Value *nextMidFunc(Value *low, Value *high, Value *mid)
{
    for (int i = 0; i < low - high; i++)
    {
        if (high[i].symbol == mid->symbol)
            return &high[i - 1];
    }

    return &high[0];
}