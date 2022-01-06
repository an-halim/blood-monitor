#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30100_PulseOximeter.h"

#define WIFI_SSID "VV"
#define WIFI_PASSWORD "matematika"
#define DATABASE_URL "database-mikro-default-rtdb.firebaseio.com"

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

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String path = "/database/";
uint32_t tsLastReport = 0;
int lastpos =  0;
 
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
void onBeatDetected()
{
  Serial.println("Beat!");
  display.drawBitmap( 60, 20, bitmap, 28, 28, 1);
  display.display();
}

int getPos(){
  int pos;
  Serial.print("Mencoba mengambil lokasi terakhir : ");
  if(Firebase.getInt(fbdo, path + "total")){
    if(fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer){
      pos = fbdo.intData();
      Serial.println(pos);
    }
  }
  if(pos < 0) pos = 0;
  
  return pos;
}

int getState(){
  int state;
  Serial.print("Mencoba membaca perintah : ");
  if(Firebase.getInt(fbdo, path + "state")){
    if(fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer){
      state = fbdo.intData();
      Serial.println(state);
    }
  }
  if(state < 0) state = 0;
  
  return state;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();

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

    config.database_url = DATABASE_URL;
    config.signer.test_mode = true;
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    delay(2000);
    lastpos = getPos();
    
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

  bool startPox = pox.begin(); 
  if (!startPox) {    
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

#define arrSize 60
float databpm[arrSize];
float dataspo2[arrSize];
float avgbpm,avgspo2;
int i = 0;
long int realtimeDelay = 0;
long int finalDelay =0;
int state = 0; // controller scan

void loop() {
  if(state == 1){
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {   
      Serial.print("Heart BPM:");
      Serial.print(pox.getHeartRate());
      Serial.println();
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
      databpm[i] = pox.getHeartRate();
      dataspo2[i] = pox.getSpO2();
    
  
  
      //send realtime to firebase
      if(millis() - realtimeDelay > 4000 ){
        if(databpm[i] != 0 && dataspo2[i] != 0){
          Serial.println("Mencoba mengirim hasil realtime ke firebase...");
          String fullPathBPM = path + "data/"+ lastpos + "/bpm";
          String fullPathSpo2 = path + "data/"+ lastpos + "/spo2";
  
          Serial.println("Path BPM "+fullPathBPM); 
          Serial.println("Path SpO2 "+fullPathSpo2); 
          bool setPos = Firebase.setInt(fbdo, path + "total", lastpos+1);
          bool setbpm = Firebase.setFloat(fbdo, fullPathBPM, databpm[i]);
          bool setOxi = Firebase.setFloat(fbdo, fullPathSpo2, dataspo2[i]);  
          if(setPos && setbpm && setOxi){
            Serial.println("Sukses mengirim data ke firebase!");          
          }else{
            Serial.println(fbdo.errorReason());
          }
          pox.begin();
        }    
        realtimeDelay = millis();
      } 
      
  
      // send final result to firebase
      if(millis() - finalDelay > 60000 ){
        i = 0;
        avgbpm = 0;
        avgspo2 = 0;
        int successTes = 0;
        for(int j = 0; j < 60; j++){
          if(databpm[j] <= 0 && dataspo2[j] <= 9){
            //do nothing
          }else{
            avgbpm += databpm[j];
            avgspo2 += dataspo2[j];
            successTes++;
          }        
        }
        avgbpm /= successTes;
        avgspo2 /= successTes;
        Serial.print("AVG Heart BPM:");
        Serial.print(avgbpm);
        Serial.println();
        Serial.print("Oxygen Percent:");
        Serial.print(avgspo2);
        Serial.println();
        if(avgbpm != 0 && avgspo2 != 0){        
          Serial.println("Mencoba mengirim hasil final ke firebase...");
          String fullPathBPM = path + "data/"+ lastpos + "/bpm";
          String fullPathSpo2 = path + "data/"+ lastpos + "/spo2";
          Serial.println("Path BPM "+fullPathBPM); 
          Serial.println("Path SpO2 "+fullPathSpo2); 
          bool setPos = Firebase.setInt(fbdo, path + "total", lastpos+1);
          bool setbpm = Firebase.setFloat(fbdo, fullPathBPM, avgbpm);
          bool setOxi = Firebase.setFloat(fbdo, fullPathSpo2, avgspo2);  
          if(setPos && setbpm && setOxi){
            Serial.println("Sukses mengirim data ke firebase!");          
          }else{
            Serial.println(fbdo.errorReason());
          }
          lastpos++;
          pox.begin();
        }    
        state = 0;
        finalDelay = millis();
      }   
       
      tsLastReport = millis();
      i++;
    }
  }else{
    state = getState();
    if(state == 1){
      pox.begin();
    }    
  }
   
}