#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        puts("The number of arguments passed must be three");
        return 0;
    }
    const char *command = argv[1];//Команда  -c - copmress; -d - decompress
    const char *output = argv[2]; // -o - Указывает на имя выходного файла
    const char *firstFile = argv[3];
    const char *secondFile = argv[4];

    if (strcmp(command, "-c") == 0)
    {

        if (compress_file(firstFile, secondFile) != 0)
        {
            printf("Не удалось откыть файлы\n");
        }
    }
    else if (strcmp(command, "-d") == 0)
    {
        if (decompress_file(firstFile, SecondFile) != 0)
        {
            printf("Не удалось откыть файлы\n");
        }
    }
    else
    {
	pritnf("Введен неверный ключ\n");
    }
    return 0;
}