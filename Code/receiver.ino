#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h> 

const int pinCE = 2; 
const int pinCSN = 15;

const byte address[6] = "00001";
RF24 radio(pinCE, pinCSN);
// Define the same structure to receive the data
struct DataPacket {
  char roomName[32];
  float temperature;
};

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    while (1) {}
  }

  radio.openReadingPipe(0, address); // Open a reading pipe
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); // Set the module to receiver mode
}

void loop() {
  if (radio.available()) {
    DataPacket receivedData;
    radio.read(&receivedData, sizeof(receivedData));
    
    Serial.println("Data received:");
    Serial.print("Room: ");
    Serial.println(receivedData.roomName);
    Serial.print("Temperature: ");
    Serial.println(receivedData.temperature);
  }
}