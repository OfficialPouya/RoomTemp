#include <SPI.h>
#include <RF24.h>

// Define the pins for the nRF24L01
#define CE_PIN 9
#define CSN_PIN 10

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Define the address (the pipe through which we'll communicate)
const byte address[6] = "00001";

// Define a structure to hold the room name and temperature
struct DataPacket {
  char roomName[32]; // Room name (adjust size as needed)
  float temperature; // Temperature value
};

void setup() {
  Serial.begin(9600);

  // Initialize the radio
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    while (1) {}
  }

  radio.openWritingPipe(address); // Set the address for transmission
  radio.setPALevel(RF24_PA_LOW);  // Set the power level (can also be RF24_PA_MIN, RF24_PA_HIGH, or RF24_PA_MAX)
  radio.stopListening();          // Set the module to transmitter mode
}

void loop() {
  // Create an instance of DataPacket and populate it
  DataPacket dataToSend;
  strcpy(dataToSend.roomName, "Living Room"); // Copy the room name into the structure
  dataToSend.temperature = 23.5; // Set the temperature value

  // Send the structure
  bool success = radio.write(&dataToSend, sizeof(dataToSend));
  
  if (success) {
    Serial.println("Data sent successfully");
    Serial.print("Room: ");
    Serial.println(dataToSend.roomName);
    Serial.print("Temperature: ");
    Serial.println(dataToSend.temperature);
  } else {
    Serial.println("Failed to send data");
  }
  
  delay(1000); // Wait for a second before sending the next message
}
