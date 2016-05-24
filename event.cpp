#include "event.h"
#include "crc.h"
#include <stdlib.h>
#include <stdio.h>


bool Event::init(const int instance)
{
    _address = instance * sizeof(eventDesc_t);
    eventDesc_t tEvent;
    EEPROM.get(_address, tEvent);

    const uint16_t crc = tEvent.crc;
    tEvent.crc = _address;

    //debugInt = crc_run(sizeof(eventDesc_t), &tEvent);
    if( crc_run(sizeof(eventDesc_t), &tEvent) == crc)
    {
        _event_desc = tEvent;
    }
    else
    {
        memset(&_event_desc, 0, sizeof(eventDesc_t) );
    }

}

void Event::erase ( const uint32_t new_id             )
{
    memset(&_event_desc, 0, sizeof(eventDesc_t) );
    _event_desc.id = new_id;

    /////////////
    // Update eeprom
    _event_desc.crc = _address;
    _event_desc.crc = crc_run(sizeof(eventDesc_t), &_event_desc);
    EEPROM.put(_address, _event_desc);
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
    return (uint16_t)( (_event_desc.end_date - ct ) / 60 / 60 / 24 );

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
        if( ( _event_desc.alarms[i].remainingDays == rd          ) &&
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
    String t = String::format("%X\\%X\\",_event_desc.id,_event_desc.end_date);

    for(int i=0; i < _num_of_alarms; i++)
    {
        t += String::format("%X\\", _event_desc.alarms[i].remainingDays);
    }

    t += String::format("%s&",_event_desc.description);

    return t;
}

bool Event::parseString(const String str)
{
    int index = 0;

    /////////////
    // Parse Id
    uint32_t id;
    if(split(str, "\\", id, index) == false)
    {
        return false;
    }

    // check if input id is not null and
    // in case of current id is not null then, id must be equal to input id
    // in case of ccurent id is not, it is a empty event, so attibute input id
    // to this one.
    if( (id==0) || ((id != _event_desc.id) && _event_desc.id != 0 ) )
    {
        return false;
    }
    /////////////
    // Parse end date
    uint32_t end_date;
    if( split(str, "\\", end_date, index) == false )
    {
        return false;
    }

    /////////////
    // Parse alarms dates...
    uint32_t alarms[_num_of_alarms];
    for(int i=0; i < _num_of_alarms; i++)
    {
        if( split(str, "\\", alarms[i], index) == false )
        {
            return false;
        }
    }

    /////////////
    // Parse description
    String description;
    if( ( split(str, "&", description , index) == false) ||
        ( description.length() > (_length_of_description - 1) )  )
    {
        return false;
    }

    // For each alarm, check if values must be updated...
    // If yes, re-armed alarms.
    for(int i=0; i < _num_of_alarms; i++)
    {
        if( (end_date != _event_desc.end_date) ||
            (_event_desc.alarms[i].remainingDays != (uint16_t)alarms[i]))
        {
            _event_desc.alarms[i].status = ALARM_STATUS_ARMED;
        }
        _event_desc.alarms[i].remainingDays = (uint16_t)alarms[i];
    }

    // Update other values
    _event_desc.end_date = end_date;
    _event_desc.id = id;
    description.toCharArray(_event_desc.description, _length_of_description);

    /////////////
    // Update eeprom
    _event_desc.crc = _address;
    _event_desc.crc = crc_run(sizeof(eventDesc_t), &_event_desc);
    EEPROM.put(_address, _event_desc);

    return true;
}

bool Event::split(          const String     s       ,
                            const String     delim   ,
                                  String   & data    ,
                                  int      & index   )
{
    const int i = s.indexOf(delim,index); // try to find delimiter

    if(i == -1)
    {
        return false;
    }

    data = s.substring(index, i); // Extract data

    index = i + 1; // Update index

    return true;

}

bool Event::split(          const String     s       ,
                            const String     delim   ,
                                  uint32_t & data    ,
                                  int      & index   )
{
    String sdata;
    if(split(s,delim,sdata,index) == true)
    {
        // Convert hexadecimal string to uint32_t
        data = (uint32_t)strtoul(sdata, nullptr,16);
        return true;
    }
    return false;
}
