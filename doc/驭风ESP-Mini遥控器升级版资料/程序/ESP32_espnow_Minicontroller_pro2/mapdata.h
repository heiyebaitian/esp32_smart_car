#ifndef MAPDATA_H
#define MAPDATA_H

#include <EEPROM.h>
#define EEPROM_SIZE 64

// TRIGGER BUTTONS
#define LB 19
#define RB 25

// LEFT-JOYSTICK
#define LH 34
#define LV 35
#define LS 5

// RIGHT-JOYSTICK
#define RH 36
#define RV 32
#define RS 4

#define R1 2
#define R2 13

// BATTERY VOLTAGE
#define ADC 39  //电池ADC

// RGB
//#define DATA_PIN 17

//OTHERS
#define BK 23  //拨动开关

int buttons[7] = {R1,R2,LS,RS,LB,RB,BK};


const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 2;    // Delay in milliseconds between pot samples

int LX_read = 0;
int LY_read = 0;
int RX_read = 0;
int RY_read = 0;

byte LX_zero = 127;
byte LY_zero = 127;
byte RX_zero = 127;
byte RY_zero = 127;

int LX_to_send = 0;
int LY_to_send = 0;
int RX_to_send = 0;
int RY_to_send = 0;

bool LY_inverted = false; 
bool LX_inverted = false; 
bool RY_inverted = false; 
bool RX_inverted = false; 

int counter = 0;
int invert_counter = 0;

float voltage = 3.00;
int percentage = 0;

void pinmode_pullup(){
  for (int i = 0; i < 7; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

  
/**************************************************/
int map_normal(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 127);
  else
    val = map(val, middle, upper, 127, 255);
  return ( reverse ? 255 - val : val );
}

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
int map_exponential(int val, bool reverse)
{
  val = constrain(val, 0, 1023);
  float cube = ((pow((val - 512),3)/520200) + 258.012) / 2; 
  return ( reverse ? 255 - cube : cube );
}
/**************************************************/

void read_joydata(){
  int potValues[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues[i] = analogRead(LH);
      delay(delayBetweenSamples);
    }
    int potValue = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue += potValues[i];
    }
    potValue = potValue / numberOfPotSamples;
    LX_read = map(potValue, 0, 4095, 255, 0);


    int potValues2[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues2[i] = analogRead(LV);
      delay(delayBetweenSamples);
    }
    int potValue2 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue2 += potValues2[i];
    }
    potValue2 = potValue2 / numberOfPotSamples;
    LY_read = map(potValue2, 0, 4095, 255, 0);


    int potValues3[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues3[i] = analogRead(RH);
      delay(delayBetweenSamples);
    }
    int potValue3 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue3 += potValues3[i];
    }
    potValue3 = potValue3 / numberOfPotSamples;
    RX_read = map(potValue3, 0, 4095, 255, 0);


    int potValues4[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues4[i] = analogRead(RV);
      delay(delayBetweenSamples);
    }
    int potValue4 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue4 += potValues4[i];
    }
    potValue4 = potValue4 / numberOfPotSamples;
    RY_read = map(potValue4, 0, 4095, 255, 0);
}


void zero_test(){   //摇杆原点纠偏程序
  Serial.println(" joy_zero_testing... ");
  read_joydata();
  delay (300);
  
  LX_to_send = map_normal(LX_read, 0, 127, 255, 0);    
  LY_to_send = map_normal(LY_read, 0, 127, 255, 0);     
  RX_to_send = map_normal(RX_read, 0, 127, 255, 0);    
  RY_to_send = map_normal(RY_read, 0, 127, 255, 0);
       
  LX_to_send = constrain(LX_to_send,0,255);
  LY_to_send = constrain(LY_to_send,0,255);
  RX_to_send = constrain(RX_to_send,0,255);  
  RY_to_send = constrain(RY_to_send,0,255);

  LX_zero = LX_to_send;
  LY_zero = LY_to_send;
  RX_zero = RX_to_send;
  RY_zero = RY_to_send;

  Serial.println(" Writing in EEPROM... ");
  delay (300);
  
  if( EEPROM.read(1) != LX_zero) EEPROM.write(1, LX_zero);
  if( EEPROM.read(2) != LY_zero) EEPROM.write(2, LY_zero);
  if( EEPROM.read(3) != RX_zero) EEPROM.write(3, RX_zero);
  if( EEPROM.read(4) != RY_zero) EEPROM.write(4, RY_zero);
  EEPROM.commit();

  Serial.println(" Done... ");
  Serial.print(" LX_zero: ");Serial.print(EEPROM.read(1));
  Serial.print(" LY_zero: ");Serial.print(EEPROM.read(2));
  Serial.print(" RX_zero: ");Serial.print(EEPROM.read(3));
  Serial.print(" RY_zero: ");Serial.println(EEPROM.read(4));  
}
  
