#ifndef MAPDATA_H_
#define MAPDATA_H_

#include <EEPROM.h>

// TRIGGER BUTTONS
#define LB 19
#define RB 25
#define LT 13
#define RT 0

// LEFT-JOYSTICK
#define LH 34
#define LV 35
#define LS 1

// RIGHT-JOYSTICK
#define RH 36
#define RV 32
#define RS 3

// D-PAD
#define L1 14
#define L2 26
#define L3 18
#define L4 5

#define R1 2
#define R2 4
#define R3 15
#define R4 12

// BATTERY VOLTAGE
//#define ADC 39  //电池ADC

// RGB
//#define DATA_PIN 17

//OTHERS
#define LP 33  //左电位器
#define RP 39  //右电位器
#define CT 16  //左中
#define BK 23  //左拨
#define ST 27  //右拨

int buttons[17] = {L1,L2,L3,L4,R1,R2,R3,R4,LS,RS,LT,RT,LB,RB,BK,ST,CT};


int angle=0;//屏幕画图用

int LX_fine = 0;
int LY_fine = 0;
int RX_fine = 0;
int RY_fine = 0;

int LX_to_send = 0;
int LY_to_send = 0;
int RX_to_send = 0;
int RY_to_send = 0;

bool LY_inverted = false; 
bool LX_inverted = false; 
bool RY_inverted = false; 
bool RX_inverted = false; 

bool LX_decrease = false;
bool LY_decrease = false;
bool RX_decrease = false;
bool RY_decrease = false;

bool LX_increase = false;
bool LY_increase = false;
bool RX_increase = false;
bool RY_increase = false;

bool mode = true;
bool mode_button_pressed = false;
int counter = 0;
int invert_counter = 0;


void eepromini(){  
  if( EEPROM.read(1) != 55)
    {
      EEPROM.write(2, 127);
      EEPROM.write(3, 127);
      EEPROM.write(4, 127);
      EEPROM.write(5, 127); 
      EEPROM.write(6, 0);
      EEPROM.write(7, 0);
      EEPROM.write(8, 0);
      EEPROM.write(9, 0);    
      EEPROM.write(1, 55);
    }

  LX_fine = EEPROM.read(2);
  LY_fine = EEPROM.read(3);
  RX_fine = EEPROM.read(4);
  RY_fine = EEPROM.read(5);
  LX_inverted = EEPROM.read(6);
  LY_inverted = EEPROM.read(7);
  RX_inverted = EEPROM.read(8);
  RY_inverted = EEPROM.read(9);
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

//void batteryLevel() {
//  int sensorValue = analogRead(ADC);
//  float voltage = sensorValue * (7.00 / 4095.0);
//  Serial.print(voltage);
//  Serial.print("V ||");
//  int percentage = (voltage / 4.2) * 100;
//  Serial.print(percentage);
//  Serial.println("%");
////  bleGamepad.setBatteryLevel(percentage);
////  if (percentage < 60) {
////    Serial.println("LOW battery");
////    for (uint8_t i = 30; i < 220; i++) {
////      for (int j = 0; j < 6; j++) {
////        leds[j] = CHSV( HUE_RED, 255, i);
////      }
////      FastLED.show();
////      delay(15);
////    }
////    for (uint8_t i = 220; i > 30; i--) {
////      for (int j = 0; j < 6; j++) {
////        leds[j] = CHSV( HUE_RED, 255, i);
////      }
////      FastLED.show();
////      delay(15);
////    }
////  }
//}

#endif  
