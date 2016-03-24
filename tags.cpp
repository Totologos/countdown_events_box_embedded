#include "tags.h"
#include "application.h"

static MFRC522       _mfrc522;             // Tag reader

void tags::Setup(void)
{
    _mfrc522.Setup(SS_PIN, RST_PIN);
    SPI.begin();
    SPI.setClockSpeed(100000);

    _mfrc522.PCD_Init();   // Init MFRC522 module
    //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

    _retry = TAGS_NUM_OF_RETRY;
    _tagIsPresent = false;
    _current_tag = 0;

    _prevTime = 0;
    _currentTime = 0;
}

void tags::CyclTask(void)
{

  unsigned long time = millis();
  _currentTime += (time - _prevTime);
  _prevTime = time;

  if(_currentTime < 100)
  {
      return;
  }
  _currentTime = 0;

  if ( _mfrc522.PICC_IsNewCardPresent())
  {
      if ( _mfrc522.PICC_ReadCardSerial())
      {

          if(_mfrc522.uid.size == sizeof(uint32_t)) // Compatible only
          {
              _tagIsPresent = true;
              _current_tag = *((uint32_t*)_mfrc522.uid.uidByte);
              _retry = TAGS_NUM_OF_RETRY;
          }
      }
  }
  else
  {
      _retry--;
      if(_retry == 0)
      {
          _tagIsPresent = false;
          _retry = 1;
      }
  }
}
