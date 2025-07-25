#include "mapdata.h"
//#include "mpu.h"
#include "ble.h"
#include "espnow.h"
#include "rgb.h"
//#include "oled.h"




//**********************Setup*************************
void setup() { 
  eepromini();
    
  Serial.begin(115200); 

/*  // Initialize interface to the MPU6050
  initialize_MPU6050();
  //此函数可以用来纠正误差(貌似我程序有BUG在这用不了，开源版的程序倒是有用，还没找到原因)，若想了解用法可参考https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
  // Call this function if you need to get the IMU error values for your module
   //calculate_IMU_error();*/
   
  for (int i = 0; i < 7; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  delay(300);
  bleGamepad.begin();
  
  // The default bleGamepad.begin() above is the same as bleGamepad.begin(16, 1, true, true, true, true, true, true, true, true, false, false, false, false, false);
  // which enables a gamepad with 16 buttons, 1 hat switch, enabled x, y, z, rZ, rX, rY, slider 1, slider 2 and disabled rudder, throttle, accelerator, brake, steering
  // Auto reporting is enabled by default. 
  // Use bleGamepad.setAutoReport(false); to disable auto reporting, and then use bleGamepad.sendReport(); as needed
  
  //Setup controller with 10 buttons, accelerator, brake and steering
//  bleGamepad.setAutoReport(true);
//  bleGamepad.setControllerType(CONTROLLER_TYPE_GAMEPAD);  //CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
//  bleGamepad.begin(numOfButtons,numOfHatSwitches,enableX,enableY,enableZ,enableRZ,enableRX,enableRY,enableSlider1,enableSlider2,enableRudder,enableThrottle,enableAccelerator,enableBrake,enableSteering);
  
  Serial.println("Starting BLE work!");
  
  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS);      //LED初始化
  FastLED.setBrightness(max_bright);                             //LED亮度

  delay(300);
  
  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  Serial.println("ESPNow/Multi-Slave/Master Example");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();

  // 设置发送数据回调函数
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  resetData();
/*
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);//开像素点发光
  display.clearDisplay();//清屏
  
  oledmode=0;
  oleddisplay();
  delay(200);
  display.invertDisplay(true);
  delay(300);
  display.invertDisplay(false);
  delay(200);
  display.invertDisplay(true);
  delay(300);
  display.invertDisplay(false);
  delay(200);
  display.clearDisplay();//清屏
  
  oledmode=1;
  oleddisplay();
*/  
}

