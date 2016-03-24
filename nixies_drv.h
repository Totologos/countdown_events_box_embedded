#ifndef _NIXIES_DRV_H_
#define _NIXIES_DRV_H_

#include "application.h"
#include "pwm.h"

// NIXIES pinout
#define NIXIES_DRV_POL      RX
#define NIXIES_DRV_BL       D0
#define NIXIES_DRV_LE       D6
#define NIXIES_DRV_DATA     D2
#define NIXIES_DRV_CLK      D4

#define NIXIES_MAX_BRIGHTNESS (2500)

class NixiesDriver
{
    public:
      NixiesDriver(void);

      // Must be call in setup() function in main file
      void      Setup();

      // Must be call in loop() function in main file as soon
      // as possible.
      void      CyclTask(void);

      // Define the Bightness.
      // The minium brightness value is 0 (display off)
      // and the maxium is the value return by GetMaxBrightness() (display max on)
      void      SetBrightness(const uint16_t value);

      // Return current value of brightness
      uint16_t  GetBrightness(void) { return _brightness; }

      // Return the maximum value of the brightness
      uint16_t  GetMaxBrightness(void) { return _dimmer.GetPeriod(); }

      // Used to blink the display.
      // periodeMs (in ms) is the total time
      // DutyCycle in % (between 0 and 100), 0 always off, 100 always on
      void      SetBlink(const uint32_t periodeMs, const uint32_t dutyCycle);

      // Disp value on display.
      // value must be in between 0 and 999.
      void      DispValue(const uint16_t value);



    private:



      uint16_t  _currentValueDisp;
      pwm       _dimmer;
      uint16_t  _brightness;

      unsigned long _prevTime;
      uint32_t      _currentTime;
      uint32_t      _durationOn;
      uint32_t      _durationTotal;

      uint32_t      _periodeMsprev = 0;
      uint32_t      _dutyCycleprev = 0;


      void      LoadShiftRegister(uint32_t value);

};

#endif
