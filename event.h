#ifndef _EVENT_H_
#define _EVENT_H_

#include "application.h"

class Events
{
    public :
        Events() {};

        String toString(            void                            );
        bool parseString(           String str                      );

        bool checkThisOne(          uint16_t id                     );

        bool init(	                uint32_t id,
                                    uint32_t start_date = 0         );

        uint16_t getCountDown(      void                            );

    private :
    
        uint32_t _id;
        uint32_t _start_date;
        uint16_t _alarm[3];

};

#endif
