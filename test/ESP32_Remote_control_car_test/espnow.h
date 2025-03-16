
#ifndef __ESPNOT__H
#define __ESPNOT__H
#pragma once


void resetData();
void OnDataRecv(const esp_now_recv_info_t* mac, const uint8_t *incomingData, int len);
void InitESPNow();


#endif