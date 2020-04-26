#include "log.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>

char* getLocatDateTime()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char* year = malloc( sizeof(char) * ( 4 + 1 ) );
	sprintf(year, "%d", tm.tm_year + 1900);
	char* month = malloc( sizeof(char) * ( 2 + 1 ) );
	sprintf(month, "%02d", tm.tm_mon + 1);
	char* day = malloc( sizeof(char) * ( 2 + 1 ) );
	sprintf(day, "%02d", tm.tm_mday);
	char* hour = malloc( sizeof(char) * ( 2 + 1 ) );
	sprintf(hour, "%02d", tm.tm_hour);
	char* min = malloc( sizeof(char) * ( 2 + 1 ) );
	sprintf(min, "%02d", tm.tm_min);
	char* sec = malloc( sizeof(char) * ( 2 + 1 ) );
	sprintf(sec, "%02d", tm.tm_sec);

	int date_len = strlen(year) + strlen(month) + strlen(day) + strlen(hour) + strlen(min) + strlen(sec) + 6;

	char* date = malloc( sizeof(char) * ( date_len ) );
	snprintf(date, date_len, "%s-%s-%s %s:%s:%s", year, month, day, hour, min, sec);

	free(year); free(month); free(day); free(hour); free(min); free(sec);

	return date;
}

int logZapisDoPliku(char* message)
{
    int fPointer;
	fPointer = open("logFile", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
	if(fPointer == -1)
	{
	    return 0;
	}

	int date_len = 19;
	char* date = getLocatDateTime();

	int message_n_len = strlen(message) + date_len + 5;
	char* message_n = malloc( sizeof(char) * ( message_n_len ) );
    snprintf(message_n, message_n_len, "%s | %s\n", date, message);

	write(fPointer, message_n, strlen(message_n));

	free(date);
	free(message_n);
    close(fPointer);
    return 1;
}

int logZapisDoLoguSystemowego(char* message)
{
	syslog(LOG_INFO, "%s", message);
}

void logUspienieDemona()
{
    logZapisDoPliku("[DEMON] \t- Uspienie demona (Naturalne)");
	logZapisDoLoguSystemowego("[DEMON] - Uspienie demona (Naturalne)");
}

void logObudzenieDemona()
{
	logZapisDoPliku("[DEMON] \t- Obudzenie demona (Naturalne)");
	logZapisDoLoguSystemowego("[DEMON] - Obudzenie demona (Naturalne)");
}

void logUspienieDemonaSygnal()
{
    logZapisDoPliku("[DEMON] \t- Uspienie demona (Sygnal)");
	logZapisDoLoguSystemowego("[DEMON] - Uspienie demona (Sygnal)");
}

void logObudzenieDemonaSygnal()
{
	logZapisDoPliku("[DEMON] \t- Obudzenie demona (Sygnal)");
	logZapisDoLoguSystemowego("[DEMON] - Obudzenie demona (Sygnal)");
}

void logKopiaPliku(char* path)
{
	char info[23] = "[PLIK] \t- Kopia pliku ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logModyfikacjaPliku(char* path)
{
	char info[29] = "[PLIK] \t- Modyfikacja pliku ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logUsunieciePliku(char* path)
{
	char info[27] = "[PLIK] \t- Usuniecie pliku ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logKopiaPlikuMMAP(char* path)
{
	char info[30] = "[PLIK] \t- Kopia pliku (MMAP) ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logModyfikacjaPlikuMMAP(char* path)
{
	char info[36] = "[PLIK] \t- Modyfikacja pliku (MMAP) ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logKopiaKatalogu(char* path)
{
	char info[29] = "[KATALOG] - Kopia katalogu ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}

void logUsuniecieKatalogu(char* path)
{
	char info[33] = "[KATALOG] - Usuniecie katalogu ";

	int info_len = strlen(info);
	int path_len = strlen(path);
	int message_length = path_len + info_len + 1;

	char* message = malloc( sizeof(char) * ( message_length ) );
    snprintf(message, message_length, "%s%s", info, path);

	logZapisDoPliku(message);
	logZapisDoLoguSystemowego(message);

	free(message);
}