#ifndef WEMOS_OLED_H
#define WEMOS_OLED_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

//取64*30 逐行式 顺向高位在前
static const unsigned char PROGMEM pic1[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xC0,0x00,
0x1F,0xE0,0x00,0x00,0x07,0xFF,0xF0,0x00,0x1F,0xF8,0x00,0x00,0x0F,0xFF,0xFC,0x00,
0x1F,0xFC,0x00,0x00,0x0F,0xFF,0xFE,0x00,0x1F,0xFE,0x00,0x00,0x0F,0xFF,0xFF,0x00,
0x1F,0xFF,0x00,0x00,0x0F,0xFF,0xFF,0x80,0x0F,0xFF,0x3F,0xC0,0x0F,0xFF,0x1F,0x80,
0x07,0xFF,0x7F,0xF8,0x0F,0xE0,0x01,0xC0,0x00,0x07,0x7F,0xFC,0x07,0xE0,0x00,0xC0,
0x1C,0x03,0x7F,0xFE,0x07,0xC0,0x1C,0xC0,0x3E,0x03,0x7F,0xFE,0x07,0xDC,0x3E,0xC0,
0x3E,0x03,0x7F,0xFE,0x07,0xDE,0x7E,0x40,0x3E,0x02,0x3F,0xFE,0x07,0xDF,0xFE,0x40,
0x3E,0x06,0x01,0xFC,0x07,0xC7,0xFE,0x40,0x3E,0x08,0xC0,0x1C,0x0F,0xC7,0xFC,0x40,
0x1F,0x00,0xF8,0x38,0x0F,0xCF,0xF8,0x40,0x0F,0xFC,0xFF,0x70,0x0F,0x9F,0xF8,0x40,
0x07,0xE6,0x7F,0xE0,0x0F,0x9F,0xF8,0x40,0x00,0x06,0x3F,0xF8,0x0F,0x9F,0xCC,0x60,
0x03,0xE6,0x0F,0xF8,0x0F,0x9F,0x06,0x70,0x0F,0xE6,0x0F,0xF8,0x0F,0x0E,0x00,0x3C,
0x1F,0x8C,0x18,0x30,0x0F,0x00,0x00,0x00,0x0F,0x08,0x00,0x00,0x0E,0x00,0x00,0x00,
0x0C,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void oleddisplay(){      //WEMOS_OLED扩展板屏幕显示
  display.clearDisplay();//清屏    
  display.setCursor(1,23);            
  display.print("X:");             
  display.print(data.j1PotX);     
  display.setCursor(1,35);
  display.print("Y:");  
  display.print(data.j1PotY);    
  display.setCursor(34,23);
  display.print("X:");             
  display.print(data.j2PotX);     
  display.setCursor(34,35);
  display.print("Y:");  
  display.print(data.j2PotY); 

  if(!data.buttonLB) display.fillCircle(0, 0, 3, WHITE);
  if(!data.buttonR2) display.fillCircle(0, 48, 3, WHITE);
  if(!data.buttonRB) display.fillCircle(64, 0, 3, WHITE);
  if(!data.buttonR1) display.fillCircle(64, 48, 3, WHITE);  
  display.invertDisplay((data.tSwitch1?0:1));
  
  display.drawLine(0, 14, 64, 14, WHITE);
  display.setCursor(31,4);display.print(voltage);display.print("V");
  display.drawRoundRect(3,4,20,7,1,WHITE);
  display.fillRoundRect(3,4,map(percentage,0,100,0,20),7,1,WHITE);
  display.fillRect(23,6,2,3,WHITE);
    
  display.display();
}

//在CORE0上运行OLED相关程序（Arduino默认在CORE1上运行）
/*************************************/
TaskHandle_t Task1;

  //Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  //
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

/************************************************/
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);                  //注：有时地址不是0x3C,可能是0x3D等，若屏幕不亮可以试试用wire库中的示例程序扫描地址
/************************************************/
  
  display.setTextColor(WHITE);//开像素点发光
  display.setTextSize(1);  
  
  for(int i=0;i<=18;i++){
    display.clearDisplay();//清屏
    display.drawBitmap(0, i, pic1, 64, 30, WHITE); //画出字符对应点阵数据
    display.display();//开显示
    delay(20);
  }
  for(int i=18;i>=9;i--){
    display.clearDisplay();//清屏
    display.drawBitmap(0, i, pic1, 64, 30, WHITE); //画出字符对应点阵数据
    display.display();//开显示
    delay(20);    
  }
  delay(600);

  for(;;){
    oleddisplay();//WEMOS_OLED扩展板屏幕显示 
    delay(10);

/************************************************/
  //同时按下四键及拨动开关3秒后进行摇杆原点纠偏，此时不要碰摇杆 
    if (!data.buttonR1&&!data.buttonR2&&!data.buttonLB&&!data.buttonRB&&!data.tSwitch1){
      delay (3000);
      if (!data.buttonR1&&!data.buttonR2&&!data.buttonLB&&!data.buttonRB&&!data.tSwitch1){
        display.clearDisplay();//清屏    
        display.setCursor(6,6);  display.print("Zero_test"); 
        display.setCursor(4,22);  display.print("Testing...");    
        display.display();//开显示
        delay (1000);
        
        display.clearDisplay();//清屏    
        display.setCursor(6,6);  display.print("Zero_test");    
        display.setCursor(1,18);            
        display.print("X:");             
        display.print(EEPROM.read(1));     
        display.setCursor(1,30);
        display.print("Y:");  
        display.print(EEPROM.read(2));    
        display.setCursor(34,18);
        display.print("X:");             
        display.print(EEPROM.read(3));     
        display.setCursor(34,30);
        display.print("Y:");  
        display.print(EEPROM.read(4));
        display.display();//开显示
        delay (2000); 
      }
    }
  
  //同时按下四键后变换背光RGB模式 
    if (!data.j1Button&&!data.j2Button&&!data.buttonLB&&!data.buttonRB){
      delay (300);
      if (!data.j1Button&&!data.j2Button&&!data.buttonLB&&!data.buttonRB){        
        display.clearDisplay();//清屏    
        display.setCursor(8,16);  display.print("Back_RGB:");
        display.setCursor(15,28); display.print("Mode:");
        display.setCursor(47,28); display.print(lightState);     
        display.display();//开显示
        delay (1500);
      }
    } 
/************************************************/ 

  } 
}

void run_OLED_on_Core_0(){
//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
}
/*************************************/
#endif 
