#ifndef _EVENT_LIST_H_
#define _EVENT_LIST_H_

#include <stdlib.h>
#include "event.h"
#include "application.h"

#define EVENTS_LIST_SIZE     (20)

class EventsList
{
    private :
        //setup
        static const int number_of_events = 20;
    public :
        EventsList() {};

        void        init            (void  );

        String      toString        ( void                           );

        void        newIdEvent    ( const uint32_t id  ) ;

        void        eraseEvent      (const uint32_t id ) ;

        void        updateEvent     (String str);

        Event *     getCurrentEvent ( void ) { return _current_event; }

    private :

        Event _events_list[number_of_events];

        Event * _current_event;

        uint32_t _id;
        uint32_t _start_date;
        uint16_t _alarm[3];

};

#endif
