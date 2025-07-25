#ifndef ESPNOW_H_
#define ESPNOW_H_

//***********ESP-NOW遥控相关*****************
#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j2PotX;
  byte j2PotY;
  bool button1;
  bool button2;
  bool button3;
  bool button4;
  bool buttonL1;
  bool buttonL2;
  bool buttonL3;
  bool buttonL4;
  bool j1Button;
  bool j2Button;
  bool button7;
  bool button8;
  bool tSwitch1;
  bool tSwitch2;
  bool button11;
  byte pot1;    
  byte roll;
  byte pitch;
};
Data_Package data; //Create a variable with the above structure


void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
  data.buttonL1 = 1;
  data.buttonL2 = 1;
  data.buttonL3 = 1;
  data.buttonL4 = 1;
  data.j1Button = 1;
  data.j2Button = 1;
  data.button7 = 1;
  data.button8 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button11 = 1;
  data.pot1=127;
  data.roll=127;
  data.pitch=127;
}

/////////////////////////////////////////

void InitESPNow();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_add, const uint8_t *data, int data_len);
void sendData();//uint8_t data

// 数据发送回调函数
// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
 // sendTime = millis();//////////////////////////////
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
           
//  Serial.print("Last Packet Sent to: ");
//  Serial.println(macStr);
//  Serial.print("Last Packet Send Status: ");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// 数据接收回调函数
// callback when data is received from Slave to Master
void OnDataRecv(const uint8_t *mac_add, const uint8_t *data, int data_len)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_add[0], mac_add[1], mac_add[2], mac_add[3], mac_add[4], mac_add[5]);
           
//  Serial.print("Last Packet Recv from: ");
//  Serial.println(macStr);
//  Serial.print("Last Packet Recv Data: ");
//  Serial.println(*data);
//  Serial.println("");
}

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// send data
void sendData() {//uint8_t data  
  // this will broadcast a message to everyone in range
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
    esp_now_add_peer(&peerInfo);
  }  
  // 发送数据
  //esp_err_t result = esp_now_send(broadcastAddress, (const uint8_t *)message.c_str(), message.length());
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  // and this will send a message to a specific device
  /*uint8_t peerAddress[] = {0x3C, 0x71, 0xBF, 0x47, 0xA5, 0xC0};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, peerAddress, 6);
  if (!esp_now_is_peer_exist(peerAddress))
  {
    esp_now_add_peer(&peerInfo);
  }
  esp_err_t result = esp_now_send(peerAddress, (const uint8_t *)message.c_str(), message.length());*/
//  if (result == ESP_OK){
//    Serial.println("Broadcast message success");
//  }else if (result == ESP_ERR_ESPNOW_NOT_INIT){
//    Serial.println("ESPNOW not Init.");
//  }else if (result == ESP_ERR_ESPNOW_ARG){
//    Serial.println("Invalid Argument");
//  }else if (result == ESP_ERR_ESPNOW_INTERNAL){
//    Serial.println("Internal Error");
//  }else if (result == ESP_ERR_ESPNOW_NO_MEM){
//    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
//  }else if (result == ESP_ERR_ESPNOW_NOT_FOUND){
//    Serial.println("Peer not found.");
//  }else{
//    Serial.println("Unknown error");
//  }
}

  


#endif  