void eeprom_ini(){ 
  EEPROM.begin(EEPROM_SIZE);
   
  if( EEPROM.read(0) != 55){     //判断是否首次使用（新的所有地址貌似是255的值）
    
      zero_test();      
        
      EEPROM.write(0, 55);
      EEPROM.commit();
    }

  LX_zero = EEPROM.read(1);
  LY_zero = EEPROM.read(2);
  RX_zero = EEPROM.read(3);
  RY_zero = EEPROM.read(4);
  
}


void batteryLevel() {
  int sensorValue = analogRead(ADC);
  voltage = sensorValue/4095.0 * 7.27;   //7.23乘数具体数值用万用表实测纠偏    
  percentage = (voltage-3.2) * 100;
//  percentage = 2808.3808 * pow(voltage,4) - 43560.9157 * pow(voltage,3) + 252848.5888 * pow(voltage,2)
//                   - 650767.4615 * voltage + 626532.5703; //这段计算公式参考https://www.youtube.com/watch?v=qKUrXwkr3cc
if (voltage>4.19) percentage = 100;     //限制到0-100范围
  if (voltage<3.20) percentage = 0;
  if (voltage>3.00){
//    Serial.print("Lipo: ");Serial.print(voltage);Serial.print("V || ");
//    Serial.print(percentage);Serial.println("%");
  }
  
//  bleGamepad.setBatteryLevel(percentage);
//  if (percentage < 60) {
//    Serial.println("LOW battery");
//    for (uint8_t i = 30; i < 220; i++) {
//      for (int j = 0; j < 6; j++) {
//        leds[j] = CHSV( HUE_RED, 255, i);
//      }
//      FastLED.show();
//      delay(15);
//    }
//    for (uint8_t i = 220; i > 30; i--) {
//      for (int j = 0; j < 6; j++) {
//        leds[j] = CHSV( HUE_RED, 255, i);
//      }
//      FastLED.show();
//      delay(15);
//    }
//  }
}

//摇杆坐标转化为离原点距离
int joy_distance(float coord_x,float coord_y){
  int joy_dis=0;
  joy_dis=sqrt(sq(coord_x)+sq(coord_y));
  if (joy_dis < 5) joy_dis=0;      //消除一些摇杆误差
  return joy_dis;
}
//摇杆坐标转化为0-360的角度
int coordinates_to_angle(float coord_x,float coord_y){  
  int joy_angle=0;
  if (coord_x>0&&coord_y>0){        //第一象限
    joy_angle=asin(coord_y/sqrt(sq(coord_x)+sq(coord_y)))*180/3.14159;
  }
  if (coord_x<0&&coord_y>0){        //第二象限
    joy_angle=180-asin(coord_y/sqrt(sq(coord_x)+sq(coord_y)))*180/3.14159;
  }
  if (coord_x<0&&coord_y<0){        //第三象限
    joy_angle=180-asin(coord_y/sqrt(sq(coord_x)+sq(coord_y)))*180/3.14159;  //此时Y坐标为负数
  }
  if (coord_x>0&&coord_y<0){        //第四象限
    joy_angle=360+asin(coord_y/sqrt(sq(coord_x)+sq(coord_y)))*180/3.14159;  //此时Y坐标为负数
  }
  return joy_angle;
}
//摇杆角度转化为左摇杆LED序号
int angle_to_lednum1(int angle){ 
  int led_num=0;
  if ((0<=angle&&angle<23)||(338<=angle&&angle<360)) led_num=0;
  if (23<=angle&&angle<68) led_num=7;
  if (68<=angle&&angle<113) led_num=6;
  if (113<=angle&&angle<158) led_num=5;
  if (158<=angle&&angle<203) led_num=4;
  if (203<=angle&&angle<248) led_num=3;
  if (248<=angle&&angle<293) led_num=2;
  if (293<=angle&&angle<338) led_num=1;
  return led_num;
}
//摇杆角度转化为右摇杆LED序号
int angle_to_lednum2(int angle){ 
  int led_num=0;
  if ((0<=angle&&angle<23)||(338<=angle&&angle<360)) led_num=4;
  if (23<=angle&&angle<68) led_num=3;
  if (68<=angle&&angle<113) led_num=2;
  if (113<=angle&&angle<158) led_num=1;
  if (158<=angle&&angle<203) led_num=0;
  if (203<=angle&&angle<248) led_num=7;
  if (248<=angle&&angle<293) led_num=6;
  if (293<=angle&&angle<338) led_num=5;
  return led_num;
}

#endif 
