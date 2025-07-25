#include "ws2812_FASTLED.h"
#include "tb6612.h"
#include "claw_Servo.h"
#include "espnow.h"

#include <BluetoothSerial.h>
BluetoothSerial ESP32_BT;
char BluetoothData;         //蓝牙数据

boolean acc_on=false;    //陀螺仪开关
int roll,pitch;   //roll and pitch sent from Android device陀螺仪数据

unsigned long previousMillis1 = 0;   
unsigned long previousMillis2 = 0;
const long LEDdelay=40;               //(milliseconds)LED变化时间间隔
int fade = 1;  //呼吸效果

void setup(){   
  Serial.begin(115200);       // 启动硬件串口（用于输出调试信息和接收控制指令）
  delay(100);

  
/**********************************************/    
  // 初始化 ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  delay(100);
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
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
  dcMotorIni(1,0);     //分别对应电机的默认转动方向,改变0/1变换方向
  tb6612_ini();  //电机驱动初始化
  servo_ini();        //舵机初始化 

  ESP32_BT.begin("ESP32_car"); //启用蓝牙
  
  analogWriteResolution(2, 13);  //板载LED
  
}

void loop(){
   unsigned long currentMillis = millis();    
  if (currentMillis - previousMillis1 >= LEDdelay) { //RGB灯变化   
      previousMillis1 = currentMillis;
      runLED();
      
      posDegrees += fade;  //模拟呼吸效果
      if (posDegrees > clawmax || posDegrees < clawmin) fade = -fade;

      run_claw();     //前爪动作，两种模式
      
      rgb_bright-= fade*1;   //保持和前爪一样频率的呼吸，注意控制合适的取值范围，不然可能亮度突变
      rgb_bright = constrain (rgb_bright,min_bright,max_bright); //限制取值范围
      FastLED.setBrightness(rgb_bright);
      
      analogWrite(2,rgb_bright);  //板载LED
  }
  if (currentMillis - previousMillis2 >= 30000) {  //每30秒自动变换RGB模式
    previousMillis2 = currentMillis;
    lightState++;
    if (lightState > 3)  lightState = 1;
  }

//连上蓝牙时接收蓝牙数据
   if(ESP32_BT.connected()){
     if(ESP32_BT.available()){   
        BluetoothData=ESP32_BT.read();
        
        switch(BluetoothData){
          
        //***Control Pad on Left - Sends 'X__,Y__*' every 100ms
        //读取左摇杆发送来的数据'X__,Y__*'
        case'X':              //左摇杆
          lpad_x=ESP32_BT.parseInt();
          while (BluetoothData!='*'){
            if(ESP32_BT.available()){
              BluetoothData=ESP32_BT.read();   //Get next character from bluetooth
              if(BluetoothData=='Y')lpad_y=ESP32_BT.parseInt();
            }
          }
          break;    
        //***Control Pad on Right - Sends 'RX__,Y__*' every 100ms
        //读取右摇杆发送来的数据'RX__,Y__*'
        case'R':              //右摇杆
          while (BluetoothData!='*'){
            if(ESP32_BT.available()){
              BluetoothData=ESP32_BT.read();   //Get next character from bluetooth
              if(BluetoothData=='X')rpad_x=ESP32_BT.parseInt();
              if(BluetoothData=='Y')rpad_y=ESP32_BT.parseInt();
            }
          }
          break;
    
          //***Accelerometer - sends 'Aroll,pitch*'every 150 ms 
          //读取陀螺仪的数据'A__,__*'   
        case 'A':
          roll=ESP32_BT.parseInt();
          roll=constrain(roll,-90,90);     //限制范围
          while (BluetoothData!='*'){
            if (ESP32_BT.available()){
              BluetoothData=ESP32_BT.read();   //Get next character from bluetooth
              if(BluetoothData==',')pitch=ESP32_BT.parseInt();
              pitch=constrain(pitch,-50,50);  //限制范围
            }
          }      
          break;
        case'T':              //陀螺仪开
          acc_on=true;
          Serial.println("acc_on");
          break;
        case't':              //陀螺仪关
          acc_on=false;
          rpad_x=127; rpad_y=127;  //复位一下
          Serial.println("acc_off");
          break;

          //读取控制舵机值的数据'S__*'
        case'S':              //前爪舵机
          clawpos= map(ESP32_BT.parseInt(),0,100,clawmin,clawmax); //读取前爪舵机角度
          break;
        case'C':              //前爪模式切换
          clawmode = 0;
          break;
        case'c':              //前爪模式切换
          clawmode = 1;
          break;                    
        }                
      }
 //未连上蓝牙时接收ESPNOW遥控器数据     
   } else {
    lpad_x=data.j1PotX;
    lpad_y=data.j1PotY;
    rpad_x=( (data.buttonR3==0) ? data.roll : data.j2PotX );
    rpad_y=( (data.buttonR3==0) ? data.pitch : data.j2PotY );
//    rpad_x=data.j2PotX;
//    rpad_y=data.j2PotY;
    
    if (!data.tSwitch1){     // 前爪模式切换
      clawmode = 1;
      if (!data.j1Button) clawpos = clawmin;
      if (!data.j2Button) clawpos = clawmax;
      if (!data.buttonLB) {
        if (clawpos > clawmin) clawpos -=1;
        delay(10);   //加延时才能正常递增减
      }
      if (!data.buttonRB) {
        if (clawpos < clawmax) clawpos +=1; 
        delay(10);   //加延时才能正常递增减
      }
    } else {
      clawmode = 0;       
    }
   }
    

  if (acc_on){
    rpad_x = map(roll, -90, 90, 0, 255); 
    rpad_y = map(pitch, -50, 50, 0, 255);    
  }
  
  Mapdata();   //处理数据
  run_motors();  //运行电机
}
