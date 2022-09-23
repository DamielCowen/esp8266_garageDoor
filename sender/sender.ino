#include "credentials.h"
#include <ESP8266WiFi.h>
#include <espnow.h>


#define analogPin A0 
int adcValue = 0; 
int LED = 5;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

uint8_t broadcastAddress[] = {0x4c, 0x75, 0x25, 0x38, 0x96, 0x98}; /*4c:75:25:38:96:98 */

typedef struct struct_message {
            float reading; 
}struct_message;

struct_message myData;


void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  //WiFi.begin(ssid,password);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  adcValue = analogRead(analogPin);
  if (analogRead(analogPin)> 500){

    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
  /* Print the output in the Serial Monitor */
  Serial.print("ADC Value = ");
  Serial.println(adcValue);
  /*Send data */
  myData.reading = adcValue;
  esp_now_send(0, (uint8_t *) &myData, sizeof(myData));

  
  delay(500);

}
