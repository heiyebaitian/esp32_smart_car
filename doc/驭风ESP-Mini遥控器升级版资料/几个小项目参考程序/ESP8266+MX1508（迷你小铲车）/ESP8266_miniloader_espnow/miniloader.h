#include <Servo.h>

#define ArmPin 5        //臂舵机 5引脚接臂舵机
#define BucketPin 13    //斗舵机 13引脚接挖斗舵机
#define LightFrontPin 16  //前灯 16引脚接前照射灯
#define LightEmergencyPin 0  //运行警示灯 0引脚接工作警示灯
#define MAX_SPEED 180     //0-255

bool LedFront = HIGH;
bool LedEmergencyOff = LOW;
#define Armdir 80     //铲臂默认角度，按需修改
const int ArmMin = 5; //铲臂最大及最小角度，按需修改
const int ArmMax = 100;
#define Bucketdir 90  //铲斗默认角度，按需修改
const int BucketMin = 5;  //铲斗最大及最小角度，按需修改
const int BucketMax = 160;
int LoaderArmAngle = Armdir, LoaderBucketAngle = Bucketdir;//初始化舵机角度
int limit=45;

Servo LoaderArm, LoaderBucket;  //建立两个舵机对象 LoaderArm和LoaderBucket

void miniloader_ini(){  //铲斗等初始化
  pinMode(LightFrontPin, OUTPUT);
  pinMode(LightEmergencyPin, OUTPUT);

  digitalWrite(LightFrontPin, LedFront);
  digitalWrite(LightEmergencyPin , LedEmergencyOff);

  LoaderArm.attach(ArmPin);           //定义臂舵机的对应引脚
  LoaderArm.write(LoaderArmAngle);    //初始化臂舵机角度

  LoaderBucket.attach(BucketPin);     //定义挖斗舵机对应引脚
  LoaderBucket.write(LoaderBucketAngle);   //初始化挖斗舵机角度
}
