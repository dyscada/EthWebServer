#include "sdFile.h"
#include "mpu6050.h"

#include "SD.h"

String currFileName;
int lastFileNameIndex;
int currFileNameIndex;

// Save reading number on RTC memory
//RTC_DATA_ATTR int readingID = 0;
extern int errorID;
extern int readingID;

extern String dayStamp;
extern String timeStamp;
extern String fileNameString;
extern int fileNameIndex;
extern String fileName;

extern String errorContents;
extern String dataMessage;
extern String errorMessage;

//
void logData()
{
  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  //getTimeStamp();
  check_SD();
  //Serial.println(fileNameString);
  fileName = "/" + fileNameString + ".txt";

  File fileData = SD.open(fileName.c_str());
  if (!fileData)
  {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, fileName.c_str(), "Reading ID, Hour, Temperature \r\n");
  }
  else
  {
    //Serial.println("File already exists");
  }
  fileData.close();
  dataMessage = String(readingID) + "," + String(timeStamp) + "," + String(getTemperature().c_str()) + " --> ";
  readingID++;
  Serial.print("data: ");
  Serial.print(dataMessage);
  //appendData(SD, "/data.txt", dataMessage.c_str());
  appendData(SD, fileName.c_str(), dataMessage.c_str());
}

void appendData(fs::FS &fs, const char *path, const char *message)
{
  //Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Fail to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.printf("Appended to %s\n", path);
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void logError()
{
  errorMessage = String(errorID) + "," + String(dayStamp) + "," + String(timeStamp) + "," +
                 errorContents + "\r\n";
  errorID++;
  Serial.print("error: ");
  Serial.print(errorMessage);
  appendError(SD, "/error.txt", errorMessage.c_str());
}

void appendError(fs::FS &fs, const char *path, const char *message)
{
  //Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Fail to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.printf("Appended to %s\n", path);
    //Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

// If SD Card short memory
// Remove old data

void check_SD()
{
  uint64_t bytesAvailable = SD.totalBytes();
  int spaceAvailableInMB = bytesAvailable / (1024 * 1024);
  if (spaceAvailableInMB < 100)
  {
    Serial.printf("Space available: %u MB \n", spaceAvailableInMB);
  }
  else
  {
    //Serial.printf("Space available: %u MB \n", spaceAvailableInMB);
  }

  //Serial.println(fileNameString);

  fileNameIndex = fileNameString.toInt();
  //fileNameIndex = fileNameIndex - 1;

  //Serial.println(String(fileNameIndex));
}


void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  lastFileNameIndex = 99999999;
  
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print(" DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print(" FILE: ");
      Serial.print(file.name());
      Serial.print(" SIZE: ");
      Serial.println(file.size());
      currFileName = file.name();
      currFileName= currFileName.substring(1,9);
      currFileNameIndex = currFileName.toInt();
    }
    file = root.openNextFile();

    // currFileName = file.name();
    // currFileName= currFileName.substring(1,9);
    // Serial.println(currFileName);
    // currFileNameIndex = currFileName.toInt();
    // if ((lastFileNameIndex - currFileNameIndex) > 0)
    // {
       //lastFileNameIndex = currFileNameIndex;
    // }
  }
   String myString = String(currFileNameIndex);
   Serial.println(myString);
}

// Initialize SD card
void init_SD()
{
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS))
  {
    Serial.println("ERROR - SD card initialization failed!");
    return; // init failed
  }
  uint64_t cardSize = SD.cardSize();
  int cardSizeInMB = cardSize / (1024 * 1024);
  Serial.printf("Card size: %u MB \n", cardSizeInMB);

  uint64_t bytesAvailable = SD.totalBytes();
  int spaceAvailableInMB = bytesAvailable / (1024 * 1024);
  Serial.printf("Space available: %u MB \n", spaceAvailableInMB);

  uint64_t spaceUsed = SD.usedBytes();
  int spaceUsedInMB = spaceUsed;
  Serial.printf("Space used: %u bytes \n", spaceUsedInMB);

  listDir(SD, "/", 0);
}
