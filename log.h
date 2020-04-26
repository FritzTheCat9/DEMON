#ifndef LOG_H
#define LOG_H

char* getLocatDateTime();

int logZapisDoPliku(char* message);
int logZapisDoLoguSystemowego(char* message);

void logUspienieDemona();                           // LOG - DEMON
void logObudzenieDemona();
void logUspienieDemonaSygnal();
void logObudzenieDemonaSygnal();

void logKopiaPliku(char* path);                     // LOG - PLIK
void logModyfikacjaPliku(char* path);
void logUsunieciePliku(char* path);

void logKopiaPlikuMMAP(char* path);                 // LOG - PLIK - MMAP
void logModyfikacjaPlikuMMAP(char* path);

void logKopiaKatalogu(char* path);                  // LOG - KATALOG
void logUsuniecieKatalogu(char* path);

#endif