/*
Modified by Jean-Jacques DeLisle for Mouser Electronics August 2023
The original authors code and introduction comments are included below.
*/

/*
 This example connects to an unencrypted WiFi network.
 Then it prints the MAC address of the WiFi module,
 the IP address obtained, and other network details.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <WiFiS3.h>
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;


//#include "arduino_secrets.h"

#define SECRET_SSID "SSID"
#define SECRET_PASS "PASSWORD"

//#define SECRET_SSID "Cafe de Leche"
//#define SECRET_PASS "nicaragua"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

// bitmap for a heart symbol
uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void lefteye(){
  //Left eye
  frame[1][3] = 1;
  frame[1][4] = 1;
  frame[2][3] = 1;
  frame[2][4] = 1;
}

void righteye(){
  //Right eye
  frame[1][8] = 1;
  frame[1][9] = 1;
  frame[2][8] = 1;
  frame[2][9] = 1;
}

void wink(){
  //Wink with the left eye
  frame[1][3] = 0;
  frame[1][4] = 0;
  frame[2][3] = 1;
  frame[2][4] = 1;
}

void mouth(){
  //Mouth
  frame[5][3] = 1;
  frame[5][9] = 1;
  frame[6][3] = 1;
  frame[6][4] = 1;
  frame[6][5] = 1;
  frame[6][6] = 1;
  frame[6][7] = 1;
  frame[6][8] = 1;
  frame[6][9] = 1;
  frame[5][4] = 0;
  frame[5][5] = 0;
  frame[5][6] = 0;
  frame[5][7] = 0;
  frame[5][8] = 0;
}

void frown(){
  frame[5][3] = 1;
  frame[5][9] = 1;
  frame[6][3] = 0;
  frame[6][4] = 0;
  frame[6][5] = 0;
  frame[6][6] = 0;
  frame[6][7] = 0;
  frame[6][8] = 0;
  frame[6][9] = 0;
  frame[5][3] = 1;
  frame[5][4] = 1;
  frame[5][5] = 1;
  frame[5][6] = 1;
  frame[5][7] = 1;
  frame[5][8] = 1;
  frame[5][9] = 1;
  frame[6][3] = 1;
  frame[6][9] = 1;
}


void setup() {
  //code to initialize the matrix
  matrix.begin();

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting initial connection to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    lefteye();
    righteye();
    matrix.renderBitmap(frame, 8, 12);
    delay(1000);
    wink();
    matrix.renderBitmap(frame, 8, 12);

    // wait 10 seconds for connection:
    delay(9000);
  }

  // you're connected now, so print out the data:
  lefteye();
  righteye();
  mouth();
  matrix.renderBitmap(frame, 8, 12);
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}


void loop() {

  status = WiFi.status();
  while (status == WL_CONNECTED) {
    lefteye();
    righteye();
    mouth();
    matrix.renderBitmap(frame, 8, 12);
    delay(1000);
    wink();
    matrix.renderBitmap(frame, 8, 12);
    delay(1000);
    status = WiFi.status();
    //Serial.println(status);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    //display frown
    lefteye();
    righteye();
    frown();
    matrix.renderBitmap(frame, 8, 12);

    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // check the network connection once every 10 seconds:
  //printCurrentNet();
  //delay(10000);
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  //byte encryption = WiFi.encryptionType();
  //Serial.print("Encryption Type:");
  //Serial.println(encryption, HEX);
  //Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
