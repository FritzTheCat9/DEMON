#ifndef DEMON_H
#define DEMON_H

// zmiana czasu snu demona
int changeSleepTime(char* arg);
// zaśnięcie demona przez kilka sekund
void sleepInSec(unsigned int sec);


// główna funkcja demona (zwykłe pliki)
void demonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE);
// funkcja wywołująca w nieskończoność główną funkcję demona (zwykłe pliki)
void infiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE);


// funkcja obsługująca sygnał SIGUSR1
void signalSIGUSR1Handler(char* source, char* destination, int recursive, int MAX_FILE_SIZE);


// usunięcie plików które są w folderze destination a nie ma ich w folderze source (zwykłe pliki)
void deleteFilesFromDestination(char* source, char* destination);
// skopiowanie pliku z katalogu źródłowego do docelowego lub zmiana pliku zmodyfikowanego w katalogu docelowym, który nie jest zmodyfikowany w katalogu źródłowym
void modifyOrCreateFilesFromSourceToDestination(char* source, char* destination, int MAX_FILE_SIZE);


// -r (katalogi)


// usunięcie katalogów z destination, których nie ma w source 
void RemoveDirectory(char* source, char* destination);
// REKURENCYJNE skopiowanie pliku/folderu z katalogu źródłowego do docelowego lub zmiana pliku/folderu zmodyfikowanego w katalogu docelowym, który nie jest zmodyfikowany w katalogu źródłowym
void RecursiveCreateDirectory(char* source, char* destination, int MAX_FILE_SIZE);


// główna funkcja demona (zwykłe pliki i katalogi)
void RecursiveDemonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE);
// funkcja wywołująca w nieskończoność główną funkcję demona (zwykłe pliki i katalogi)
void RecursiveInfiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE);

#endif