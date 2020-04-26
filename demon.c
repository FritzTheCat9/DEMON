#include "demon.h"
#include "file.h"
#include "log.h"
#include "directory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int changeSleepTime(char* arg)
{
    char string_time[strlen(arg)-2];
    for(int i=2; i<strlen(arg); i++)
    {
        string_time[i-2] = arg[i];
    }
    string_time[strlen(arg)-2] = '\0';
    int time = atoi(string_time);
    if(time <= 0 || time > 300)        // czas snu demona musi być z przedziału <1, 300> sekund
    {
        time = 300;
    }
    return time;
}

void sleepInSec(unsigned int sec) 
{
    usleep(sec * 1000000);
}

void demonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE)
{
    logUspienieDemona();

    sleepInSec(sleepTime);

    logObudzenieDemona();

    /* USUNIĘCIE PLIKÓW KTÓRYCH NIE MA W SOURCE A SĄ W DESTINATION */
    deleteFilesFromDestination(source, destination); 

    /* DODANIE LUB MODYFIKACJA PLIKÓW KTÓRE SA W SOURCE A NIE MA W DESTINATION LUB MAJA INNA DATE MODYFIKACJI */
    modifyOrCreateFilesFromSourceToDestination(source, destination, MAX_FILE_SIZE);  
}

void infiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE)
{
    while(1)
    {
        demonFunction(source, destination, sleep_time, MAX_FILE_SIZE); 
    }
}

void signalSIGUSR1Handler(char* source, char* destination, int recursive, int MAX_FILE_SIZE)
{
    logObudzenieDemonaSygnal();

    /* USUNIĘCIE PLIKÓW KTÓRYCH NIE MA W SOURCE A SĄ W DESTINATION */
    deleteFilesFromDestination(source, destination); 

    /* DODANIE LUB MODYFIKACJA PLIKÓW KTÓRE SA W SOURCE A NIE MA W DESTINATION LUB MAJA INNA DATE MODYFIKACJI */
    modifyOrCreateFilesFromSourceToDestination(source, destination, MAX_FILE_SIZE);

    /* USUNIĘCIE FOLDERÓW KTÓRYCH NIE MA W SOURCE A SĄ W DESTINATION */
    RemoveDirectory(source, destination); 

    if(recursive == 1)
    {
        /* DODANIE LUB MODYFIKACJA FOLDERÓW KTÓRE SA W SOURCE A NIE MA W DESTINATION LUB MAJA INNA DATE MODYFIKACJI */
        RecursiveCreateDirectory(source, destination, MAX_FILE_SIZE);   
    }

    logUspienieDemonaSygnal();
}

void deleteFilesFromDestination(char* source, char* destination)
{
    DIR* DestinationDirectory = opendir(destination);
    struct dirent* DestinationDirectory_File;

    while ((DestinationDirectory_File = readdir(DestinationDirectory)) != NULL) 
    {
        int destination_path_length = strlen(destination) + strlen(DestinationDirectory_File->d_name) + 2;
        char destination_path[destination_path_length];
        snprintf(destination_path, destination_path_length, "%s/%s", destination, DestinationDirectory_File->d_name);
        if(!isRegularFile(destination_path))
        {
            continue;
        }

        DIR* SourceDirectory = opendir(destination);
        struct dirent* SourceDirectory_File;

        int identicalName = 0;

        while ((SourceDirectory_File = readdir(SourceDirectory)) != NULL) 
        {
            int source_path_length = strlen(source) + strlen(SourceDirectory_File->d_name) + 2;
            char source_path[source_path_length];
            snprintf(source_path, source_path_length, "%s/%s", source, SourceDirectory_File->d_name);
            if(!isRegularFile(source_path))
            {
                continue;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)
            {
                identicalName = 1;
            }
        }

        if(identicalName == 0)
        {
            logUsunieciePliku(destination_path);
            removeFile(destination_path);
        }

        closedir(SourceDirectory);
    }
    closedir(DestinationDirectory);
}

