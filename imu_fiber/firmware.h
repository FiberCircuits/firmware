// Are we using the dev board or our custom circuit?
//#define MY_STM32C0116_DK


// UART ///////////////////////////////////////////////////////////////////////

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
  const int led = PA8;
  static bool LED_ON = HIGH;
#endif

