#include <SPI.h>
#include <RF24.h>

// Define the RF24 object with CE and CSN pins
RF24 radio(16, 15); // CE, CSN

// Define the address for the communication pipe
const byte address[6] = "1Node"; 

// Define the struct for the data packet
struct DataPacket {
  char room[10];   // Room name (up to 9 characters + null terminator)
  float value;     // Float value (e.g., temperature)
};

// Declare the received data variable
DataPacket receivedData;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1, address); // Set the address for the receiver
  radio.setPALevel(RF24_PA_LOW);     // Set power level
  radio.startListening();            // Set the module as a receiver
  Serial.println("Setupd ready");
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData)); // Read the data packet
    Serial.print("Received data from room: ");
    Serial.print(receivedData.room);
    Serial.print(", Value: ");
    Serial.println(receivedData.value);

    // Send acknowledgment
    radio.stopListening();
    const char ack[] = "Data Received";
    radio.writeAckPayload(1, &ack, sizeof(ack)); // Send acknowledgment payload
    radio.startListening();
  }
  else
  {
    Serial.println("Not ready");
  }
}
