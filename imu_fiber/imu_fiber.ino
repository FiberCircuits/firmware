#include "firmware.h"
#include "accel.h"
#include "mag.h"

// #define ARDUINO_VISUALIZATION


#ifdef USE_CAP_SENSE // defined in firmware.h
// capacitive sensing
const int cap_touch_pin = PA3;
ADCTouchSensor cap_touch = ADCTouchSensor(cap_touch_pin, GROUNDED_PIN);
int cap_touch_reference = 0;
#endif

// accelerometer
MC3672 accel = MC3672();

// magnetometer
Adafruit_MMC5603 mag = Adafruit_MMC5603();


#define USE_MAG // U2
#define USE_ACC0 // U3
#define USE_ACC1 // U4

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(led, OUTPUT);

  // UART, with custom pins:
  Serial.setRx(_RX);
  Serial.setTx(_TX);
  Serial.begin(115200);
  Serial.print("t mx my mz ax ay az\n");

#ifdef USE_CAP_SENSE
  // capacitive sensing
  cap_touch.begin();
  delay(100);
  cap_touch_reference = cap_touch.read(50);
#endif

  // accelerometer
#ifdef USE_ACC0
  accel.start(I2C_ADDR0);
#endif
#ifdef USE_ACC1
  accel.start(I2C_ADDR1);
#endif

#ifdef USE_MAG
  // magnetometer
  if (!mag.begin(MMC56X3_DEFAULT_ADDRESS, &Wire)) {
    while (1) {                                         // TODO ?
        Serial.print("!\n");
    }
  }
  mag.setDataRate(100); // in Hz, from 1-255 or 1000
  mag.setContinuousMode(true);
#endif
}



///////////////////////////////////////////////////////////////////////////////
void loop()
{
  // touch sensor
  printTouchSense();

#ifdef USE_MAG
  // magnetometer: heading, or (x, y)
  printMagData();
#endif

  // accelerometer: (x, y, z)
#ifdef USE_ACC0
  printAccelData(I2C_ADDR0);
#endif

#ifdef USE_ACC1
  printAccelData(I2C_ADDR1);
#endif

  Serial.print("\n");

  async_debug_led_blink(200);
  wait_fps(50); // frames per second (max ~ 55)
}



///////////////////////////////////////////////////////////////////////////////
void printTouchSense()
{
  // TODO: use simple ADC instead?
#ifdef USE_CAP_SENSE
  // capacitive sensing
  static float old_val = 0.0;
  const float smooth_coef = 0.5;

  // const float threshold = 5;
  // value = (value > threshold);

  float value = cap_touch.read(5) - cap_touch_reference;
  value = value * smooth_coef  +  old_val * (1 - smooth_coef);
  old_val = value;

 #ifdef ARDUINO_VISUALIZATION
  Serial.print(value * 100);
 #else
  Serial.print(value);
 #endif

  Serial.print(" ");
#endif
}


///////////////////////////////////////////////////////////////////////////////
void printAccelData(uint8_t addr)
{
    accel.setDeviceAddr(addr);
    MC3672_acc_t rawAccel = accel.readRawAccel();
    delay(5);

    // Display the results (acceleration is measured in m/s^2)
    Serial.print(rawAccel.XAxis_g); Serial.print(" ");
    Serial.print(rawAccel.YAxis_g); Serial.print(" ");
    Serial.print(rawAccel.ZAxis_g); Serial.print(" ");
}


///////////////////////////////////////////////////////////////////////////////
void printMagData()
{
  sensors_event_t event;
  mag.getEvent(&event);

#ifdef USE_CAP_SENSE
  // save space by moving the atan2 calculation to software
  Serial.print(event.magnetic.x);
  Serial.print(" ");
  Serial.print(event.magnetic.y);
  Serial.print(" ");
  Serial.print(event.magnetic.z);
 #ifdef ARDUINO_VISUALIZATION
  Serial.print(" -100 100 ");
 #else
  Serial.print(" ");
 #endif

#else

  // TODO: use gravity vector to rotate x, y, z measures and get the heading
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  heading = float(heading * float(180)) / float(3.14159); // => [-180 ; +180]
  // Normalize to 0-360
  // if (heading < 0) heading += 360;
  Serial.print(heading);
 #ifdef ARDUINO_VISUALIZATION
  Serial.print(" -180 180 ");
 #else
  Serial.print(" ");
 #endif

#endif
}


///////////////////////////////////////////////////////////////////////////////
void async_debug_led_blink(int ms_toggle)
{
  static uint32_t time_stamp = 0;

  if (millis() - time_stamp > ms_toggle) {
      time_stamp = millis();

      digitalWrite(led, LED_ON);
      delay(5);
      digitalWrite(led, !LED_ON);
  }
}


///////////////////////////////////////////////////////////////////////////////
void wait_fps(int fps) // tries to ensure a stable frames rate (Hz)
{
    static uint32_t time_stamp = 0;
    int delay_ms = 1000 / fps;

    while (millis() - time_stamp < delay_ms); // wait

    time_stamp = millis();
}

