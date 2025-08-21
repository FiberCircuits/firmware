// UART ///////////////////////////////////////////////////////////////////////

// #define DEBUG

#ifdef DEBUG
  #define DEBUG_setRx(...)     Serial.setRx(__VA_ARGS__)
  #define DEBUG_setTx(...)     Serial.setTx(__VA_ARGS__)
  #define DEBUG_BEGIN(...)     Serial.begin(__VA_ARGS__)
  #define DEBUG_PRINT(...)     Serial.print(__VA_ARGS__)

  #warning "!!! LED STRIP AND SERIAL TX ARE ON THE SAME PIN !!!"
#else
  #define DEBUG_setRx(...)
  #define DEBUG_setTx(...)
  #define DEBUG_BEGIN(...)
  #define DEBUG_PRINT(...)

  #define USING_LED_STRIP
#endif


//#define MY_STM32C0116_DK // using the dev board?

#ifdef MY_STM32C0116_DK
  const int _RX = PA10_R;
  const int _TX = PA9_R;
#else // custom circuit
  const int _TX = PA14; // warning: this is also the LED Din pin!
  const int _RX = PA13;
#endif


// I2C ////////////////////////////////////////////////////////////////////////

const int _SDA = PB7;
const int _SCL = PB6;


// LED ////////////////////////////////////////////////////////////////////////

#ifdef MY_STM32C0116_DK
  const int led = PA7;      // beside GND
  static bool LED_ON = LOW; // inverted logic
#else // custom circuit
  const int led = PA11;
  static bool LED_ON = HIGH;
#endif

