#ifndef FILE_H
#define FILE_H

int isDirectory(char* path);
int isRegularFile(char* path);
int copyFile(char* source, char* destination);
int areHavingEqualModificationDate(char* source, char* destination);
int changeModificationDate(char* pathToChangeDate, char* pathToGetDate);
int isThisFileInADirectory(char* fileName, char* directoryName);
int removeFile(char* path);
int getFileSize(char* path);
int mapFile(char* source_mapp, char* destination_write);
int changeMAX_FILE_SIZE(char* arg);

#endif