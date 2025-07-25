//#include <WiFi.h>
//#include <esp_now.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

// 设置数据结构体
typedef struct Data_Package {
  byte j1PotX;  //左右摇杆
  byte j1PotY;
  byte j2PotX;
  byte j2PotY;
  bool buttonR1;  //左右按键
  bool buttonR2;
  bool j1Button;  //左右摇杆按键
  bool j2Button;
  bool buttonLB;  //左右扳机按键
  bool buttonRB;
  bool tSwitch1;  //拨动开关   
  byte roll;      //用于M5StackCore2或未来增加的陀螺仪功能
  byte pitch;
  bool buttonR3;
};
Data_Package data; //Create a variable with the above structure

void resetData() {  //数据重置
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.buttonR1 = 1;
  data.buttonR2 = 1;
  data.j1Button = 1;
  data.j2Button = 1;
  data.buttonLB = 1;
  data.buttonRB = 1;
  data.tSwitch1 = 1;
  data.roll=127;
  data.pitch=127;
  data.buttonR3 = 1;
}

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength)
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

// 数据接收回调函数
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("j1PotX: ");
  Serial.print(data.j1PotX);
  Serial.print("; j1PotY: ");
  Serial.print(data.j1PotY); 
  Serial.print("; j2PotX: ");
  Serial.print(data.j2PotX);
  Serial.print("; j2PotY: ");
  Serial.print(data.j2PotY);
  Serial.print("; ***buttonR1: ");
  Serial.print(data.buttonR1);
  Serial.print("; ***buttonR2: ");
  Serial.print(data.buttonR2);
  Serial.print("; buttonR3: ");
  Serial.println(data.buttonR3);
  Serial.print("; ***roll: ");
  Serial.print(data.roll);
  Serial.print("; pitch: ");
  Serial.println(data.pitch);
}
/*
// callback when data is sent
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status)
{
  char macStr[18];
  formatMacAddress(macAddr, macStr, 18);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}*/


// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == 0) {
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

//void broadcast()
//{
//  // this will broadcast a message to everyone in range
//  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//  esp_now_peer_info_t peerInfo = {};
//  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
//  if (!esp_now_is_peer_exist(broadcastAddress))
//  {
//    esp_now_add_peer(&peerInfo);
//  }
//  //esp_err_t result = esp_now_send(broadcastAddress, (const uint8_t *)message.c_str(), message.length());
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
//  // and this will send a message to a specific device
//  /*uint8_t peerAddress[] = {0x3C, 0x71, 0xBF, 0x47, 0xA5, 0xC0};
//  esp_now_peer_info_t peerInfo = {};
//  memcpy(&peerInfo.peer_addr, peerAddress, 6);
//  if (!esp_now_is_peer_exist(peerAddress))
//  {
//    esp_now_add_peer(&peerInfo);
//  }
//  esp_err_t result = esp_now_send(peerAddress, (const uint8_t *)message.c_str(), message.length());*/
//  if (result == ESP_OK)
//  {
//    Serial.println("Broadcast message success");
//  }
//  else if (result == ESP_ERR_ESPNOW_NOT_INIT)
//  {
//    Serial.println("ESPNOW not Init.");
//  }
//  else if (result == ESP_ERR_ESPNOW_ARG)
//  {
//    Serial.println("Invalid Argument");
//  }
//  else if (result == ESP_ERR_ESPNOW_INTERNAL)
//  {
//    Serial.println("Internal Error");
//  }
//  else if (result == ESP_ERR_ESPNOW_NO_MEM)
//  {
//    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
//  }
//  else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
//  {
//    Serial.println("Peer not found.");
//  }
//  else
//  {
//    Serial.println("Unknown error");
//  }
//}
