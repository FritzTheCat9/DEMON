#ifndef FILE_H
#define FILE_H

// czy plik jest katalogiem
int isDirectory(char* path);
// czy plik jest zwykłym plikiem
int isRegularFile(char* path);

// kopiowanie pliku
int copyFile(char* source, char* destination);
// czy pliki maja taką samą datę modyfikacji
int areHavingEqualModificationDate(char* source, char* destination);
// zmiana daty modyfikacji pliku
int changeModificationDate(char* pathToChangeDate, char* pathToGetDate);
// czy plik jest w katalogu
int isThisFileInADirectory(char* fileName, char* directoryName);
// usunięcie pliku
int removeFile(char* path);
// pobranie rozmiaru pliku
int getFileSize(char* path);
// zmapowanie pliku w pamięci
int mapFile(char* source_mapp, char* destination_write);
// zmiana maksymalnego rozmiaru pliku kopiowanego funkcjami read/write
int changeMAX_FILE_SIZE(char* arg);

#endif