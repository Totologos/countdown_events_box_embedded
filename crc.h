#ifndef _CRC_H_
#define _CRC_H_

#include "application.h"

class Crc
{
    public :
        Crc() {};

        static uint16_t     run ( const uint32_t       buffer_size,
                                  const void *         buffer);

    private :
        static bool         _tabccitt_init;
        static uint16_t     _tabccitt [256];
        static uint16_t     updateCrc( uint16_t crc, uint8_t c);
        static void         init( void );
};

#endif