void loop() {  

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
    int adjustedValue = map(potValue, 0, 4095, 0, 255);


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
    int adjustedValue2 = map(potValue2, 0, 4095, 0, 255);

/*
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
    int adjustedValue3 = map(potValue3, 0, 4095, 255, 0);


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
    int adjustedValue4 = map(potValue4, 0, 4095, 255, 0);


    int potValues5[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues5[i] = analogRead(LP);
      delay(delayBetweenSamples);
    }
    int potValue5 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue5 += potValues5[i];
    }
    potValue5 = potValue5 / numberOfPotSamples;
    int adjustedValue5 = map(potValue5, 0, 4095, 0, 255);

    int potValues6[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues6[i] = analogRead(RP);
      delay(delayBetweenSamples);
    }
    int potValue6 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue6 += potValues6[i];
    }
    potValue6 = potValue6 / numberOfPotSamples;
    int adjustedValue6 = map(potValue6, 0, 4095, 0, 255);*/
    

  LX_to_send = map_normal(adjustedValue,  0, 115, 255, LX_inverted);
  LY_to_send = map_normal(adjustedValue2, 0, 115, 255, LY_inverted);
//  RX_to_send = map_normal(adjustedValue3, 0, 140, 255, RX_inverted);
//  RY_to_send = map_normal(adjustedValue4, 0, 140, 255, RY_inverted);

  /*  
  //Mode select    
  if(!mode)
  {
  LX_to_send = map_normal(analogRead(A3), 0, 512, 1023, LX_inverted);
  LY_to_send = map_normal(analogRead(A6), 0, 512, 1023, LY_inverted);
  RX_to_send = map_normal(analogRead(A2), 0, 512, 1023, RX_inverted);
  RY_to_send = map_normal(analogRead(A1), 0, 512, 1023, RY_inverted);  
  }
  
  if(mode)
  {
  LX_to_send = map_exponential(analogRead(A3), LX_inverted);
  LY_to_send = map_exponential(analogRead(A6), LY_inverted);
  RX_to_send = map_exponential(analogRead(A2), RX_inverted);
  RY_to_send = map_ex
  ponential(analogRead(A1), RY_inverted);   
  }
*/  
  LX_to_send = LX_to_send  + LX_fine;
  LY_to_send = LY_to_send  + LY_fine;
//  RX_to_send = RX_to_send  + RX_fine;
//  RY_to_send = RY_to_send  + RY_fine; 
    
  LX_to_send = constrain(LX_to_send,0,255);
  LY_to_send = constrain(LY_to_send,0,255);
//  RX_to_send = constrain(RX_to_send,0,255);  
//  RY_to_send = constrain(RY_to_send,0,255);
  
  data.j2PotX = LX_to_send;
  data.j2PotY = LY_to_send;
//  data.j2PotX = RX_to_send;  
//  data.j2PotY = RY_to_send; 
//  Serial.print(" LX: ");Serial.print(data.j1PotX);
//  Serial.print(" LY: ");Serial.print(data.j1PotY);
//  Serial.print(" RX: ");Serial.print(data.j2PotX);
//  Serial.print(" RY: ");Serial.println(data.j2PotY);  
  data.buttonL3 = digitalRead(R1);
  data.buttonL4 = digitalRead(R2);
  data.buttonL2 = digitalRead(R3);
  data.buttonL1 = digitalRead(R4);
//  data.buttonL1 = digitalRead(L1);
//  data.buttonL2 = digitalRead(L2);
//  data.buttonL3 = digitalRead(L3);
//  data.buttonL4 = digitalRead(L4);
//  data.j1Button = digitalRead(LS);
//  data.j2Button = digitalRead(RS);
  data.button7 = digitalRead(LT);
  data.button8 = digitalRead(RT);
//  data.tSwitch1 = digitalRead(BK);
  data.tSwitch2 = digitalRead(ST);
//  data.button11 = digitalRead(CT);
//  data.pot1 = adjustedValue5;  
//  data.roll = map(angleX, -90, +90, 0, 255);
//  data.pitch = map(angleY, -90, +90, 255, 0);

//************************************************************************  
  if (bleGamepad.isConnected()) {
//    while (millis() > time_now + period) {
////      Serial.println("Checking Battery Level");
//      batteryLevel();
//      time_now = millis();
//    }    
    gamepadbutton();

    bleGamepad.setAxes(LX_to_send, LY_to_send, 0, 0, 127, 127, 0,0,DPAD_CENTERED); 
    delay(delayBetweenHIDReports);
  }
    
/*    if (digitalRead(L1) == LOW){
      if (digitalRead(L2) == LOW){
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_UP_LEFT);
      } else if (digitalRead(L4) == LOW){
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_UP_RIGHT);
      } else {
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_UP);
      }
    }
    else if (digitalRead(L3) == LOW){
      if (digitalRead(L2) == LOW){
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_DOWN_LEFT);
      } else if (digitalRead(L4) == LOW){
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_DOWN_RIGHT);
      } else {
        bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_DOWN);
      }
    }
      
    else if (digitalRead(L2) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_LEFT);

    else if (digitalRead(L4) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_RIGHT);
    else 
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, adjustedValue5,0,DPAD_CENTERED); 
    delay(delayBetweenHIDReports);
  }
*/
//*****************************************************
  //  read_IMU();  
//  read_IMU();    // Use MPU6050 instead of Joystick 1 for controling left, right, forward and backward movements


  
  // ESP-NOW发送数据
  sendData();
  
//*****************************************************  
  unsigned long currentMillis = millis();    
  if (currentMillis - previousMillis1 >= LEDdelay) {    
    previousMillis1 = currentMillis;
    runLED();         //RGB环形LED灯的变化
  }
/*  if (currentMillis - previousMillis2 >= LEDdelay*10) {    
    previousMillis2 = currentMillis;
    batteryLevel();         //读电池电量
  }

  oleddisplay();*/
  
}

