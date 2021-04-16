#ifndef MPU6050_H_
#define MPU6050_H_

#include <Arduino.h>

void initMPU();

String getGyroReadings();
String getAccReadings();
String getTemperature();

#endif /* MPU6050_H_ */