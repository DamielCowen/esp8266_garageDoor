#include "credentials.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>




// Variables for the sonic range finder
const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;


//Variables for wifi
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
uint8_t broadcastAddress[] = {0x4c, 0x75, 0x25, 0x37, 0x97, 0x6c}; //4c:75:25:38:96:98 4C:75:25:37:97:6C
typedef struct struct_message {
            float distance; 
}struct_message;
struct_message myData;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
            //  Serial.println("Found a channel");
            //  Serial.println(WiFi.channel(i));
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}


void setup() {
  //Set up sonic range finder
  Serial.begin(115200); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  //int32_t channel = getWiFiChannel(WIFI_SSID);
  int32_t channel = 6;

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  //Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo)); //https://github.com/espressif/arduino-esp32/issues/6029
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  //Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    
    return;
  }
}


void loop() {
  //measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distanceCm = duration * SOUND_SPEED/2;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  myData.distance = distanceCm;
  esp_now_send(0, (uint8_t *) &myData, sizeof(myData));

  delay(500);
}
