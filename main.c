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

char* src;
char* dest;
int recursive = 0;
int MAX_FILE_SIZE = 1000000;

void handlerfunction()
{
    signalSIGUSR1Handler(src, dest, recursive, MAX_FILE_SIZE);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        usage();
    }
    else
    {
        char source[strlen(argv[1])];           // ścieżka źródła
        strcpy(source, argv[1]);
        char destination[strlen(argv[2])];      // ścieżka celu
        strcpy(destination, argv[2]);

        src = malloc( sizeof(char) * ( strlen(source) + 1 ) );
        dest = malloc( sizeof(char) * ( strlen(destination) + 1 ) );
        snprintf(src, strlen(source) + 1, "%s", source);
        snprintf(dest, strlen(destination) + 1, "%s", destination);

        if(isDirectory(source) == 0 || isDirectory(destination) == 0)
        {
            printf("BŁĄD - podano niepoprawne sciezki (obie powinny byc katalogami)\n");
            return 0;
        }

        int sleep_time = 300;                  // czas snu demona (domyślnie 5 minut)
        int signalSIGUSR1 = 0;

        for(int i=3; i<argc; i++)
        {
            char option = argv[i][1];
            switch (option)
            {
            case 't':
                sleep_time = changeSleepTime(argv[i]);
                printf("Sleep time set to: %d sec.\n", sleep_time);
                break;
            case 's':
                signalSIGUSR1 = 1;
                printf("Signal SIGUSR1 turned on\n");
                break;
            case 'r':
                recursive = 1;
                printf("Recurcive synchronisation\n");
                break;
            case 'f':
                MAX_FILE_SIZE = changeMAX_FILE_SIZE(argv[i]);
                printf("Max file size set to: %d bytes.\n", MAX_FILE_SIZE);
                break;
            default:
                break;
            }
        }

        signal(SIGUSR1, &handlerfunction);

        pid_t pid = fork();

        if (pid < 0)            // fork failed
        {
            return 0;
        }
        else if(pid == 0)       // pid - child
        {
            if(recursive == 0)
            {
                infiniteDemonFunction(source, destination, sleep_time, MAX_FILE_SIZE);
            }
            else
            {
                RecursiveInfiniteDemonFunction(source, destination, sleep_time, MAX_FILE_SIZE);
            }
            
            exit(0);
        }

        // pid - parent
        if(signalSIGUSR1 == 1)
        {
            kill(pid, SIGUSR1);
        }
    }
    
    free(src);
    free(dest);
    return 0;
}