/*
Kierunek studiów i numer grupy ps: Informatyka, PS2
Skład grupy: Bartłomiej Umiński, Dominik Borowski, Michał Wysocki
Wybrany temat projektu: Temat 2 - Demon synchronizujący dwa podkatalogi
*/

#include "directory.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int makeDirectory(char* path)
{
    int result = mkdir(path, 0755);
    if(result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int removeDirectory(char* path)
{
    int result = rmdir(path);
    if(result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}