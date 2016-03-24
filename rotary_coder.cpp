#include "rotary_coder.h"

void rotaryCoder::init(const int pin_irq, const int pin_updw)
{
    _pin_updw = pin_updw;
    _max = 100;
    _min = 0;
    _val = 0;
    _timeIRQ = 0;
    pinMode(pin_updw, INPUT);
    pinMode(pin_irq, INPUT);
    attachInterrupt(pin_irq, &rotaryCoder::handler, this, FALLING);

}

void rotaryCoder::handler()
{
    long t = millis();
    if(t < _timeIRQ + 10)
    {
        return;
    }
    _timeIRQ = t;

    if(digitalRead(_pin_updw))
    {
        digitalWrite(D7, HIGH);
        if(_val < _max)
        {
            _val++;
        }
    }
    else
    {
        digitalWrite(D7, LOW);
        if(_val > _min)
        {
            _val--;
        }

    }
}
