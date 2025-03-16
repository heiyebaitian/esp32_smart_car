#ifndef ESPNOW_H
#define ESPNOW_H

#include <WiFi.h>
#include <esp_now.h>

//全0xFF的Mac地址时广播到附近所有ESPNOW设备
uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//uint8_t PairAddress[6] = {};
//
//esp_now_peer_info_t peerInfo = {};
//
//#define MAC_ADDRESS_OFFSET 10 // 存MAC地址的EEPROM地址，从10开始


// Global copy of slave
#define NUMSLAVES 5
esp_now_peer_info_t slaves[NUMSLAVES] = {};
int SlaveCnt = 0;

#define CHANNEL 1
#define PRINTSCANRESULTS 0

//bool SCAN=1;
//int8_t scanResults = 0;
//
//// 初始化一个二维字符数组，用于存储MAC地址  
//char PairMacAddress[5][18]; // 5表示你可以存储5个MAC地址，18是因为一个MAC地址的长度是17个字符，再加一个'\0'字符用于字符串结束  
//int macCount = 0; // 记录已存储的MAC地址数量

//配对连接pair with another ESP-NOW device
void BroadcastMode(){ 
  SlaveCnt=1;    
  memcpy(&slaves[0].peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
    esp_now_add_peer(&slaves[0]);
  } 
}

// Scan for slaves in AP mode
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  //reset slaves
  memset(slaves, 0, sizeof(slaves));
  SlaveCnt = 0;
  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
      }
      delay(10);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];

        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slaves[SlaveCnt].peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
        slaves[SlaveCnt].channel = CHANNEL; // pick a channel
        slaves[SlaveCnt].encrypt = 0; // no encryption
        SlaveCnt++;
      }      
    }
  }

  if (SlaveCnt > 0) {
    Serial.print(SlaveCnt); Serial.println(" Slave(s) found, processing..");
  } else {
    Serial.println("No Slave Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}

// Check if the slave is already paired with the master.
// If not, pair the slave with master
void manageSlave() {
  if (SlaveCnt > 0) {
    for (int i = 0; i < SlaveCnt; i++) {
      Serial.print("Processing: ");
      for (int ii = 0; ii < 6; ++ii ) {
        Serial.print((uint8_t) slaves[i].peer_addr[ii], HEX);
        if (ii != 5) Serial.print(":");
      }
      Serial.print(" Status: ");
      // check if the peer exists
      bool exists = esp_now_is_peer_exist(slaves[i].peer_addr);
      if (exists) {
        // Slave already paired.
        Serial.println("Already Paired");
      } else {
        // Slave not paired, attempt pair
        esp_err_t addStatus = esp_now_add_peer(&slaves[i]);
        if (addStatus == ESP_OK) {
          // Pair success
          Serial.println("Pair success");
        } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
          // How did we get so far!!
          Serial.println("ESPNOW Not Init");
        } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
          Serial.println("Add Peer - Invalid Argument");
        } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
          Serial.println("Peer list full");
        } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
          Serial.println("Out of memory");
        } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
          Serial.println("Peer Exists");
        } else {
          Serial.println("Not sure what happened");
        }
        delay(100);
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process,Switch to Broadcast Mode");
  }
}

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
Data_Package data; //Create a variable with the above structure

void resetData() {  //数据重置
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.buttonR1 = 1;
  data.buttonR2 = 1;
  data.j1Button = 1;
  data.j2Button = 1;
  data.buttonLB = 1;
  data.buttonRB = 1;
  data.tSwitch1 = 1;
  data.roll=127;
  data.pitch=127;
  data.buttonR3 = 1;  
}

/////////////////////////////////////////

    
// 数据发送回调函数
// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
//  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// 数据接收回调函数
// callback when data is received from Slave to Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  memcpy(&data, incomingData, sizeof(data));
  
//  Serial.print("j1PotX: ");
//  Serial.print(data.j1PotX);
//  Serial.print("; j1PotY: ");
//  Serial.print(data.j1PotY); 
//  Serial.print("; j2PotX: ");
//  Serial.print(data.j2PotX);
//  Serial.print("; j2PotY: ");
//  Serial.println(data.j2PotY);
}


// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == 0) {       //ESP_OK=0
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

////配对连接pair with another ESP-NOW device
//void pair_device(){     
//  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
//  if (!esp_now_is_peer_exist(broadcastAddress))
//  {
//    esp_now_add_peer(&peerInfo);
//  } 
//}

//// 发送数据 send data
//void sendData() {       //uint8_t data  
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
//}


// 发送数据 send data
void sendData() {
  for (int i = 0; i < SlaveCnt; i++) {
    const uint8_t *peer_addr = slaves[i].peer_addr;
    esp_err_t result = esp_now_send(peer_addr, (uint8_t *) &data, sizeof(data));
//    Serial.print("Send Status: ");
//    if (result == ESP_OK) {
//      Serial.println("Success");
//    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
//      // How did we get so far!!
//      Serial.println("ESPNOW not Init.");
//    } else if (result == ESP_ERR_ESPNOW_ARG) {
//      Serial.println("Invalid Argument");
//    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
//      Serial.println("Internal Error");
//    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
//      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
//    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
//      Serial.println("Peer not found.");
//    } else {
//      Serial.println("Not sure what happened");
//    }
//    delay(10);
  }
}

#endif  
