#include "event.h"
#include "crc.h"
#include <stdlib.h>
#include <stdio.h>


bool Event::init(const uint32_t id, const uint32_t end_date)
{
    _event_desc.id = id;
    _event_desc.end_date = end_date;
}

bool Event::isEqual( const uint32_t id)
{
    return (_event_desc.id == id);
}

uint16_t Event::getRemainingDays( void )
{
    const uint32_t ct = Time.now();

    if( _event_desc.end_date == 0 )
    {
        _status = EVENT_STATUS_NO_CONFIGURED;
        return 0;
    }

    if( _event_desc.end_date <= ct )
    {
        _status = EVENT_STATUS_END;
        return 0;
    }

    _status = EVENT_STATUS_IN_PROGRESS;
    return (uint16_t)( (ct - _event_desc.end_date) / 60 / 60 / 24 );

}

Event::eventStatus_t Event::getStatus( void )
{
    getRemainingDays(); // _status is updating in getRemainingDays method.
    return _status;
}

bool Event::checkAlarm(void)
{
    // If event is not in progress, alarm has no meaning.
    if(getStatus() != EVENT_STATUS_IN_PROGRESS)
    {
        return false;
    }

    const uint16_t rd = getRemainingDays();
    bool ret = false;
    for(int i=0; i < _num_of_alarms; i++)
    {
        if( ( _event_desc.alarms[i].remainingDays <= rd          ) &&
            ( _event_desc.alarms[i].status == ALARM_STATUS_ARMED ) )
        {
            _event_desc.alarms[i].status = ALARM_STATUS_RING;
        }

        if( _event_desc.alarms[i].status == ALARM_STATUS_RING )
        {
            ret = true;
        }
    }
    return ret;
}

void Event::resetAlarm(void)
{
    for(int i=0; i < _num_of_alarms; i++)
    {
        if( _event_desc.alarms[i].status == ALARM_STATUS_RING )
        {
            _event_desc.alarms[i].status = ALARM_STATUS_RESETED;
        }
    }
}

String Event::toString(void)
{
    String t = String::format("%X,%X",_event_desc.id,_event_desc.end_date);


    uint32_t t2 =  strtol(t, nullptr, 16);
    return t;
}

bool Event::parseString(const String str)
{
    int i = 0;
    if(split(str, ",", _event_desc.id, i) == false)
    {
        return false;
    }

    uint32_t end_date;
    if( split(str, ",", end_date, i) == false )
    {
        return false;
    }

    if(end_date != _event_desc.end_date)
    {
        _event_desc.end_date = end_date;
        // If end date was been updated, then re-armed alarms.
        for(int i=0; i < _num_of_alarms; i++)
        {
            _event_desc.alarms[i].status = ALARM_STATUS_ARMED;
        }
    }

    return true;
}

bool Event::split(          const String    s       ,
                            const String    delim   ,
                                  uint32_t &data    ,
                                  int      &index   )
{
    const int       i = s.indexOf(delim,index);
    if(i == -1)
    {
        return false;
    }
    data = (uint32_t)strtol(s.substring(index, i), nullptr,16);

    index = i + 1;

    return true;

}
