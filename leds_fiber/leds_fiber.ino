#include <Adafruit_NeoPixel.h>
#include "firmware.h"
#include "accel.h"



// capacitive sensing
const int cap_touch_pin = PA3; // NOT USED IF USED BY UART (see below)
ADCTouchSensor cap_touch = ADCTouchSensor(cap_touch_pin, GROUNDED_PIN);
int cap_touch_reference = 0;

// accelerometer
MC3672 accel = MC3672();

// LED strip
const int STRIP_PIN = _RX; // temporary tests
const int STRIP_PIXEL_NUM = 50;
Adafruit_NeoPixel strip(STRIP_PIXEL_NUM, STRIP_PIN, NEO_GRB + NEO_KHZ800);




///////////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(led, OUTPUT);

  // UART, with custom pins:
  Serial.setRx(PA3);            // temporary tests
  Serial.setTx(_TX);
  Serial.begin(115200);
  Serial.print("Starting!\n");

#ifdef USING_TOUCH_SENSING
  // capacitive sensing
  cap_touch.begin();
  delay(100);
  cap_touch_reference = cap_touch.read(50);
#endif

  // accelerometer
  accel.start(I2C_ADDR0);

#ifdef USING_LED_STRIP
  // LED strip
  strip.begin();
#endif
}



///////////////////////////////////////////////////////////////////////////////
void loop()
{
  // accelerometer:
  int tilt = getAccelData();

  // touch sensor
  static bool isOn = true;
  bool wasTouched = getTouch();
  if (wasTouched) isOn = !isOn;
  if (!isOn) tilt = 0;

  // LED strip update:
  led_strip(tilt);

  async_debug_led_blink(200);
  wait_fps(30); // no need to be too fast
}



///////////////////////////////////////////////////////////////////////////////
int getAccelData()
{
    accel.setDeviceAddr(I2C_ADDR0);
    MC3672_acc_t rawAccel = accel.readRawAccel();

    const float COEF = 0.3;
    static float old_val = 0;

    // smoothen
    float new_val = rawAccel.YAxis_g * COEF  +  old_val * (1-COEF);

    old_val = new_val;
    return int(new_val);
}


///////////////////////////////////////////////////////////////////////////////
void led_strip(int tilt)
{
    static int index = 0;
    const int threshold = 15; // TODO find

    // default is off:
    uint32_t start_color = strip.Color(0, 0, 0);
    uint32_t end_color = strip.Color(0, 0, 0);

    if (tilt < -threshold) {
        start_color = strip.Color(100, 0, 0); // red
    } else if (tilt >  threshold) {
        end_color = strip.Color(100, 0, 0); // red
    }

    for(int i=0; i<STRIP_PIXEL_NUM/2; i++) {
        strip.setPixelColor(i, start_color);
    }
    for(int i=STRIP_PIXEL_NUM/2; i<STRIP_PIXEL_NUM; i++) {
        strip.setPixelColor(i, end_color);
    }

    strip.show();
}


///////////////////////////////////////////////////////////////////////////////
bool getTouch()
{
  static float old_val = 0.0;
  const float smooth_coef = 0.5;
  const int threshold = 10;

  float value = cap_touch.read(5) - cap_touch_reference;
  value = value * smooth_coef  +  old_val * (1 - smooth_coef);
  old_val = value;

  return (value > threshold) ;
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

