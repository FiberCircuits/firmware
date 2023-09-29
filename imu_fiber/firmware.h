// Cap sense ///////////////////////////////////////////////////////////////////

#define USE_CAP_SENSE


#ifdef USE_CAP_SENSE
  #include <ADCTouchSensor.h>

  #ifdef ADCTOUCH_INTERNAL_GROUNDING
    const int GROUNDED_PIN = -1;
  #else
    const int GROUNDED_PIN = PC15; // options: PC14/15 (or connect PA7/10/12)
  #endif
#endif


// UART ///////////////////////////////////////////////////////////////////////
//#define MY_STM32C0116_DK // using the dev board?

#ifdef MY_STM32C0116_DK
  const int _RX = PA10_R;
  const int _TX = PA9_R;
#else // custom circuit
  const int _TX = PA14;
  const int _RX = PA13;
#endif


// I2C ////////////////////////////////////////////////////////////////////////
const int _SDA = PB7;
const int _SCL = PB6;


// LED ////////////////////////////////////////////////////////////////////////

#ifdef MY_STM32C0116_DK
  // TODO: change pin: LED and SCL are on the same pin!
  const int led = PB6;
  static bool LED_ON = LOW; // inverted logic
#else // custom circuit
  const int led = PA8;      // TODO: confirm! (can also be PA9_R or PA11)
  static bool LED_ON = HIGH;
#endif

