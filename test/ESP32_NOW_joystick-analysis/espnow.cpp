#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"

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


double calculate_angle(double x, double y) {
    // 计算弧度
    double angle_in_radians = atan2(y, x);
    // 转换为度数
    double angle_in_degrees = angle_in_radians * (180.0 / M_PI);
    return angle_in_degrees;
}

double calculate_length(double x, double y) {
    // 计算向量的长度
    return sqrt(x * x + y * y);
}

double calculate_max_length(double angle_degrees, int max_coord) {
    // 将角度转换为弧度
    double angle_radians = angle_degrees * (M_PI / 180.0);

    // 计算在给定角度下的x和y分量的最大可能值
    double x_component = fabs(cos(angle_radians));
    double y_component = fabs(sin(angle_radians));

    // 确保x和y分量不会超出范围，同时最大化它们的值
    double max_factor = fmin(1.0 / x_component, 1.0 / y_component);
    
    x_component *= max_coord * max_factor;
    y_component *= max_coord * max_factor;

    // 使用勾股定理计算最大长度
    double max_length = sqrt(x_component * x_component + y_component * y_component);

    return max_length;
}

double groupA_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupA_percentage = MK_length * cos(MK_angle * (M_PI / 180.0));
    return MK_groupA_percentage;
}

double groupB_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupB_percentage = MK_length * sin(MK_angle * (M_PI / 180.0));
    return MK_groupB_percentage;
}


// 数据接收回调函数
void OnDataRecv(const esp_now_recv_info_t* mac, const uint8_t *incomingData, int len) {    
    //void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {  
    memcpy(&espnow_data, incomingData, sizeof(espnow_data));

    double angle = calculate_angle(127 - espnow_data.j1PotX, espnow_data.j1PotY - 127) + 180;
    double length = calculate_length(espnow_data.j1PotX - 127, espnow_data.j1PotY - 127);
    double percentage = length / calculate_max_length(calculate_angle(127 - espnow_data.j1PotX, espnow_data.j1PotY - 127) + 180 , 127);
    Serial.printf("\n角度为: %.2f 度\n", angle);
    Serial.printf("向量的长度为: %.2f\n", length);
    Serial.printf("杆量: %d\n", int(percentage * 100));
    Serial.printf("A组分量: %.2lf\n", groupA_motor_percentage(angle, percentage));
    Serial.printf("B组分量: %.2lf\n", groupB_motor_percentage(angle, percentage));
    // Serial.print("j1PotX: ");
    // Serial.print(espnow_data.j1PotX);
    // Serial.print("; j1PotY: ");
    // Serial.print(espnow_data.j1PotY); 
    // Serial.print("; j2PotX: ");
    // Serial.print(espnow_data.j2PotX);
    // Serial.print("; j2PotY: ");
    // Serial.print(espnow_data.j2PotY);
    // Serial.print(";     buttonR1: ");
    // Serial.print(espnow_data.buttonR1);
    // Serial.print("; buttonR2: ");
    // Serial.print(espnow_data.buttonR2);
    // Serial.print("; buttonLB: ");
    // Serial.print(espnow_data.buttonLB);
    // Serial.print("; buttonRB: ");
    // Serial.print(espnow_data.buttonRB);
    // Serial.print(";     Data_size: ");
    // Serial.println(sizeof(espnow_data));
  //  Serial.print(";     roll: ");
  //  Serial.print(espnow_data.roll);
  //  Serial.print("; pitch: ");
  //  Serial.println(espnow_data.pitch);

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
