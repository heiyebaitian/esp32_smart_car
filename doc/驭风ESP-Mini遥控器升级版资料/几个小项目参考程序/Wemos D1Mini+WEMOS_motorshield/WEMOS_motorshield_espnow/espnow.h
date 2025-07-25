#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
#else
#include <ESP8266WiFi.h>
#include <espnow.h>
#endif


#define CHANNEL 1

// config AP SSID
void configDeviceAP() {
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "12345678";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}


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

#if defined(ESP32)
  // 数据接收回调函数
  void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {    
    //void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {  
    memcpy(&data, incomingData, sizeof(data));
    Serial.print("j1PotX: ");
    Serial.print(data.j1PotX);
    Serial.print("; j1PotY: ");
    Serial.print(data.j1PotY); 
    Serial.print("; j2PotX: ");
    Serial.print(data.j2PotX);
    Serial.print("; j2PotY: ");
    Serial.print(data.j2PotY);
    Serial.print(";     buttonR1: ");
    Serial.print(data.buttonR1);
    Serial.print("; buttonR2: ");
    Serial.print(data.buttonR2);
    Serial.print("; buttonLB: ");
    Serial.print(data.buttonLB);
    Serial.print("; buttonRB: ");
    Serial.print(data.buttonRB);
    Serial.print(";     Data_size: ");
    Serial.println(sizeof(data));
  //  Serial.print(";     roll: ");
  //  Serial.print(data.roll);
  //  Serial.print("; pitch: ");
  //  Serial.println(data.pitch);
  }
#else
  // 数据接收回调函数
//void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {    
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
    Serial.print(";     buttonR1: ");
    Serial.print(data.buttonR1);
    Serial.print("; buttonR2: ");
    Serial.print(data.buttonR2);
    Serial.print("; buttonLB: ");
    Serial.print(data.buttonLB);
    Serial.print("; buttonRB: ");
    Serial.print(data.buttonRB);
    Serial.print(";     Data_size: ");
    Serial.println(sizeof(data));
  //  Serial.print(";     roll: ");
  //  Serial.print(data.roll);
  //  Serial.print("; pitch: ");
  //  Serial.println(data.pitch);
  }
#endif

/*
void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength)
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

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
