#ifndef _EVENT_LIST_H_
#define _EVENT_LIST_H_

#include <stdlib.h>
#include "event.h"
#include "application.h"

#define EVENTS_LIST_SIZE     (20)

class eventsList
{
    public :
        eventsList() {};

        String toString(            void                            );
        bool parseString(           String str                      );

        bool checkThisOne(          uint16_t id                     );

        bool init(	                uint32_t id,
                                    uint32_t start_date = 0         );

        uint16_t getCountDown(      void                            );

    private :

        std::vector<event> _event_list(EVENTS_LIST_SIZE);


        uint32_t _id;
        uint32_t _start_date;
        uint16_t _alarm[3];

};

#endif
