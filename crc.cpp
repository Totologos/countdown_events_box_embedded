#include "crc.h"

/* The CRC's are computed using polynomials. The  coefficients
 *  for the algorithms are defined by the following constants.
 */
#define P_CCITT    ( 0x1021 )

uint16_t Crc::run (  const uint32_t       buffer_size,
                     const void *         buffer)
{


    if (( buffer == NULL ) || ( buffer_size == 0 ))
    {
        return ( 0 );
    }
    
    if(_tabccitt_init == false)
    {
        Crc::init();
    }

    uint16_t t_crc = 0x0000;
    uint32_t i;
    for ( i = 0; i < buffer_size; i++ )
    {
        t_crc = Crc::updateCrc ( t_crc, ((uint8_t*)buffer ) [i] );
    }

    return ( t_crc );


}

void Crc::init( void )
{
    uint32_t i, j;
    uint16_t crc, c;

    for ( i = 0; i < 256; i++ )
    {
        crc = 0;
        c   = ((uint16_t)i ) << 8;

        for ( j = 0; j < 8; j++ )
        {
            if (( crc ^ c ) & 0x8000 )
            {
                crc = ( crc << 1 ) ^ P_CCITT;
            }
            else
            {
                crc = crc << 1;
            }

            c = c << 1;
        }

        _tabccitt [i] = crc;
    }

    _tabccitt_init = true;
}

/**
 * \brief  The function sys_update_crc_ccitt calculates  a  new  CRC-CCITT
 *         value  based  on the previous value of the CRC and the next
 *         byte of the data to be checked.
 *
 * \param crc : original crc
 * \param c : input byte to encode
 *
 * \return new crc
 */
uint16_t Crc::updateCrc      ( uint16_t crc,
                               uint8_t  c )
{
    uint16_t tmp, short_c;

    short_c = 0x00ff & (uint16_t)c;

    tmp = ( crc >> 8 ) ^ short_c;
    crc = ( crc << 8 ) ^ _tabccitt [tmp];

    return ( crc );
}