void gamepadbutton(){
  int currentButton1State = digitalRead(R1);
  int currentButton2State = digitalRead(R2);
  int currentButton3State = digitalRead(R3);
  int currentButton4State = digitalRead(R4);

  int currentButton7State = digitalRead(LT);
  int currentButton8State = digitalRead(RT);

  int currentButton10State = digitalRead(ST);

  
  if (currentButton1State != previousButton1State) {
      if (currentButton1State == LOW)
        bleGamepad.press(BUTTON_1);
      else
        bleGamepad.release(BUTTON_1);
    }
    previousButton1State = currentButton1State;

    if (currentButton2State != previousButton2State) {
      if (currentButton2State == LOW)
        bleGamepad.press(BUTTON_2);
      else
        bleGamepad.release(BUTTON_2);
    }
    previousButton2State = currentButton2State;

    if (currentButton3State != previousButton3State) {
      if (currentButton3State == LOW)
        bleGamepad.press(BUTTON_3);
      else
        bleGamepad.release(BUTTON_3);
    }
    previousButton3State = currentButton3State;

    if (currentButton4State != previousButton4State) {
      if (currentButton4State == LOW)
        bleGamepad.press(BUTTON_4);
      else
        bleGamepad.release(BUTTON_4);
    }
    previousButton4State = currentButton4State;

    if (currentButton7State != previousButton7State) {
      if (currentButton7State == LOW)
        bleGamepad.press(BUTTON_5);
      else
        bleGamepad.release(BUTTON_5);
    }
    previousButton7State = currentButton7State;

    if (currentButton8State != previousButton8State) {
      if (currentButton8State == LOW)
        bleGamepad.press(BUTTON_6);
      else
        bleGamepad.release(BUTTON_6);
    }
    previousButton8State = currentButton8State;  

    if (currentButton10State != previousButton10State) {
      if (currentButton10State == LOW)
        bleGamepad.press(BUTTON_8);
      else
        bleGamepad.release(BUTTON_8);
    }
    previousButton10State = currentButton10State;                
}

/*
void gamepadbutton(){
  int currentButton1State = digitalRead(R1);
  int currentButton2State = digitalRead(R2);
  int currentButton3State = digitalRead(R3);
  int currentButton4State = digitalRead(R4);
  int currentButton5State = digitalRead(LS);
  int currentButton6State = digitalRead(RS);
  int currentButton7State = digitalRead(LT);
  int currentButton8State = digitalRead(RT);
  int currentButton9State = digitalRead(BK);
  int currentButton10State = digitalRead(ST);
  int currentButton11State = digitalRead(CT);
  
  if (currentButton1State != previousButton1State) {
      if (currentButton1State == LOW)
        bleGamepad.press(BUTTON_1);
      else
        bleGamepad.release(BUTTON_1);
    }
    previousButton1State = currentButton1State;

    if (currentButton2State != previousButton2State) {
      if (currentButton2State == LOW)
        bleGamepad.press(BUTTON_2);
      else
        bleGamepad.release(BUTTON_2);
    }
    previousButton2State = currentButton2State;

    if (currentButton3State != previousButton3State) {
      if (currentButton3State == LOW)
        bleGamepad.press(BUTTON_3);
      else
        bleGamepad.release(BUTTON_3);
    }
    previousButton3State = currentButton3State;

    if (currentButton4State != previousButton4State) {
      if (currentButton4State == LOW)
        bleGamepad.press(BUTTON_4);
      else
        bleGamepad.release(BUTTON_4);
    }
    previousButton4State = currentButton4State;

    if (currentButton5State != previousButton5State) {
      if (currentButton5State == LOW)
        bleGamepad.press(BUTTON_9);
      else
        bleGamepad.release(BUTTON_9);
    }
    previousButton5State = currentButton5State;

    if (currentButton6State != previousButton6State) {
      if (currentButton6State == LOW)
        bleGamepad.press(BUTTON_10);
      else
        bleGamepad.release(BUTTON_10);
    }
    previousButton6State = currentButton6State;

    if (currentButton7State != previousButton7State) {
      if (currentButton7State == LOW)
        bleGamepad.press(BUTTON_5);
      else
        bleGamepad.release(BUTTON_5);
    }
    previousButton7State = currentButton7State;

    if (currentButton8State != previousButton8State) {
      if (currentButton8State == LOW)
        bleGamepad.press(BUTTON_6);
      else
        bleGamepad.release(BUTTON_6);
    }
    previousButton8State = currentButton8State;

    if (currentButton9State != previousButton9State) {
      if (currentButton9State == LOW)
        bleGamepad.press(BUTTON_7);
      else
        bleGamepad.release(BUTTON_7);
    }
    previousButton9State = currentButton9State;

    if (currentButton10State != previousButton10State) {
      if (currentButton10State == LOW)
        bleGamepad.press(BUTTON_8);
      else
        bleGamepad.release(BUTTON_8);
    }
    previousButton10State = currentButton10State;

    if (currentButton11State != previousButton11State) {
      if (currentButton11State == LOW)
        bleGamepad.press(BUTTON_11);
      else
        bleGamepad.release(BUTTON_11);
    }
    previousButton11State = currentButton11State;
                
}
*/

  
