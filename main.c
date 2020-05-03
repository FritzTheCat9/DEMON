/*
Kierunek studiów i numer grupy ps: Informatyka, PS2
Skład grupy: Bartłomiej Umiński, Dominik Borowski, Michał Wysocki
Wybrany temat projektu: Temat 2 - Demon synchronizujący dwa podkatalogi
*/

#include "file.h"
#include "help.h"
#include "demon.h"
#include "log.h"
#include "directory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char* src;                              // ścieżka źródła - przesyłana do funkcji obsługującej sygnał SIGUSR1
char* dest;                             // ścieżka celu - przesyłana do funkcji obsługującej sygnał SIGUSR1
int recursive = 0;                      // czy wykonujemy synchronizację włącznie z katalogami (rekurencyjnie)
int MAX_FILE_SIZE = 1000000;            // maksymalny rozmiar pliku który kopujemy funkcjami read/write (potem tylko mmap/write)

void handlerfunction()                  // funkcja obsługująca sygnał SIGUSR1
{
    signalSIGUSR1Handler(src, dest, recursive, MAX_FILE_SIZE);
}

int main(int argc, char* argv[])        // funkcja główna programu
{
    if(argc < 3)
    {
        usage();                        // wypisanie instrukcji gdy mamy mniej niż 2 parametry
    }
    else
    {
        char source[strlen(argv[1])];           // ścieżka źródła - odczytanie z podanych argumentów
        strcpy(source, argv[1]);
        char destination[strlen(argv[2])];      // ścieżka celud - odczytanie z podanych argumentów
        strcpy(destination, argv[2]);

        src = malloc( sizeof(char) * ( strlen(source) + 1 ) );          // ścieżka źródła - przesyłana do funkcji obsługującej sygnał SIGUSR1
        dest = malloc( sizeof(char) * ( strlen(destination) + 1 ) );    // ścieżka celu - przesyłana do funkcji obsługującej sygnał SIGUSR1
        snprintf(src, strlen(source) + 1, "%s", source);
        snprintf(dest, strlen(destination) + 1, "%s", destination);

        if(isDirectory(source) == 0 || isDirectory(destination) == 0)   // srgumenty 1 i 2 muszą być folderami
        {
            printf("BŁĄD - podano niepoprawne sciezki (obie powinny byc katalogami)\n");
            return 0;
        }

        int sleep_time = 300;                   // czas snu demona (domyślnie 5 minut)
        int signalSIGUSR1 = 0;                  // czy uruchomiono program z parametrem -s (sygnał SIGUSR1)

        for(int i=3; i<argc; i++)               // wczytanie wszystkich parametrów programu (pozostałe argumenty podane przy wywołaniu programu)
        {
            char option = argv[i][1];
            switch (option)
            {
            case 't':                                                   // argument t - zmiana czasu snu demona
                sleep_time = changeSleepTime(argv[i]);
                printf("Sleep time set to: %d sec.\n", sleep_time);
                break;
            case 's':                                                   // argument s - natychmiastowa synchronizacja folderów przy uruchomieniu programu
                signalSIGUSR1 = 1;
                printf("Signal SIGUSR1 turned on\n");
                break;
            case 'r':                                                   // argument r - rekurencyjne synchronizowanie folderów
                recursive = 1;
                printf("Recurcive synchronisation\n");
                break;
            case 'f':                                                   // argument f - zmiana domyślnego maksymalnego rozmiaru pliku kopiowanego funkcjami read/write 
                MAX_FILE_SIZE = changeMAX_FILE_SIZE(argv[i]);
                printf("Max file size set to: %d bytes.\n", MAX_FILE_SIZE);
                break;
            default:
                break;
            }
        }

        signal(SIGUSR1, &handlerfunction);                              // funkcja obsługująca sygnał SIGUSR1

        pid_t pid = fork();     // stworzenie procesu - demona

        if (pid < 0)            // fork failed
        {
            return 0;
        }
        else if(pid == 0)       // pid - child
        {
            if(recursive == 0)
            {
                infiniteDemonFunction(source, destination, sleep_time, MAX_FILE_SIZE);              // wywołanie nieskończonej funkcji synchronizującej foldery
            }
            else
            {
                RecursiveInfiniteDemonFunction(source, destination, sleep_time, MAX_FILE_SIZE);     // wywołanie nieskończonej funkcji synchronizującej foldery rekurencyjnie
            }
            
            exit(0);
        }

        // pid - parent
        if(signalSIGUSR1 == 1)          // opcja -s (synchronizacja natychmiastowa)
        {
            kill(pid, SIGUSR1);
        }
    }
    
    free(src);
    free(dest);
    return 0;
}