#include <Adafruit_NeoPixel.h>
#include "firmware.h"
#include "accel.h"
#include "leds_text.h"


const int cap_touch_pin = PA3;

// accelerometer
MC3672 accel = MC3672();

#ifdef USING_LED_STRIP
  const int STRIP_PIN = _TX;
  const int STRIP_PIXEL_NUM = strip_num * strip_leds;
  Adafruit_NeoPixel strip(STRIP_PIXEL_NUM, STRIP_PIN, NEO_GRB + NEO_KHZ800);
#endif



///////////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(led, OUTPUT);

  // UART, with custom pins:
  DEBUG_setRx(_RX);
  DEBUG_setTx(_TX); // TX pin conflicts with LED strip!
  DEBUG_BEGIN(115200);
  DEBUG_PRINT("Starting!\n");

  accel.start(I2C_ADDR1);

#ifdef USING_LED_STRIP
  strip.begin();
#endif
}



///////////////////////////////////////////////////////////////////////////////
void loop()
{
#ifdef USING_LED_STRIP
  //                         01234567
  char *text = " HELLO WORLD !       ";
  char *ptr = text;

  const int display_width = 8;
  int shifts = strlen(text) - display_width;

  for (int i=0; i < shifts; i++) {
    text_to_pixels(ptr, pixels);
    rainbow_text(pixels);
    ptr = ptr + 1;
  }


/*
  // TODO: test:
  strip.fill(strip.Color(50, 0, 0)); // NOT WORKING
  leds_text_mask(HELLO_pixels);
  delay(500);

  strip.fill(strip.Color(50, 0, 0)); // NOT WORKING
  leds_text_mask(WORLD_pixels);
  delay(500);
*/
#endif

  //async_debug_led_blink(100);
}



///////////////////////////////////////////////////////////////////////////////
#ifdef USING_LED_STRIP

// Function to generate the 2D array for the given text
void text_to_pixels(const char *text, int pixels[strip_num][strip_leds]) {

    memset(pixels, 0, sizeof(int) * strip_num * strip_leds);

    int text_length = strlen(text);
    if (text_length > 10)
        text_length = 10; // just truncate for now

    for (int i = 0; i < text_length; ++i) {
        int index = -1;
        char c = text[i];

        if (c >= 'A' && c <= 'Z') {
            index = c - 'A';
        } else if (c >= '0' && c <= '9') {
            index = c - '0' + 26;
        } else if (c == ' ') {
            index = 36;
        } else if (c == '!') {
            index = 37;
        } else if (c == ':') {
            index = 38;
        } else if (c == '.') {
            index = 39;
        }
        if (index != -1) {
            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 5; ++col) {
                    if ((font[index][col] & (1 << (4 - row))) != 0) {
                        pixels[row][i * 5 + col] = 1;
                    }
                }
            }
        }
    }
}




void rainbow_text(const int pixels[strip_num][strip_leds]) {
  for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 10*256) {
    strip.rainbow(firstPixelHue, 1, 255, 50);
    leds_text_mask(pixels);
    strip.show();

    int tilt = getAccelData();
    int wait = map(tilt, -20,-80, 40,0);
    delay(wait);
  }
}

void leds_text_mask(const int pixels[strip_num][strip_leds])
{
    const int x_max = strip_leds;
    const int y_max = strip_num;
    int x = 0, y = 0;

    for (int i = 0; i < x_max * y_max; i++) {
        if (pixels[y][x] == 0)
            strip.setPixelColor(i, strip.Color(0, 0, 0)); // turn it off

        if (y % 2 == 0) {               // scan right in even rows
            if (x == x_max - 1) y++;    // go to next row if reached the end
            else                x++;
        } else {                        // scan left in odd rows
            if (x == 0) y++;            // go to next row if reached the end
            else        x--;            // otherwise go to "next" pixel
        }
    }
}
#endif



///////////////////////////////////////////////////////////////////////////////
int getAccelData()
{
    accel.setDeviceAddr(I2C_ADDR1);
    MC3672_acc_t rawAccel = accel.readRawAccel();

    const float COEF = 0.3;
    static float old_val = 0;

    // smoothen
    float new_val = rawAccel.ZAxis_g * COEF  +  old_val * (1-COEF);
    DEBUG_PRINT(new_val);
    DEBUG_PRINT(" ");

    old_val = new_val;
    return int(new_val);
}



///////////////////////////////////////////////////////////////////////////////
bool getTouch()
{
  int value = analogRead(cap_touch_pin);
  DEBUG_PRINT(value);
  DEBUG_PRINT(" ");

  return (value > 4) ; // Empirical threshold
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
  DEBUG_PRINT("\n");
}
