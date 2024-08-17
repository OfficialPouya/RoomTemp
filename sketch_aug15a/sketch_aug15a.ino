#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(115200);

  delay(1000);
  
  //Scan for Wifi
  Serial.println("Scanning for WiFi networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) 
  {
    Serial.println("No networks found");
  } 
  else
  {
    Serial.println("Networks found:");
    for (int i = 0; i < n; ++i) 
    {
      Serial.printf("%d: %s, Signal strength (RSSI): %d dBm\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
      delay(10);
      if(WiFi.SSID(i) == ssid)
      {
        Serial.println(ssid);
        Serial.println(WiFi.SSID(i));
        Serial.println("Your network is in range");
        Serial.println("Connecting to WiFi...");
        WiFi.begin(ssid, password);
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(1000);
          Serial.print(".");
        }
        if(WiFi.status() == WL_CONNECTED)
        {
          // Print the IP address
          Serial.println();
          Serial.println("Connected to WiFi");
          Serial.print("IP Address: ");
          Serial.println(WiFi.localIP());
        }
      }
      break;
    }
  }

  delay(100);


  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }


}

void loop() {
  float temperature = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");


  delay(2000); // Wait for 2 seconds before reading again
}