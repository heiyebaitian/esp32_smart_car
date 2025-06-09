#ifndef __SONO_LUMIN_ALERT__H
#define __SONO_LUMIN_ALERT__H
#pragma once

// 定义音调频率
#define C0 -1
#define B2 123
#define C3 130
#define CH3 139
#define D3 146
#define DH3 155
#define E3 164
#define F3 174
#define FH3 185
#define G3 196
#define GH3 207
#define A3 220
#define AH3 233
#define B3 246
#define C4 261
#define CH4 277
#define D4 293
#define DH4 311
#define E4 329
#define F4 349
#define FH4 370
#define G4 392
#define GH4 415
#define A4 440
#define AH4 466
#define B4 493
#define C5 523
#define CH5 554
#define D5 587
#define DH5 622
#define E5 659
#define F5 698
#define FH5 740
#define G5 784
#define GH5 831
#define A5 880
#define AH5 932
#define B5 988
#define C6 1046
#define CH6 1108
#define D6 1174
#define DH6 1244
// 结束定义音调频率

typedef enum
{
      SPEAKER_OFF = 0, SPEAKER_POWER_ON_TONE, SPEAKER_CAS_PROMPT_TONE
} SPEAKER_MODE;

typedef enum
{
      LIGHT_OFF = 0, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_Y, LIGHT_W
} LIGHT_MODE;

typedef enum
{
      SLA_READY = 0, SLA_BOOT_OK, SLA_CAS_TRIGGERED, SLA_FIRE, SLA_SYS_ERROR
} SLA_MODE;

extern SPEAKER_MODE Speaker_mode;
extern LIGHT_MODE Light_mode;
extern SLA_MODE SLA_mode;

void power_on_tone_play();
void SonoLuminAlert_Task(void * pvParameters);
void run_SonoLuminAlert_Task_on_Core_1();


#endif