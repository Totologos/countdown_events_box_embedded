#ifndef _EVENT_H_
#define _EVENT_H_

#include "application.h"



class Event
{
    private :
        // Setup
        static const int       _num_of_alarms = 3;
        static const int       _length_of_description = 32;
    public :

    typedef enum
    {
        EVENT_STATUS_NO_CONFIGURED     = 0x1000,
        EVENT_STATUS_IN_PROGRESS       = 0x2000,
        EVENT_STATUS_END               = 0x3000
    }eventStatus_t;

                    Event           () {};

        bool        init            ( const uint16_t adress                 );

        String      toString        ( void                                  );

        bool        parseString     ( const String str                      );

        void        erase           ( const uint32_t new_id = 0             );
        

        bool        isEqual         ( const uint32_t id                     );

        uint16_t    getRemainingDays( void                                  );

        eventStatus_t getStatus( void );

        bool        checkAlarm(void);
        void        resetAlarm(void);

        String debugStr;
        uint32_t debugInt;

    private :

        enum alarmStatus_t
        {
            ALARM_STATUS_RESETED = 0,       // Alarm is off
            ALARM_STATUS_ARMED,             // Alarm is ready to ring
            ALARM_STATUS_RING               // Alarm is ringing

        };

        struct alarmDesc_t
        {
            alarmStatus_t   status;
            uint16_t        remainingDays;

        };

        struct eventDesc_t
        {
            uint32_t      id;
            uint32_t      end_date;
            char          description[_length_of_description];
            alarmDesc_t   alarms[_num_of_alarms];
            uint16_t      crc;
        };
        eventDesc_t _event_desc;

        uint16_t             _address;

        eventStatus_t   _status = EVENT_STATUS_NO_CONFIGURED;

        bool        split           ( const String    s       ,
                                      const String    delim   ,
                                            uint32_t &data    ,
                                            int      &index   );

        bool        split(          const String    s       ,
                                    const String    delim   ,
                                          String   &data    ,
                                          int      &index   );


};

#endif
