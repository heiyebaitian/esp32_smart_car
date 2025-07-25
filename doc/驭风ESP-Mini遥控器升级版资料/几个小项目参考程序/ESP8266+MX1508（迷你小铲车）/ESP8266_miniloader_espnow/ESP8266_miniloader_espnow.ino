#include "ws2812_FASTLED.h"
#include "mx1508.h"
#include "espnow.h"
#include "miniloader.h"
/**********************************************/
#include "OTA.h"  //OTA相关
#include <Mywifi.h>  //注释掉此行，替换为下边两行，填入自己的WIFI帐号和密码
//const char* mySSID = "********";
//const char* myPASSWORD = "********";
/**********************************************/

unsigned long previousMillis1 = 0;   
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
const long LEDdelay=40;               //(milliseconds)LED变化时间间隔
int fade = 1;  //呼吸效果

void setup(){   
  Serial.begin(115200);       // 启动硬件串口（用于输出调试信息和接收控制指令）
  delay(100);
/**********************************************/    
  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  Serial.println("ESPNow");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();

  // 设置接收数据回调函数
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
//  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  resetData();
/**********************************************/ 
  
  FastLED_ini();          //FastLED初始化
  dcMotorIni(0,0);     //分别对应电机的默认转动方向,改变0/1变换方向
  mx1508_ini();  //电机驱动初始化
  miniloader_ini();//铲斗等初始化

/**********************************************/
  setupOTA("MyOTA", mySSID, myPASSWORD);  //开启OTA
/**********************************************/
  
}

void loop(){
/**********************************************/
ArduinoOTA.handle();      //执行OTA
/**********************************************/
  
  unsigned long currentMillis = millis();    
  if (currentMillis - previousMillis1 >= LEDdelay) { //RGB灯变化   
      previousMillis1 = currentMillis;
      runLED();
      
      rgb_bright += fade*2;   //呼吸灯效果
      if (rgb_bright > max_bright || rgb_bright < min_bright) fade = -fade;
      FastLED.setBrightness(rgb_bright);     
  }
  if (currentMillis - previousMillis2 >= 30000) {  //每30秒自动变换RGB模式
    previousMillis2 = currentMillis;
    lightState++;
    if (lightState > 3)  lightState = 1;
  }
  if (currentMillis - previousMillis3 >= 1000) {  //每1秒闪烁
    previousMillis3 = currentMillis;
    LedEmergencyOff = (LedEmergencyOff == LOW) ? HIGH : LOW;
    digitalWrite(LightEmergencyPin, LedEmergencyOff);    
    LedFront = (LedFront == LOW) ? HIGH : LOW;
    digitalWrite(LightFrontPin, LedFront);    
  }


    lpad_x=data.j1PotX;
    lpad_y=data.j1PotY;
//    rpad_x=( (data.buttonR3==0) ? data.roll : data.j2PotX );
//    rpad_y=( (data.buttonR3==0) ? data.pitch : data.j2PotY );

    if ( !data.buttonLB ){     
      if (LoaderBucketAngle<Bucketdir) LoaderBucketAngle++; else LoaderBucketAngle--;
    }
    if ( !data.buttonRB ){     
      if (LoaderArmAngle<Armdir) LoaderArmAngle++; else LoaderArmAngle--;
    }
    if ( data.j2PotY<127-limit && LoaderArmAngle>ArmMin ) LoaderArmAngle--;
    else if ( data.j2PotY>127+limit && LoaderArmAngle<ArmMax ) LoaderArmAngle++;
    if ( data.j2PotX<127-limit && LoaderBucketAngle<ArmMax ) LoaderBucketAngle++;
    else if ( data.j2PotX>127+limit && LoaderBucketAngle>ArmMin ) LoaderBucketAngle--;
    
    LoaderArm.write(LoaderArmAngle);
    LoaderBucket.write(LoaderBucketAngle);    
  
  Mapdata();   //处理数据
  run_motors();  //运行电机
}
