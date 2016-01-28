
#include "pwm.h"
#include "application.h"




void pwm::SetDutyCycle(const uint16_t duty)
{
  if(duty == _duty)
  {
      return; // nothing change !  
  }

  if(duty < _period )
  {
     _duty = duty;
  }
  else
  {
    _duty = _period;
  }

  TIM_OCInitStructure.TIM_Pulse = _duty;
  *_CCR = TIM_OCInitStructure.TIM_Pulse;

}

bool pwm::Setup(const int pin, const uint16_t period)
{
      _pin = pin;
      _period = period;
      /* Compute the pulse value to generate a PWM signal with variable duty cycle */
      _duty = _period; // Just set it to the period to start off, which should be 0 duty cycle effectively

      STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();

      if (PIN_MAP[_pin].timer_ch == TIM_Channel_1)
      {
          _CCR = &(PIN_MAP[_pin].timer_peripheral-> CCR1);
      }
      else if (PIN_MAP[_pin].timer_ch == TIM_Channel_2)
      {
          _CCR = &(PIN_MAP[_pin].timer_peripheral-> CCR2);
      }
      else if (PIN_MAP[_pin].timer_ch == TIM_Channel_3)
      {
          _CCR = &(PIN_MAP[_pin].timer_peripheral-> CCR3);
      }
      else if (PIN_MAP[_pin].timer_ch == TIM_Channel_4)
      {
          _CCR = &(PIN_MAP[_pin].timer_peripheral-> CCR4);
      }
      else
      {
          return false;
      }

      pinMode(_pin, OUTPUT);
      analogWrite(_pin, 0);

      /* GPIOB clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

      /* Initialise GPIO for PWM function */
      GPIO_InitTypeDef        GPIO_InitStructure;    //GPIO Initialisation Structure
      /* Initialize D0/PB7, Alternative Function, 100Mhz, Output, Push-pull*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
      GPIO_Init(GPIOB, &GPIO_InitStructure); // Note the GPIOB.
      // Consult https://docs.particle.io/datasheets/photon-datasheet/#pin-out-diagrams for pinout, pay attention to the STM32 Pin column, ie PB7
      // the 'B' in PB7 means you should use the 'B' GPIO, GPIOB.  PA4 would use GPIOA.

      // Map the pin to the timer
      GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM4);
      /* TIMER 4 clock enable */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

      /* Timer Base configuration */
      TIM_TimeBaseStructure.TIM_Prescaler = 0;
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3; // symmetrical PWM
      TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
      //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
      TIM_TimeBaseStructure.TIM_Period = _period;
      TIM_TimeBaseStructure.TIM_ClockDivision = 0;
      TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); // TIM4 = Timer 4

      //Initialise Output Compare Structure

      //pulse = (uint16_t) ((period-1)/(100.0/dutyCycle));

      /* Timer 4 Channel 2 PWM output configuration */
      TIM_OCStructInit(&TIM_OCInitStructure);
      TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; // can't find a decent reference to say whats the diff between PWM1 and PWM2
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCInitStructure.TIM_Pulse = _duty;
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

      // Channel 2 init timer 4
      TIM_OC2Init(TIM4, &TIM_OCInitStructure);
      TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
      //TIM_ARRPreloadConfig(TIM4, ENABLE); // don't seem to need this

      return true;
}
