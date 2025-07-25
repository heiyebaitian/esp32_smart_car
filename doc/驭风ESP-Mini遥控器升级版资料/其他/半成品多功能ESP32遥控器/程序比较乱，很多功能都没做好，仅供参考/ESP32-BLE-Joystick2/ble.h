#ifndef BLE_H_
#define BLE_H_

#include <BleGamepad.h>
/*
#define numOfButtons        10
#define numOfHatSwitches    1
#define enableX             true    
#define enableY             true
#define enableZ             true
#define enableRZ            true
#define enableRX            true
#define enableRY            true
#define enableSlider1       true
#define enableSlider2       true
#define enableRudder        false
#define enableThrottle      false
#define enableAccelerator   false
#define enableBrake         false
#define enableSteering      false
*/

BleGamepad bleGamepad("ESP Controller", "Yufeng ESP Controller", 100);


int period = 1000;
unsigned long time_now = 0;

const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 2;    // Delay in milliseconds between pot samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports
const int debounceDelay = 10;        // Delay in milliseconds between button press

int previousButton1State = HIGH;
int previousButton2State = HIGH;
int previousButton3State = HIGH;
int previousButton4State = HIGH;
int previousButton5State = HIGH;
int previousButton6State = HIGH;
int previousButton7State = HIGH;
int previousButton8State = HIGH;
int previousButton9State = HIGH;
int previousButton10State = HIGH;
int previousButton11State = HIGH;
int previousButton12State = HIGH;
int previousButton13State = HIGH;


#endif 
