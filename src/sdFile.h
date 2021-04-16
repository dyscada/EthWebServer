#ifndef SDFILE_H_
#define SDFILE_H_

#include "FS.h"

// Define CS pin for the SD card module
#define SD_CS 13

extern void getTimeStamp();

void init_SD();
void check_SD();
void writeFile(fs::FS &fs, const char *path, const char *message);
void logError();
void logData();
void appendError(fs::FS &fs, const char *path, const char *message);
void appendData(fs::FS &fs, const char *path, const char *message);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

#endif /* SDFILE_H_ */