#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LedControl.h>
#include <SoftwareSerial.h>

Adafruit_MPU6050 mpu;
SoftwareSerial BT_serial(3,2);       //블루투스 Tx->3번핀, Rx->2번핀
LedControl lc = LedControl(8, 10, 9, 1); // (DIN, CLK, CS, number)

unsigned long timePrev;   //지난시간
bool isLedOn = false; // LED 상태를 저장하는 변수
byte pattern[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};

void setup(void) {
  Serial.begin(9600);
  BT_serial.begin(9600);
  if(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1){
      delay(10);
    }
  }
//  Serial.println("MPU6050 Found!");
  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 15); // 밝기
  lc.clearDisplay(0); // 도트 모두지우기
  delay(100);
}

void showLED(byte arr[], int a) {
  if(a == 1) {
    for(int i = 0; i < 8; i++) {
      lc.setRow(0, i, arr[i]);
    }
  } else {
    for(int i = 0; i < 8; i++) {
      lc.setRow(0, i, B00000000);
   }
  }
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp; //a : acceleration g: gyro
  mpu.getEvent(&a, &g, &temp);

  char result;
  
  if(Serial.available()>0){              //씨리얼에서 신호가 있으면,
    char result = (char)Serial.read();
    Serial.println(result);

   switch(result){
      case '0':             //물 참(blink처럼 뚜껑 열렸을 때랑 구분 원함)
        isLedOn = true; 
        Serial.println("물");   
        break;

      case '1':           //뚜껑 열림
        isLedOn = true;
        Serial.println("열림");
        break;

       case '2':           //정상
        isLedOn = false;
        Serial.println("정상");
        break;
      }
    }

  if(isLedOn) {
    showLED(pattern, 1);
    delay(1000);
    Serial.print("LED ON,");
  } 
  else {
    showLED(pattern, 0);
    Serial.print("off,");
  }
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.println(a.acceleration.z);

      if((a.acceleration.y <-4.00) || (a.acceleration.x <-4.00) || (a.acceleration.y >4.00) || (a.acceleration.x >4.00)){
      Serial.print("불켜,");
      Serial.print(a.acceleration.x);
      Serial.print(",");
      Serial.print(a.acceleration.y);
      Serial.print(",");
      Serial.println(a.acceleration.z);
      isLedOn = true;
//      break;
    } 
    else {
      Serial.print("불안켜짐,");
      Serial.print(a.acceleration.x);
      Serial.print(",");
      Serial.print(a.acceleration.y);
      Serial.print(",");
      Serial.println(a.acceleration.z);
      isLedOn =false;
    }
    delay(500);
  }
