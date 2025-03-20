#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"
#include "joystick_analysis.h"
#include "MK_encoder_motor_driver.h"
#include "CAS.h"

#define DEBUG_MODE 0

bool buttonR1_last_state = 0; // 右按键上一次状态

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
    double L_angle = handle_joystick_adsorption( calculate_angle(espnow_data.j1PotX, espnow_data.j1PotY, 127) );
    double L_length = calculate_length(espnow_data.j1PotX, espnow_data.j1PotY, 127);
    double L_percentage = handle_joystick_dead_zone( calculate_joystick_percentage(espnow_data.j1PotX, espnow_data.j1PotY, 127) );
    double L_Apercentage = groupA_motor_percentage(L_angle, L_percentage);
    double L_Bpercentage = groupB_motor_percentage(L_angle, L_percentage);


    double R_angle = calculate_angle(espnow_data.j2PotX, espnow_data.j2PotY, 127);
    double R_length = calculate_length(espnow_data.j2PotX, espnow_data.j2PotY, 127);
    double R_percentage = handle_joystick_dead_zone( calculate_joystick_percentage(espnow_data.j2PotX, espnow_data.j2PotY, 127) );

    #if DEBUG_MODE == 1
      Serial.printf("\n[espnow_Task]L角度为: %.2f 度\n", L_angle);
      Serial.printf("[espnow_Task]L向量的长度为: %.2f\n", L_length);
      Serial.printf("[espnow_Task]L杆量: %.2lf\n", L_percentage);
      Serial.printf("[espnow_Task]R角度为: %.2f 度\n", R_angle);
      Serial.printf("[espnow_Task]R向量的长度为: %.2f\n", R_length);
      Serial.printf("[espnow_Task]R杆量: %.2lf\n", R_percentage);
      Serial.printf("[espnow_Task]A组分量: %.2lf\n", L_Apercentage);
      Serial.printf("[espnow_Task]B组分量: %.2lf\n", L_Bpercentage);
    #endif

    /* 左摇杆判断 */
    if(L_percentage == 0 || CAS_flag == CAS_TRIGGERED){
      // 防误触
      MK_L_StopSpeed();
    }
    else
    {
      MK_L_RunSpeed(L_Apercentage, L_Bpercentage);
    }

    /* 右摇杆判断 */
    if(R_percentage == 0 || CAS_flag == CAS_TRIGGERED){
      // 防误触
      MK_R_StopSpeed();
    }
    else if(R_angle > 330 || R_angle <= 30) MK_R_rotateThroughCenter(-R_percentage);
    else if(R_angle > 30 && R_angle <= 90) MK_R_rotateFrontAroundCenter(-R_percentage);
    else if(R_angle > 90 && R_angle <= 150) MK_R_rotateFrontAroundCenter(R_percentage);
    else if(R_angle > 150 && R_angle <= 210) MK_R_rotateThroughCenter(R_percentage);
    else if(R_angle > 210 && R_angle <= 270) MK_R_rotateBackAroundCenter(R_percentage); 
    else if(R_angle > 270 && R_angle <= 330) MK_R_rotateBackAroundCenter(-R_percentage); 

    /* 右肩键解除CAS触发状态至临时解除状态 */
    if(espnow_data.buttonRB == 0 && CAS_flag == CAS_TRIGGERED){
      CAS_flag = CAS_TEMPORARY_RELEASE;
      vTaskResume(CAS_Task_TaskHandle); // 恢复被挂起的CAS任务
    }

    /* 右键开关底盘锁 */
    if(espnow_data.buttonR1 == 0 && buttonR1_last_state == 1){
      if(MK_flag == MK_LOCK){MK_flag = MK_READY;} //  如果锁定则解锁
      else if(MK_flag == MK_READY){MK_flag = MK_LOCK;} //  如果解锁则锁定
    }
    buttonR1_last_state = espnow_data.buttonR1;

}



// Init ESP Now with fallback
void InitESPNow() {
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(channel);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  #if DEBUG_MODE == 1
    Serial.println("[espnow_Task]Wi-Fi parameters:");
    Serial.println("[espnow_Task]  Mode: STA");
    Serial.println("[espnow_Task]  MAC Address: " + WiFi.macAddress());
    Serial.printf("[espnow_Task]  Channel: %d\n", channel);
  #endif

  if (esp_now_init() == 0) {
    Serial.println("[espnow_Task]ESPNow Init Success");
  }
  else {
    Serial.println("[espnow_Task]ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
  esp_now_register_recv_cb(OnDataRecv); // 注册epsnow回调
  resetData();  //  清空数据
}
