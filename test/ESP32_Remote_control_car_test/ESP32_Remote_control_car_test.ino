#include "MK_encoder_motor_driver.h"
#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"

#define CHANNEL 1



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
/**********************************************/    
  // 初始化 ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  delay(100);
  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", CHANNEL);
  // Init ESPNow with a fallback logic
  InitESPNow();

  // 设置接收数据回调函数
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
//  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  resetData();
/**********************************************/ 
  MK_Init();
}

void loop() {
  // put your main code here, to run repeatedly:

}