void modifyOrCreateFilesFromSourceToDestination(char* source, char* destination, int MAX_FILE_SIZE)
{
    DIR* SourceDirectory = opendir(source);
    struct dirent* SourceDirectory_File;

    while ((SourceDirectory_File = readdir(SourceDirectory)) != NULL) 
    {
        int source_path_length = strlen(source) + strlen(SourceDirectory_File->d_name) + 2;
        char source_path[source_path_length];
        snprintf(source_path, source_path_length, "%s/%s", source, SourceDirectory_File->d_name);
        if(!isRegularFile(source_path))
        {
            continue;
        }

        DIR* DestinationDirectory = opendir(destination);
        struct dirent* DestinationDirectory_File;
        int identicalName = 0;

        while ((DestinationDirectory_File = readdir(DestinationDirectory)) != NULL) 
        {
            int destination_path_length = strlen(destination) + strlen(DestinationDirectory_File->d_name) + 2;
            char destination_path[destination_path_length];
            snprintf(destination_path, destination_path_length, "%s/%s", destination, DestinationDirectory_File->d_name);
            if(!isRegularFile(destination_path))
            {
                continue;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)
            {
                identicalName = 1;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)       // plik źródłowy ma inną datę modyfikacji niż plik w katalogu docelowym
            {
                if(!areHavingEqualModificationDate(source_path, destination_path))
                {
                    removeFile(destination_path);
                    if(getFileSize(source_path) >= MAX_FILE_SIZE)
                    {
                        mapFile(source_path, destination_path);
                        logModyfikacjaPlikuMMAP(destination_path);
                    }
                    else
                    {
                        copyFile(source_path, destination_path);
                        logModyfikacjaPliku(destination_path);
                    }
                    changeModificationDate(destination_path, source_path);
                }
            }
        }

        if(identicalName == 0)      // brak pliku źródłowego w katalogu docelowym
        {
            int destination_path_length = strlen(destination) + strlen(SourceDirectory_File->d_name) + 2;
            char destination_path[destination_path_length];
            snprintf(destination_path, destination_path_length, "%s/%s", destination, SourceDirectory_File->d_name);
            if(getFileSize(source_path) >= MAX_FILE_SIZE)
            {
                mapFile(source_path, destination_path);
                logKopiaPlikuMMAP(destination_path);
            }
            else
            {
                copyFile(source_path, destination_path);
                logKopiaPliku(destination_path);
            }
            changeModificationDate(destination_path, source_path);
        }

        closedir(DestinationDirectory);
    }
    closedir(SourceDirectory); 
}

void RemoveDirectory(char* source, char* destination)
{
    DIR* DestinationDirectory = opendir(destination);
    struct dirent* DestinationDirectory_File;

    while ((DestinationDirectory_File = readdir(DestinationDirectory)) != NULL) 
    {
        int destination_path_length = strlen(destination) + strlen(DestinationDirectory_File->d_name) + 2;
        char destination_path[destination_path_length];
        snprintf(destination_path, destination_path_length, "%s/%s", destination, DestinationDirectory_File->d_name);
        if(!isDirectory(destination_path))
        {
            continue;
        }

        DIR* SourceDirectory = opendir(destination);
        struct dirent* SourceDirectory_File;

        int identicalName = 0;

        while ((SourceDirectory_File = readdir(SourceDirectory)) != NULL) 
        {
            int source_path_length = strlen(source) + strlen(SourceDirectory_File->d_name) + 2;
            char source_path[source_path_length];
            snprintf(source_path, source_path_length, "%s/%s", source, SourceDirectory_File->d_name);
            if(!isDirectory(source_path))
            {
                continue;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)
            {
                identicalName = 1;
            }
        }

        if(identicalName == 0)
        {
            logUsuniecieKatalogu(destination_path);
            removeDirectory(destination_path);
        }

        closedir(SourceDirectory);
    }
    closedir(DestinationDirectory);
}

