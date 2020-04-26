#ifndef DEMON_H
#define DEMON_H

int changeSleepTime(char* arg);
void sleepInSec(unsigned int sec);

void demonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE);
void infiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE);
void signalSIGUSR1Handler(char* source, char* destination, int recursive, int MAX_FILE_SIZE);

void deleteFilesFromDestination(char* source, char* destination);
void modifyOrCreateFilesFromSourceToDestination(char* source, char* destination, int MAX_FILE_SIZE);

// -r (katalogi)

void RemoveDirectory(char* source, char* destination);
void RecursiveCreateDirectory(char* source, char* destination, int MAX_FILE_SIZE);

void RecursiveDemonFunction(char* source, char* destination, int sleepTime, int MAX_FILE_SIZE);
void RecursiveInfiniteDemonFunction(char* source, char* destination, int sleep_time, int MAX_FILE_SIZE);

#endif