#ifndef MAIN_H_
#define MAIN_H_

#include "FS.h"

// Replace with your network credentials
const char *ssid = "DaeYang";
const char *password = "daeyang5";

void initWiFi();
void initEthernet();
void initSPIFFS();
void initMPU();
void init_SD();
void getTimeStamp();


void writeFile(fs::FS &fs, const char *path, const char *message);
void appendData(fs::FS &fs, const char *path, const char *message);
void appendError(fs::FS &fs, const char *path, const char *message);

String getGyroReadings();
String getAccReadings();
String getTemperature();

#endif /* MAIN_H_ */