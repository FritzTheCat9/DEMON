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