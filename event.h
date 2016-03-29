#ifndef _EVENT_H_
#define _EVENT_H_

#include "application.h"

class Event
{
    public :
        Event() {};

        String toString(            void                            );
        bool parseString(           String str                      );

        bool checkThisOne(          uint32_t id                     );

        bool init(	                uint32_t id,
                                    uint32_t start_date = 0         );

        uint16_t getCountDown(      void                            );

        String debugStr;
        uint32_t debugInt;
    private :

        uint32_t _id;
        uint32_t _start_date;
        uint16_t _alarm[3];
        bool    split(              const String    s       ,
                                    const String    delim   ,
                                          uint32_t &data    ,
                                          int      &index   );


};

#endif
