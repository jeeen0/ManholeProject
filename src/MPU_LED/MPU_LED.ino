#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int L1 = 2;
void setup(void) {
  Serial.begin(115200);

  if(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1){
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  pinMode(L1, OUTPUT);
  delay(100);

}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp; //a : acceleration g: gyro
  mpu.getEvent(&a, &g, &temp);

  Serial.print("x: ");
  Serial.print(a.acceleration.x);
  Serial.print("\ty: ");
  Serial.print(a.acceleration.y);
  Serial.print("\tz: ");
  Serial.println(a.acceleration.z);
  
  if(a.acceleration.y <-8.00){
    Serial.println("불 켜져야 된다.");
    digitalWrite(L1, HIGH);
  } else {
    Serial.println("불 안켜짐");
    digitalWrite(L1, LOW);
  }
  delay(500);
}