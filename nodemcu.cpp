#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30100_PulseOximeter.h"

#define WIFI_SSID "Kata sandi 123"
#define WIFI_PASSWORD "internet"
#define DATABASE_URL "database-mikro-default-rtdb.firebaseio.com"
#define SS_PIN D8
#define RST_PIN D0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;

uint32_t tsLastReport = 0;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

const unsigned char bitmap [] PROGMEM=
{
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x0f, 0xe0, 0x7f, 0x00, 0x3f, 0xf9, 0xff, 0xc0,
0x7f, 0xf9, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0,
0xff, 0xf7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0x7f, 0xdb, 0xff, 0xe0,
0x7f, 0x9b, 0xff, 0xe0, 0x00, 0x3b, 0xc0, 0x00, 0x3f, 0xf9, 0x9f, 0xc0, 0x3f, 0xfd, 0xbf, 0xc0,
0x1f, 0xfd, 0xbf, 0x80, 0x0f, 0xfd, 0x7f, 0x00, 0x07, 0xfe, 0x7e, 0x00, 0x03, 0xfe, 0xfc, 0x00,
0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x3f, 0xc0, 0x00,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


int getPos();
String ReadRFID();
void onBeatDetected();
void pushToFirebase(String data, float bpm, float spo2);
String path = "/database/";

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    config.database_url = DATABASE_URL;
    config.signer.test_mode = true;
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever===
    }
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);
    
    display.println("Initializing pulse oximeter..");
    display.display();
    Serial.print("Initializing pulse oximeter..");
    
    if (!pox.begin()) {
    Serial.println("FAILED");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);
    display.println("FAILED");
    display.display();
    for(;;);
    } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);
    display.println("SUCCESS");
    display.display();
    Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected); 
}

void loop()
{
    if (Firebase.getInt(fbdo, path + "state")) {
          if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer) {
              int state = fbdo.to<int>();
              if(state == 1){
                Serial.print("Please tap your rfid!");
                delay(3000);
                String rfid = ReadRFID();
                if(rfid != ""){
                  Serial.print("Please put your finger on sensor!");
                  //do read bpm
                  int pos = getPos();                  
                  if(pos != -1){
                    Firebase.setInt(fbdo, path + "total", pos+1);
                    int counter = 0;
                    while(counter <= 60){
                      pox.update();
                      if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
                        Serial.print("Heart BPM:");
                        Serial.print(pox.getHeartRate());
                        Serial.print("-----");
                        Serial.print("Oxygen Percent:");
                        Serial.print(pox.getSpO2());
                        Serial.println("\n");
                        display.clearDisplay();
                        display.setTextSize(1);
                        display.setTextColor(1);
                        display.setCursor(0,16);
                        display.println(pox.getHeartRate());
                       
                        display.setTextSize(1);
                        display.setTextColor(1);
                        display.setCursor(0, 0);
                        display.println("Heart BPM");
                       
                        display.setTextSize(1);
                        display.setTextColor(1);
                        display.setCursor(0, 30);
                        display.println("Spo2");
                       
                        display.setTextSize(1);
                        display.setTextColor(1);
                        display.setCursor(0,45);
                        display.println(pox.getSpO2());
                        display.display();
                        tsLastReport = millis();
                        pushToFirebase(rfid, pox.getHeartRate(), pox.getSpO2());
                      }
                      counter++;
                      delay(1000);
                    }
                  }                  
                }else{
                  Serial.print("Failed to read rfid");
                }
              }
            }
      } else {
        Serial.println(fbdo.errorReason());
      }
}

// function
int getPos(){
  int pos;
  if (Firebase.getInt(fbdo, path + "total")) {
    if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer) {
        pos = fbdo.to<int>();
    }
  }else{
    pos = -1;
  }

  return pos;
}

void pushToFirebase(string data, float bpm, float spo2){
  bool data = Firebase.setString(fbdo, path + "data/"+ pos + "/uid", data);
  bool bpm = Firebase.setFloat(fbdo, path + "data/"+ pos + "/rate", bpm);
  bool spo2 = Firebase.setFloat(fbdo, path + "data/"+ pos + "/co2", spo2);
  if (data && bpm && spo2)
  {
    Serial.println("Successfully push data to firebase");
  }else{
    Serial.println("Failed push data to firebase");
  }
}

String ReadRFID(){
  String strID = "";
  if (rfid.PICC_IsNewCardPresent() || rfid.PICC_ReadCardSerial())
  {
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      for (byte i = 0; i <rfid.uid.size; i++) {
        strID +=
          (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(rfid.uid.uidByte[i], HEX) +
          (i != rfid.uid.size-1 ? ":" : "");
      }
  }
      
  if(strID != ""){
    strID.toUpperCase();
    Serial.print("Kartu ID Anda : ");
    Serial.println(strID);
  }else{
    strID = "";
  }

  return strID;
}

void onBeatDetected()
{
  Serial.println("Beat!");
  display.drawBitmap( 60, 20, bitmap, 28, 28, 1);
  display.display();
}
