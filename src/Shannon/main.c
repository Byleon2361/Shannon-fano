#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libShannon/compress.h>
#include <libShannon/decompress.h>
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        puts("The number of arguments passed must be three");
        return 0;
    }
    const char *command = argv[1]; // Команда  -c - copmress; -d - decompress
    const char *first = argv[2];   // -o - Указывает на имя выходного файла
    const char *second = argv[3];
    const char *third = argv[4];

    if (strcmp(command, "-c") == 0)
    {
        if (strcmp(first, "-o") == 0)
            compress(third, second);
        else if (strcmp(second, "-o") == 0)
            compress(first, third);
        else
            printf("Введен неверный ключ\n");
    }
    else if (strcmp(command, "-d") == 0)
    {
        if (strcmp(first, "-o") == 0)
            decompress(third, second, "data.dat");
        else if (strcmp(second, "-o") == 0)
            decompress(first, third, "data.dat");
        else
            printf("Введен неверный ключ\n");
    }
    else
        printf("Введен неверный ключ\n");

    // compress("file.txt", "file.sfc");
    // decompress("file.sfc", "res.txt", "data.dat");
    return 0;
}