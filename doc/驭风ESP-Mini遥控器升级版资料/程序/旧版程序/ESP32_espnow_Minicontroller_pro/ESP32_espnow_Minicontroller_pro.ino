#include "mapdata.h"
#include "espnow.h"
#include "ws2812_FASTLED.h"
#include "wemos_oled.h"     //注意可能需要改屏幕的地址0x3C或0x3D等，若屏幕不亮可以试试用wire库中的示例程序扫描地址

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
const long LEDdelay=40;               //(milliseconds)LED变化时间间隔
int fade = 1;  //呼吸效果


  //摇杆坐标转化为离原点距离
  int J1_dis = 0;
  int J2_dis = 0;
  //摇杆坐标转化为0-360的角度
  int J1_angle = 0;  
  int J2_angle = 0;

  //摇杆角度转化为左右摇杆LED序号
  int J1_led_num = 0;
  int J2_led_num = 0;
  //摇杆离原点距离转化为左右摇杆LED亮度
  int J1_led_bright = 0;
  int J2_led_bright = 0; 

//**********************Setup*************************
void setup() {    
  pinmode_pullup();
  FastLED_ini();          //FastLED初始化  
  Serial.begin(115200); 
  delay(100);
  eeprom_ini();   //EEPROM初始化
  Serial.print(" LX_zero: ");Serial.print(EEPROM.read(1));
  Serial.print(" LY_zero: ");Serial.print(EEPROM.read(2));
  Serial.print(" RX_zero: ");Serial.print(EEPROM.read(3));
  Serial.print(" RY_zero: ");Serial.println(EEPROM.read(4)); 
  
  // 初始化 ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  delay(100);
  WiFi.mode(WIFI_STA);
  Serial.println("InitESPNow");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();

  // 设置发送数据回调函数
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // 设置接收数据回调函数
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
//  esp_now_register_recv_cb(OnDataRecv);

  //配对连接pair with another ESP-NOW device
  pair_device();
  
  resetData();
  Serial.print("Data Size: "); Serial.print(sizeof(data));Serial.println(" Bytes");
  Serial.print(" LX: ");Serial.print(data.j1PotX);
  Serial.print(" LY: ");Serial.print(data.j1PotY);
  Serial.print(" RX: ");Serial.print(data.j2PotX);
  Serial.print(" RY: ");Serial.print(data.j2PotY);
  Serial.print(" // R1: ");Serial.print(data.buttonR1);
  Serial.print(" R2: ");Serial.print(data.buttonR2);
  Serial.print(" J1: ");Serial.print(data.j1Button);
  Serial.print(" J2: ");Serial.print(data.j2Button);
  Serial.print(" LB: ");Serial.print(data.buttonLB);
  Serial.print(" RB: ");Serial.print(data.buttonRB);
  Serial.print(" T1: ");Serial.println(data.tSwitch1);

  run_OLED_on_Core_0();//在CORE0上运行OLED相关程序避免延迟影响
     
}

