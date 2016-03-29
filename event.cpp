#include "event.h"
#include <stdlib.h>
#include <stdio.h>


String Event::toString(void)
{
    String t = String::format("%08X,%08X",_id,_start_date);


    uint32_t t2 =  strtol(t, nullptr, 16);
    return t;
}

bool Event::init(uint32_t id, uint32_t start_date)
{
    _id = id;
    _start_date = start_date;
}

bool Event::checkThisOne( uint32_t id)
{
    return (_id == id);
}

uint16_t Event::getCountDown( void )
{
    const uint32_t ct = Time.now();


    return (uint16_t)( (ct - _start_date) /60 / 60 / 24 );
}
