#ifndef LOG_H
#define LOG_H

// pobiera aktualną date i czas
char* getLocatDateTime();


// zapisuje wiadomość do pliku (logFile)
int logZapisDoPliku(char* message);
// zapisuje wiadomość do logu systemowego (/var/log/syslog)
int logZapisDoLoguSystemowego(char* message);


// wypisanie informacji o naturalnym uśpieniu demona                        // LOG - DEMON
void logUspienieDemona();
// wypisanie informacji o naturalnym obudzeniu demona
void logObudzenieDemona();
// wypisanie informacji o uśpieniu demona poprzez sygnał
void logUspienieDemonaSygnal();
// wypisanie informacji o obudzeniu demona poprzez sygnał
void logObudzenieDemonaSygnal();


// wypisanie informacji o wykonaniu kopi pliku (funkcje read/write )              // LOG - PLIK
void logKopiaPliku(char* path);
// wypisanie informacji o wykonaniu modyfikacji pliku
void logModyfikacjaPliku(char* path);
// wypisanie informacji o wykonaniu usuniecia pliku
void logUsunieciePliku(char* path);


// wypisanie informacji o wykonaniu kopi pliku (funkcje mmap/write)               // LOG - PLIK - MMAP
void logKopiaPlikuMMAP(char* path);
// wypisanie informacji o wykonaniu modyfikacji pliku
void logModyfikacjaPlikuMMAP(char* path);


// wypisanie informacji o wykonaniu kopii katalogu                          // LOG - KATALOG
void logKopiaKatalogu(char* path);
// wypisanie informacji o wykonaniu usuniecia katalogu
void logUsuniecieKatalogu(char* path);

#endif