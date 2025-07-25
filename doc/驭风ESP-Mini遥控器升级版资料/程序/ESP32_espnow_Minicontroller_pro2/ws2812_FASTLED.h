#ifndef WS2812_FASTLED_H
#define WS2812_FASTLED_H

#include <FastLED.h>               //控制RGB灯的库文件

#define NUM_LEDS 8           //灯珠的数量，若改造成其他数量自行更改
#define LED_DT 17              //RGB灯上DI信号输入接引脚
#define LED_TYPE WS2812       //WS2812LED灯内置芯片对应的型号
#define COLOR_ORDER GRB       //WS2812LED灯对应的色彩顺序


uint8_t max_bright=127;       //可改变此值调节LED灯的最大亮度（0-255）建议不要太亮对眼睛不好
uint8_t min_bright=5;       //可改变此值调节LED灯的最小亮度（0-255）
uint8_t rgb_bright=50;       //可改变此值调节LED灯的默认亮度（0-255）建议不要太亮对眼睛不好
uint8_t lightState=0;         //可改变此值调节默认的灯开关模式，默认为模式1，0为关
                              //可增加更多的变化模式

#define LED_DT1 14              //左摇杆RGB灯上DI信号输入接引脚
#define LED_DT2 16              //右摇杆RGB灯上DI信号输入接引脚
#define twinkleInterval  200    //  摇杆LED星闪模式，闪烁间隔时间 (毫秒)
#define twinkleChance 20        //  闪烁数量，数值越大闪烁越多（0-255）

CRGB leds[NUM_LEDS];                    //LED建立对象等
CHSV myHSVcolor(0,255,rgb_bright);  
CRGB leds1[NUM_LEDS];                    //左摇杆LED建立对象等
CRGB leds2[NUM_LEDS];                    //右摇杆LED建立对象等

//int thisdelay = 15;                                            // Standard delay
uint8_t thisdiff = 1;                                         // Standard palette jump
bool    thisdir = 0;                                          // Standard direction
uint8_t thisindex = 0;                                        // Standard palette index
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
//uint8_t thatrot = 0;                                          // You can change how quickly the hue rotates for the other wave. Currently 0.


void FastLED_ini(){
  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS);      //LED初始化
  LEDS.addLeds<LED_TYPE,LED_DT1,COLOR_ORDER>(leds1,NUM_LEDS);      //LED初始化
  LEDS.addLeds<LED_TYPE,LED_DT2,COLOR_ORDER>(leds2,NUM_LEDS);      //LED初始化
  FastLED.setBrightness(rgb_bright);                             //设置LED亮度
}


void rainbow_march() {                                           // The fill_rainbow call doesn't support brightness levels
  if (thisdir == 0) thisindex += thisrot; else thisindex-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(leds, NUM_LEDS, thisindex, thisdiff);               // I don't change deltahue on the fly as it's too fast near the end of the strip.
} // rainbow_march()


void addGlitter(uint8_t chanceofGlitter){      //随机闪光灯效果
  if (random8()<chanceofGlitter){
    leds[random8(NUM_LEDS)]=CRGB::White;
  }
}

void runLED(){
  if (lightState==0){
  fill_solid (leds,NUM_LEDS,CRGB::Black);     //关灯
  FastLED.show();   
  }else if(lightState==1){                   //特效1
  thisrot=1; thisdiff=1;
  rainbow_march();
//  addGlitter(2);                             //随机闪光灯效果，调节数值改变闪光频率
  FastLED.show();
//    delay(thisdelay); 
  }else if(lightState==2){                    //特效2
  thisrot=2; thisdiff=10;
  rainbow_march();
//  addGlitter(2);                             //随机闪光灯效果，调节数值改变闪光频率
  FastLED.show();
//    delay(thisdelay);
  }else if(lightState==3){                    //特效3
  thisrot=1; thisdiff=5;
  rainbow_march();
//  addGlitter(2);                             //随机闪光灯效果，调节数值改变闪光频率
  FastLED.show();
//    delay(thisdelay);                            
  }
}

void joy1_led_blink(){    //星闪模式
  int pos1 = random8(NUM_LEDS);
  EVERY_N_MILLISECONDS( twinkleInterval ) { 
    if( random8() < twinkleChance) {
      leds1[pos1] = ColorFromPalette( PartyColors_p, random8(255), max_bright/2, LINEARBLEND);
    }   
  }
  EVERY_N_MILLISECONDS( 10 ) { 
    fadeToBlackBy( leds1, NUM_LEDS, 10);
  } 
  FastLED.show();
}
void joy2_led_blink(){    //星闪模式
  int pos2 = random8(NUM_LEDS);
  EVERY_N_MILLISECONDS( twinkleInterval ) { 
    if( random8() < twinkleChance) {
      leds2[pos2] = ColorFromPalette( PartyColors_p, random8(255), max_bright/2, LINEARBLEND);
    }   
  }
  EVERY_N_MILLISECONDS( 10 ) { 
    fadeToBlackBy( leds2, NUM_LEDS, 10);
  }
  FastLED.show(); 
}

#endif 
