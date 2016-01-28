#ifndef _PMW_H_
#define _PMW_H_

#include "application.h"

class pwm {
  public :

      pwm() { }

      // Must be call in the setup() function in the main file
      // pin where the PWM in generated
      // period is the resolution of PWM but also the invert of frequency.
      // Fpwm = 1/period = FsysClk / 4 / period.
      // max value of period is 65535 (16 bits timer)
      // PWM used timer 4 !
      bool Setup(const int pin, const uint16_t period);

      // Update de duty cycle. duty value must be in between
      // 0 and value returned by GetPeriod();
      void SetDutyCycle(const uint16_t duty);

      // Return period
      int GetPeriod(void)     { return _period;     }

      // Return Duty
      int GetDuty(void)       { return _duty;       }


  private :

    int _pin;
    int _period;
    int _duty;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //Time base structure for timer 3
    NVIC_InitTypeDef        NVIC_InitStructure;    //Nested Vector Interrupt Controller Initialisation Structure
    TIM_OCInitTypeDef       TIM_OCInitStructure;   //Output compare init structure for PWM

    volatile uint32_t * _CCR;

};

#endif
