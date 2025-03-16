#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"
#include "joystick_analysis.h"
#include "MK_encoder_motor_driver.h"

#define CHANNEL 1



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
Data_Package espnow_data; //Create a variable with the above structure


void resetData() {  //数据重置
  // Reset the values when there is no radio connection - Set initial default values
  espnow_data.j1PotX = 127;
  espnow_data.j1PotY = 127;
  espnow_data.j2PotX = 127;
  espnow_data.j2PotY = 127;
  espnow_data.buttonR1 = 1;
  espnow_data.buttonR2 = 1;
  espnow_data.j1Button = 1;
  espnow_data.j2Button = 1;
  espnow_data.buttonLB = 1;
  espnow_data.buttonRB = 1;
  espnow_data.tSwitch1 = 1;
  espnow_data.roll=127;
  espnow_data.pitch=127;
  espnow_data.buttonR3 = 1;
}





// 数据接收回调函数
void OnDataRecv(const esp_now_recv_info_t* mac, const uint8_t *incomingData, int len) {    
    //void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {  
    memcpy(&espnow_data, incomingData, sizeof(espnow_data));
    const int16_t speed_rpm = 20;
    double angle = calculate_angle(espnow_data.j1PotX, espnow_data.j1PotY, 127);
    double length = calculate_length(espnow_data.j1PotX, espnow_data.j1PotY, 127);
    double percentage = calculate_joystick_percentage(espnow_data.j1PotX, espnow_data.j1PotY, 127);
    Serial.printf("\n角度为: %.2f 度\n", angle);
    Serial.printf("向量的长度为: %.2f\n", length);
    Serial.printf("杆量: %.2lf\n", percentage);
    Serial.printf("A组分量: %.2lf\n", groupA_motor_percentage(angle, percentage));
    Serial.printf("B组分量: %.2lf\n", groupB_motor_percentage(angle, percentage));

    if(percentage <= 0.05){
      // 防误触
      MK_Stop();
    }
    else
    {
      MK_RunSpeed(groupA_motor_percentage(angle, percentage), groupB_motor_percentage(angle, percentage));
    }
}



// Init ESP Now with fallback
void InitESPNow() {
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
