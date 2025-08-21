#include "firmware.h"
#include "accel.h"
#include "mag.h"


//#define ARDUINO_VISUALIZATION


// note: to compile everything, the -Os flag *with LTO* is needed!
#define USE_MAG       // U2
#define USE_ACC0      // U3
#define USE_ACC1      // U4
#define USE_CAP_SENSE // U5


// capacitive touch sensing
#ifdef USE_CAP_SENSE
const int cap_touch_pin = PA3;
#endif

// accelerometers
#if defined(USE_ACC0) || defined(USE_ACC1)
MC3672 accel = MC3672();
#endif

// magnetometer
#ifdef USE_MAG
Adafruit_MMC5603 mag = Adafruit_MMC5603();
#endif



///////////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(led, OUTPUT);

  // UART, with custom pins:
  Serial.setRx(_RX);
  Serial.setTx(_TX);
  Serial.begin(115200);
  //Serial.print("t mx my mz ax ay az\n");

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
        Serial.print("\t!\n");
    }
  }
  mag.setDataRate(100); // in Hz, from 1-255 or 1000
  mag.setContinuousMode(true);
#endif
}



///////////////////////////////////////////////////////////////////////////////
void loop()
{
  // touch sensor: 1 or 0
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

  async_led_blink(200);
  wait_fps(50); // frames per second (max ~ 55)
}



///////////////////////////////////////////////////////////////////////////////
void printTouchSense()
{
#ifdef USE_CAP_SENSE
//int value = digitalRead(cap_touch_pin) * 255; // TODO: test w/ pinMode(cap_touch_pin, INPUT) ?
  int value = analogRead(cap_touch_pin) / 4;    // max 255

  Serial.print(value);
  Serial.print(" ");
#endif
}


///////////////////////////////////////////////////////////////////////////////
void printAccelData(uint8_t addr)
{
#if defined(USE_ACC0) || defined(USE_ACC1)
    accel.setDeviceAddr(addr);
    MC3672_acc_t rawAccel = accel.readRawAccel();
    delay(5);

    // Display the results (acceleration is measured in m/s^2)
    Serial.print(rawAccel.XAxis_g); Serial.print(" ");
    Serial.print(rawAccel.YAxis_g); Serial.print(" ");
    Serial.print(rawAccel.ZAxis_g); Serial.print(" ");
#endif
}


///////////////////////////////////////////////////////////////////////////////
void printMagData()
{
#ifdef USE_MAG

  sensors_event_t event;
  mag.getEvent(&event);

#ifdef USE_CAP_SENSE // TODO: check if it's still needed!
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

#endif
}


///////////////////////////////////////////////////////////////////////////////
void async_led_blink(int ms_toggle)
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

