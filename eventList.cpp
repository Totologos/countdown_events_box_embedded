#include "eventList.h"


void EventsList::init(void)
{
    for(int i=0; i< number_of_events; i++)
    {
        _events_list[i].init(i);
    }
}

String EventsList::toString(void)
{
    const uint32_t currentId = (_current_event != nullptr) ?
                                _current_event->getId() : 0 ;
    String str = String::format("{%X&",currentId);
    for(int i=0; i< number_of_events; i++)
    {
        if(_events_list[i].getId() != 0)
        {
            str += _events_list[i].toString();
        }
    }

    str += "}";

    return str;
}

void   EventsList::newIdEvent    ( const uint32_t id  )
{
    if(id == 0)
    {
        _current_event = nullptr;
        return;
    }

    int firstEmpty = -1;
    for(int i=0; i< number_of_events; i++)
    {
        const uint32_t c_id = _events_list[i].getId();
        if( firstEmpty == -1 && c_id == 0)
        {
            firstEmpty = i;
        }

        if(c_id == id)
        {
            _current_event = &(_events_list[i]);
            return;
        }
    }

    if(firstEmpty != -1)
    {
        _events_list[firstEmpty].erase(id);
        _current_event = &(_events_list[firstEmpty]);
        return;
    }

    _current_event = nullptr;
}

void   EventsList::eraseEvent    ( const uint32_t id  )
{
    const uint32_t currentId = (_current_event != nullptr) ?
                                _current_event->getId() : 0 ;
    if(id == 0 || id == currentId)
    {
        return;
    }

    for(int i=0; i< number_of_events; i++)
    {
        if(id == _events_list[i].getId())
        {
            _events_list[i].erase();
            return;
        }
    }

}

void    EventsList::updateEvent     (String str)
{
    for(int i=0; i< number_of_events; i++)
    {
        if(_events_list[i].parseString(str) == true)
        {
            return;
        }
    }
}
