#include "event.h"
#include <stdlib.h>
#include <stdio.h>


String Event::toString(void)
{
    String t = String::format("%X,%X",_id,_start_date);


    uint32_t t2 =  strtol(t, nullptr, 16);
    return t;
}

bool Event::init(const uint32_t id, const uint32_t start_date)
{
    _id = id;
    _start_date = start_date;
}

bool Event::checkThisOne( const uint32_t id)
{
    return (_id == id);
}

uint16_t Event::getCountDown( void )
{
    const uint32_t ct = Time.now();

    return (uint16_t)( (ct - _start_date) / 60 / 60 / 24 );

}

bool Event::parseString(const String str)
{
    int i = 0;
    if(split(str, ",", _id, i) == false)
    {
        return false;
    }

    if(split(str, ",", _start_date, i) == false )
    {
        return false;
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
