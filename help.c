#include "help.h"

#include <stdio.h>

void usage()
{
    printf("\n");
    printf("FILE DEMON\n\n");
    printf("USE: \t\t\t ./main <source_path> <destination_path> <optional_args>\n\n");
    printf("<source_path> \t\t source directory\n");
    printf("<destination_path> \t destination directory\n");
    printf("<optional_args> \t optional arguments (you can use multiple optional arguments)\n\n");
    printf("OPTIONAL ARGUMENTS: \n\n");
    printf("-t \t <demon_sleep_time (in seconds)>\n");
    printf("\t #Important - Make sure if there is no space between -t and <demon_sleep_time>\n");
    printf("\t #Example - Setting demon sleep time to 10 sec: -t10\n");
    printf("-s \t <SIGUSR1 - immediately_demon_wake_up>\n");
    printf("-r \t recursive folder synchronisation\n");
    printf("-f \t <maximum file size (in bytes)>\n");
    printf("\t #Important - Files below <maximum file size> are copied by read/write functions\n");
    printf("\t #Important - Files above <maximum file size> are copied by mmap/write functions\n");
    printf("\t #Example - Setting maximum file size to 100 bytes: -f100\n");
    printf("\n");
}