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

        bool        init            ( const uint32_t adress                 );

        String      toString        ( void                                  );

        bool        parseString     ( const String str                      );

        bool        isEqual         ( const uint32_t id                     );

        uint16_t    getRemainingDays( void                                  );

        eventStatus_t getStatus( void );

        bool        checkAlarm(void);
        void        resetAlarm(void);

        String debugStr;
        uint32_t debugInt;

    private :

        typedef enum
        {
            ALARM_STATUS_RESETED = 0,       // Alarm is off
            ALARM_STATUS_ARMED,             // Alarm is ready to ring
            ALARM_STATUS_RING               // Alarm is ringing

        }alarmStatus_t;

        typedef struct
        {
            alarmStatus_t   status;
            uint16_t        remainingDays;

        }alarmDesc_t;

        struct eventDesc
        {
            uint32_t      id;
            uint32_t      end_date;
            char          description[_length_of_description];
            alarmDesc_t   alarms[_num_of_alarms];
            uint16_t      crc;
        };
        eventDesc _event_desc;
        
        int             _adress;

        eventStatus_t   _status = EVENT_STATUS_NO_CONFIGURED;

        bool        split           ( const String    s       ,
                                      const String    delim   ,
                                            uint32_t &data    ,
                                            int      &index                 );


};

#endif
