#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Arduino_JSON.h>

#define analogPin A0;
int adcValue = 0;
int LED = 4;

typedef struct struct_message{
  float reading;
} struct_message;
struct_message myData;

JSONVar recvData;
float x =0 ;

void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  //recvData["reading"] = myData.reading;
  x = myData.reading;
  Serial.print("Reading: ");
  Serial.println(recvData["reading"]);
  Serial.println(x);
}
void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  // initate LED
  pinMode(LED, OUTPUT);

}

void loop() {
  //adcValue = recvData["reading"];
  adcValue = x;
  if (adcValue> 500){
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
  

}
