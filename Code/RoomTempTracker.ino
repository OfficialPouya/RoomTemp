#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "HTTPSRedirect.h"

// Replace with your network credentials
const char* my_ssid = "WIFI NAME";
const char* my_password = "PASSWORD";

//------------------I2C----------------------
Adafruit_BMP280 bmp;
//------------------------------------------

//-------------Host & httpsPort-------------

//---------WiFiClientSecure object----------
// WiFiClientSecure client;
//------------------------------------------

//------------------------------------------
// Enter Google Script Deployment ID:
const char *myGScriptId = "DEPLOYMENT ID";
//------------------------------------------



// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* myhost = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";

String url = String("/macros/s/") + myGScriptId + "/exec";
HTTPSRedirect* client = nullptr;

void setup() 
{
  Serial.begin(115200);
  delay(100);
  connectToWIFI();
  delay(100);
  connectToGoogleScript();
  delay(100);
  connectToSensor();
}


void loop() 
{
  float living_room_temperature = bmp.readTemperature();
  float bedroom_temp = 0;
  float office_temp = 0;
  Serial.print("Living Room Temperature = ");
  Serial.print(living_room_temperature);
  Serial.println(" *C");

  sendData(living_room_temperature, bedroom_temp, office_temp); 
  delay(10000);
}




//------------------connectToWIFI------------------------------
/*
Scans all wifi signals and prints it with signal strength
Attempts to connect to the wifi, will wait until its connected 
*/
//-------------------------------------------------------------
void connectToWIFI()
{
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
    }
     Serial.println("Connecting to WiFi...");
      WiFi.begin(my_ssid, my_password);
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);
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
}



//-------------------------connectToSensor-----------------------
// Connecting to sensor, will wait if cannot connect
//---------------------------------------------------------------
void connectToSensor()
{
    if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}

//------------------connectToGoogleScript------------------
// Connecting to sensor, will wait if cannot connect
//---------------------------------------------------------
void connectToGoogleScript()
{
  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(myhost);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++)
  { 
    int retval = client->connect(myhost, httpsPort);
    if (retval == 1)
    {
       flag = true;
       Serial.println("Connected");
       break;
    }
    else
    {
      Serial.println("Connection failed. Retrying...");
    }
  }
  if (!flag)
  {
    Serial.print("Could not connect to server: ");
    Serial.println(myhost);
    return;
  }
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
}


void sendData(float value0, float value1, float value2)
{
 static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected()){

      client->connect(myhost, httpsPort);
    }
  }

  else
  {
    Serial.println("Error creating client object!");
  }
  
  // Create json object
  payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "\"}";
  
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  // Serial.println(payload); (uncomment for debug)
  if(client->POST(url, myhost, payload))
  { 
    Serial.println("POST SUCCESS");
  }
  else
  {
    Serial.println("Error While Connecting");
  }
 
  delay(5000);
}

