/*
Kierunek studiów i numer grupy ps: Informatyka, PS2
Skład grupy: Bartłomiej Umiński, Dominik Borowski, Michał Wysocki
Wybrany temat projektu: Temat 2 - Demon synchronizujący dwa podkatalogi
*/

#include "file.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include <sys/mman.h>

int isDirectory(char* path)
{
    struct stat stat_path;
    if(stat(path, &stat_path) != 0)         // czy plik istnieje
    {
        return 0;
    }
    if(S_ISDIR(stat_path.st_mode) == 1)     // czy plik jest katalogiem
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isRegularFile(char* path)
{
    struct stat stat_path;
    if(stat(path, &stat_path) != 0)         // czy plik istnieje
    {
        return 0;
    }
    if(S_ISREG(stat_path.st_mode) == 1)     // czy plik jest zwykłym plikiem
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int copyFile(char* source, char* destination)
{
    int fPointer1;
	fPointer1 = open(source, O_RDONLY);
	if(fPointer1 == -1)
	{
		return 0;
	}

    int fPointer2;
	fPointer2 = open(destination, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
	if(fPointer2 == -1)
	{
	    return 0;
	}

    int size = 1024;
    char* tab = malloc(size * sizeof(char));
	
    while(1)
	{
		int ileWczytanych = read(fPointer1, tab, size);
		if(ileWczytanych == 0)
		{
			break;
		}
		write(fPointer2, tab, ileWczytanych);
	}

    free(tab);
    close(fPointer1);
	close(fPointer2);
    return 1;
}

int areHavingEqualModificationDate(char* source, char* destination)
{
    struct stat stat_source;
    struct stat stat_destination;
    
    if(stat(source, &stat_source) != 0)                     // czy plik istnieje
    {
        return 0;
    }
    if(stat(destination, &stat_destination) != 0)           // czy plik istnieje
    {
        return 0;
    }

    if(stat_source.st_mtime == stat_destination.st_mtime)   // czy pliki maja taką samą datę modyfikacji
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int changeModificationDate(char* pathToChangeDate, char* pathToGetDate)
{
    struct stat stat_pathToGetDate;
    struct utimbuf newTime;

    if(stat(pathToGetDate, &stat_pathToGetDate) != 0)           // czy plik istnieje
    {
        return 0;
    }

    newTime.actime = stat_pathToGetDate.st_atime;
    newTime.modtime = stat_pathToGetDate.st_mtime;
    utime(pathToChangeDate, &newTime);                          // zmiana daty modyfikacji pliku

    struct stat stat_pathToChangeDate;
    return 1;
}

int isThisFileInADirectory(char* fileName, char* directoryName)
{
    DIR* directory = opendir(directoryName);
    struct dirent* file;
    int result = 0;

    while ((file = readdir(directory)) != NULL) 
    {
        if(strcmp(fileName, file->d_name) == 0)
        {
            result = 1;
        }
    }

    if(result == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int removeFile(char* path)
{
    int result = remove(path);
    if(result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int getFileSize(char* path)
{
    struct stat st;
    if(stat(path, &st) != 0)  
    {
        return 0;
    }
    int size = st.st_size;
    return size;
}

int mapFile(char* source_mapp, char* destination_write)
{
    int fPointer1;
	fPointer1 = open(source_mapp, O_RDONLY);
	if(fPointer1 == -1)
	{
		return 0;
	}

    int fPointer2;
	fPointer2 = open(destination_write, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
	if(fPointer2 == -1)
	{
	    return 0;
	}

    int size = getFileSize(source_mapp);
	
    char* file_in_memory = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fPointer1, 0);

	write(fPointer2, file_in_memory, size);

    munmap(file_in_memory, size);

    close(fPointer1);
	close(fPointer2);
    return 1;
}

int changeMAX_FILE_SIZE(char* arg)
{
    char string_size[strlen(arg)-2];
    for(int i=2; i<strlen(arg); i++)
    {
        string_size[i-2] = arg[i];
    }
    string_size[strlen(arg)-2] = '\0';
    int size = atoi(string_size);
    if(size <= 0)                       // rozmiar pliku musi być z przedziału wiekszy od 0 bajtów
    {
        size = 1000000;
    }
    return size;
}