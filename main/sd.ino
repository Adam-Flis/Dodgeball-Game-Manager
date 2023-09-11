/*******************************************************
 * Intellectual Property Notice
 *
 * All the contents of this software/code, including but not limited to
 * algorithms, designs, methodologies, and ideas (collectively referred
 * to as "the Contents"), are the intellectual property of Digital Sports Solutions
 * LLC. The Contents are protected by intellectual property laws and all rights
 * are reserved.
 *
 * Unauthorized use, reproduction, or distribution of the Contents,
 * in whole or in part, is strictly prohibited and may result in legal
 * action.
 *
 * For inquiries or licensing opportunities, please contact
 * adamflis2002@gmail.com
 *******************************************************/

#include "main.hpp"

void initSDCard(){
  if(!SD.begin()){
    Serial.println(F("Card Mount Failed"));
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println(F("No SD card attached"));
    return;
  }

  Serial.print(F("SD Card Type: "));
  if(cardType == CARD_MMC){
    Serial.println(F("MMC"));
  } else if(cardType == CARD_SD){
    Serial.println(F("SDSC"));
  } else if(cardType == CARD_SDHC){
    Serial.println(F("SDHC"));
  } else {
    Serial.println(F("UNKNOWN"));
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void writeData(String fileName, String data) {
  File file = SD.open("/"+fileName, FILE_WRITE);
  if(!file){
    Serial.println(F("Failed to open file for writing"));
    return;
  }
  if(file.print(data)){
    Serial.println(F("File written"));
  } else {
    Serial.println(F("Write failed"));
  }
  file.close();
}

void readData(String fileName) {
    File file = SD.open(fileName);
    if(!file){
        Serial.println(F("Failed to open file for reading"));
        return;
    }
    Serial.println(F("File Content:"));
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}