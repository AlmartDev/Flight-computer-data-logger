#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void initSDBoard() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR");
    while (1);
  }
  else 
    Serial.println("done.");
}

void setuo() {
    Serial.begin(9600);
    initSDBoard();
    writeToSD("Hey, look at this cool data!!");
}

void writeToSD(const char data) {
  String dataString = data;

  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}