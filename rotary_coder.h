#ifndef _ROTARY_CODER_H_
#define _ROTARY_CODER_H_

#include "application.h"

class rotaryCoder {
  public :
    rotaryCoder() { }

    void init(const int pin_irq, const int pin_updw);

    void setMin(int min) { _min = min; }
    void setMax(int max) { _max = max; }
    void setVal(int val) { _val = val; }

    int getMin(void) { return _min; }
    int getMax(void) { return _max; }
    int getVal(void) { return _val; }


  private:
     int _min;
     int _max;
     volatile int _val;

     int _pin_updw;

     volatile long  _timeIRQ;

     void handler();


};

#endif