void RecursiveCreateDirectory(char* source, char* destination, int MAX_FILE_SIZE) 
{
    DIR* SourceDirectory = opendir(source);
    struct dirent* SourceDirectory_File;

    while ((SourceDirectory_File = readdir(SourceDirectory)) != NULL) 
    {
        int source_path_length = strlen(source) + strlen(SourceDirectory_File->d_name) + 2;
        char source_path[source_path_length];
        snprintf(source_path, source_path_length, "%s/%s", source, SourceDirectory_File->d_name);
        if(!isDirectory(source_path) || (strcmp(SourceDirectory_File->d_name, ".") == 0) || (strcmp(SourceDirectory_File->d_name, "..") == 0))
        {
            continue;
        }

        DIR* DestinationDirectory = opendir(destination);
        struct dirent* DestinationDirectory_File;
        int identicalName = 0;

        while ((DestinationDirectory_File = readdir(DestinationDirectory)) != NULL) 
        {
            int destination_path_length = strlen(destination) + strlen(DestinationDirectory_File->d_name) + 2;
            char destination_path[destination_path_length];
            snprintf(destination_path, destination_path_length, "%s/%s", destination, DestinationDirectory_File->d_name);
            if(!isDirectory(source_path) || (strcmp(DestinationDirectory_File->d_name, ".") == 0) || (strcmp(DestinationDirectory_File->d_name, "..") == 0))
            {
                continue;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)
            {
                identicalName = 1;
            }

            if(strcmp(SourceDirectory_File->d_name, DestinationDirectory_File->d_name) == 0)        // jest katalog źródłowy w katalogu docelowym
            {
                deleteFilesFromDestination(source_path, destination_path);
                modifyOrCreateFilesFromSourceToDestination(source_path, destination_path, MAX_FILE_SIZE);
                RemoveDirectory(source_path, destination_path);
                RecursiveCreateDirectory(source_path, destination_path, MAX_FILE_SIZE);             // uwaga REKURENCJA !!!!
            }
        }

        if(identicalName == 0)      // brak katalogu źródłowego w katalogu docelowym
        {
            int destination_path_length = strlen(destination) + strlen(SourceDirectory_File->d_name) + 2;
            char destination_path[destination_path_length];
            snprintf(destination_path, destination_path_length, "%s/%s", destination, SourceDirectory_File->d_name);

            makeDirectory(destination_path);
            logKopiaKatalogu(destination_path);

            deleteFilesFromDestination(source_path, destination_path);
            modifyOrCreateFilesFromSourceToDestination(source_path, destination_path, MAX_FILE_SIZE);

            RemoveDirectory(source_path, destination_path);
            RecursiveCreateDirectory(source_path, destination_path, MAX_FILE_SIZE);             // uwaga REKURENCJA !!!!
        }

        closedir(DestinationDirectory);
    }
    closedir(SourceDirectory); 
}

void RecursiveDemonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE)
{
    logUspienieDemona();

    sleepInSec(sleepTime);

    logObudzenieDemona();

    /* USUNIĘCIE PLIKÓW KTÓRYCH NIE MA W SOURCE A SĄ W DESTINATION */
    deleteFilesFromDestination(source, destination); 

    /* DODANIE LUB MODYFIKACJA PLIKÓW KTÓRE SA W SOURCE A NIE MA W DESTINATION LUB MAJA INNA DATE MODYFIKACJI */
    modifyOrCreateFilesFromSourceToDestination(source, destination, MAX_FILE_SIZE);

    /* USUNIĘCIE FOLDERÓW KTÓRYCH NIE MA W SOURCE A SĄ W DESTINATION */
    RemoveDirectory(source, destination);

    /* DODANIE LUB MODYFIKACJA FOLDERÓW KTÓRE SA W SOURCE A NIE MA W DESTINATION LUB MAJA INNA DATE MODYFIKACJI */
    RecursiveCreateDirectory(source, destination, MAX_FILE_SIZE); 
}

void RecursiveInfiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE)
{
    while(1)
    {
        RecursiveDemonFunction(source, destination, sleep_time, MAX_FILE_SIZE);
    }
}