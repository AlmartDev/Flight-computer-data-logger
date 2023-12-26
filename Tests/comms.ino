#include <SPI.h>              
#include <LoRa.h>

void setup() {
  Serial.begin(9600);               
  while (!Serial);

  Serial.print("Initializing LoRa comms  -  ");

  if (!LoRa.begin(915E6)) {         // initialize ratio at 915 MHz
    Serial.println("ERROR");
    while (true);                   
  }
  else {
    Serial.println("done.");
  }

  LoRa.dumpRegisters(Serial);
}


void loop() {
}