void loop() {
   
  read_joydata();  
  
/*********************************************************************************************/

  LX_to_send = map_normal(LX_read, 0, LX_zero, 255, LX_inverted);    
  LY_to_send = map_normal(LY_read, 0, LY_zero, 255, LY_inverted);     
  RX_to_send = map_normal(RX_read, 0, RX_zero, 255, RX_inverted);    
  RY_to_send = map_normal(RY_read, 0, RY_zero, 255, RY_inverted);    
  
/*********************************************************************************************/  
     
  LX_to_send = constrain(LX_to_send,0,255);
  LY_to_send = constrain(LY_to_send,0,255);
  RX_to_send = constrain(RX_to_send,0,255);  
  RY_to_send = constrain(RY_to_send,0,255);
  
  data.j1PotX = LX_to_send;
  data.j1PotY = LY_to_send;
  data.j2PotX = RX_to_send;  
  data.j2PotY = RY_to_send;   
  data.buttonR1 = digitalRead(R1);
  data.buttonR2 = digitalRead(R2);
  data.j1Button = digitalRead(LS);
  data.j2Button = digitalRead(RS);
  data.buttonLB = digitalRead(LB);
  data.buttonRB = digitalRead(RB);
  data.tSwitch1 = digitalRead(BK);

  coordinates_to_Leds();  //坐标转换为LED控制

/************************************************/
//同时按下四键及拨动开关3秒后进行摇杆原点纠偏，此时不要碰摇杆 
  if (!data.buttonR1&&!data.buttonR2&&!data.buttonLB&&!data.buttonRB&&!data.tSwitch1){
    delay (3000);
    if (!data.buttonR1&&!data.buttonR2&&!data.buttonLB&&!data.buttonRB&&!data.tSwitch1){
      zero_test();
    }
  }

//同时按下四键后变换背光RGB模式 
  if (!data.j1Button&&!data.j2Button&&!data.buttonLB&&!data.buttonRB){
    delay (300);
    if (!data.j1Button&&!data.j2Button&&!data.buttonLB&&!data.buttonRB){
      lightState += 1;
      if (lightState>3) lightState = 0;
    }
  } 
/************************************************/ 

/*
  Serial.print(EEPROM.read(0));
  Serial.print("   ");Serial.print(EEPROM.read(1));
  Serial.print("   ");Serial.print(EEPROM.read(2));
  Serial.print("   ");Serial.print(EEPROM.read(3));
  Serial.print("   ");Serial.print(EEPROM.read(4));
/*  
  Serial.print(" LX: ");Serial.print(data.j1PotX);
  Serial.print(" LY: ");Serial.print(data.j1PotY);
  Serial.print(" RX: ");Serial.print(data.j2PotX);
  Serial.print(" RY: ");Serial.print(data.j2PotY);
  Serial.print(" // R1: ");Serial.print(data.buttonR1);
  Serial.print(" R2: ");Serial.print(data.buttonR2);
  Serial.print(" J1: ");Serial.print(data.j1Button);
  Serial.print(" J2: ");Serial.print(data.j2Button);
  Serial.print(" LB: ");Serial.print(data.buttonLB);
  Serial.print(" RB: ");Serial.print(data.buttonRB);
  Serial.print(" T1: ");Serial.println(data.tSwitch1);
 /* 
  Serial.print(" J1_dis: ");Serial.print(J1_dis);Serial.print(" J2_dis: ");Serial.print(J2_dis);
  Serial.print("  //  J1_angle: ");Serial.print(J1_angle);Serial.print(" J2_angle: ");Serial.println(J2_angle);
  Serial.print(" J1_led_num: ");Serial.print(J1_led_num);Serial.print(" J2_led_num ");Serial.print(J2_led_num);
  Serial.print("  //  J1_led_bright: ");Serial.print(J1_led_bright);Serial.print(" J2_led_bright: ");Serial.println(J2_led_bright);
 */ 
  
  
//*****************************************************  
  unsigned long currentMillis = millis();    
  if (currentMillis - previousMillis1 >= LEDdelay) {    
    previousMillis1 = currentMillis;
        
    runLED();

    if (J1_dis<10&&J2_dis<10){
      if (voltage>3.50){    //电池电压高于3.5V时星闪模式
        joy1_led_blink();
        joy2_led_blink(); 
      } else {              //否则省电关灯
        fill_solid (leds1,NUM_LEDS,CRGB::Black);     //关灯
        fill_solid (leds2,NUM_LEDS,CRGB::Black);     //关灯
        FastLED.show();  
      }
    } else {
      joy1_map_led();
      joy2_map_led();
    }    
    
  }
    
  if (currentMillis - previousMillis2 >= 100) {//每100ms发送一次
    previousMillis2 = currentMillis;
   // ESP-NOW发送数据
    sendData();
  } 
  
  if (currentMillis - previousMillis3 >= 5000) {//每5秒更新一次电量
    previousMillis3 = currentMillis;
    batteryLevel();         //读电池电量
  } 
  
}

void coordinates_to_Leds(){
  //摇杆坐标转化为离原点距离
  J1_dis = joy_distance((data.j1PotX-127),(data.j1PotY-127));
  J2_dis = joy_distance((data.j2PotX-127),(data.j2PotY-127));
  //摇杆坐标转化为0-360的角度
  J1_angle = coordinates_to_angle(((J1_dis<15) ? 0 :(data.j1PotX-127)),((J1_dis<15) ? 0 :(data.j1PotY-127)));  
  J2_angle = coordinates_to_angle(((J2_dis<15) ? 0 :(data.j2PotX-127)),((J2_dis<15) ? 0 :(data.j2PotY-127)));

  //摇杆角度转化为左右摇杆LED序号
  J1_led_num = angle_to_lednum1(J1_angle);
  J2_led_num = angle_to_lednum2(J2_angle);
  //摇杆离原点距离转化为左右摇杆LED亮度
  J1_led_bright = (J1_dis<15)? 0 : (map (J1_dis,0,127,0,max_bright));
  J2_led_bright = (J2_dis<15)? 0 : (map (J2_dis,0,127,0,max_bright)); 
} 

void joy1_map_led(){
  fill_solid (leds1,NUM_LEDS,CRGB::Black);     //关灯
  leds1[J1_led_num] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J1_led_bright, LINEARBLEND); 
  leds1[(J1_led_num+1)>7 ? 0 : (J1_led_num+1)] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J1_led_bright/8, LINEARBLEND); 
  leds1[(J1_led_num-1)<0 ? 7 : (J1_led_num-1)] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J1_led_bright/8, LINEARBLEND);
  FastLED.show();                    
}
void joy2_map_led(){
  fill_solid (leds2,NUM_LEDS,CRGB::Black);     //关灯
  leds2[J2_led_num] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J2_led_bright, LINEARBLEND); 
  leds2[(J2_led_num+1)>7 ? 0 : (J2_led_num+1)] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J2_led_bright/8, LINEARBLEND); 
  leds2[(J2_led_num-1)<0 ? 7 : (J2_led_num-1)] = ColorFromPalette( RainbowColors_p, beatsin8(1, 0, 255), J2_led_bright/8, LINEARBLEND); 
  FastLED.show();                   
}
