#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Update pin definitions to actual GPIO numbers
#define CE_PIN   2    // GPIO2 (D4)
#define CSN_PIN  15   // GPIO15 (D8)
#define IRQ_PIN  16   // GPIO16 (D0)

RF24 radio(CE_PIN, CSN_PIN);

struct DataPacket {
  char roomName[32];
  float temperature;
};

volatile bool dataReceived = false;
DataPacket receivedData;

void setup() {
  Serial.begin(115200);

  // Initialize the IRQ pin
  pinMode(IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), onDataReceived, FALLING);

  // Initialize the RF24 radio
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Adjust the address as needed
  radio.startListening();

  Serial.println("Receiver ready and listening...");
}

void loop() {
  if (dataReceived) {
    dataReceived = false;

    // Ensure data is available
    if (radio.available()) {
      // Read the data into the struct
      radio.read(&receivedData, sizeof(receivedData));

      // Print the received data
      Serial.print("Room: ");
      Serial.println(receivedData.roomName);
      Serial.print("Temperature: ");
      Serial.println(receivedData.temperature);
    }
  }
}

// Interrupt service routine for IRQ
void onDataReceived() {
  dataReceived = true;
}
