/******************************************************************
  @file       complementaryFilter.ino
  @brief      Test complementary filter on the Nano 33 BLE
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     2.0.0
  Date:        15/12/22

  1.0.0 Original Release.           22/02/22
  1.1.0 Added NONE fusion option.   25/05/22
  2.0.0 Changed Repo & Branding     15/12/22
  
******************************************************************/

#include <ReefwingAHRS.h>

LSM9DS1 imu;
EulerAngles angles;

int loopFrequency = 0;
const long displayPeriod = 1000;
unsigned long previousMillis = 0;

void setup() {
  // Initialise the LSM9DS1 IMU and LPS22HB Barometer
  imu.begin();

  //  Positive magnetic declination - Sydney, AUSTRALIA
  imu.setDeclination(12.717);
  imu.setFusionAlgorithm(SensorFusion::COMPLEMENTARY);
  imu.setAlpha(0.95);

  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  if (imu.connected()) {
    Serial.println("LSM9DS1 IMU Connected."); 

    //  Paste your calibration bias offset HERE
    //  This information comes from the testAndCalibrate.ino 
    //  sketch in the library examples sub-directory.
    imu.loadAccBias(0.070862, -0.046570, -0.016907);
    imu.loadGyroBias(0.800018, 0.269165, -0.097198);
    imu.loadMagBias(-0.192261, -0.012085, 0.118652);

    //  Start processing IMU data.
    imu.start();
  }
  else {
    Serial.println("LSM9DS1 Accelerometer, Magnetometer and Gyroscope not found.");
    while(1);
  }
}

void loop() {
  //  Check for new IMU data and update angles
  angles = imu.update();

  //  Display sensor data every displayPeriod, non-blocking.
  if (millis() - previousMillis >= displayPeriod) {
    Serial.print("Roll: ");
    Serial.print(angles.roll);
    Serial.print("\tPitch: ");
    Serial.print(angles.pitch);
    Serial.print("\tYaw: ");
    Serial.print(angles.yaw);
    Serial.print("\tHeading: ");
    Serial.print(angles.heading);
    Serial.print("\tLoop Frequency: ");
    Serial.print(loopFrequency);
    Serial.println(" Hz");

    loopFrequency = 0;
    previousMillis = millis();
  }

  loopFrequency++;
}