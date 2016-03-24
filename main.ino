#include "application.h"
#include "MFRC522.h"
#include "nixies_drv.h"
#include "pwm.h"
#include "rotary_coder.h"


int led2 = D7;
volatile int pos = 200;
volatile int pos2 = 200;
volatile long timeIRQ = 0;

// RFID pinout
#define SS_PIN DAC
#define RST_PIN WKP

uint32_t last_tag = 0 ;
uint32_t current_tag = 0 ;
uint32_t current_tag_iot = 0;

uint32_t current_pos_iot = 100;


int retry = 3;

MFRC522 mfrc522;  // Create MFRC522 instance.


NixiesDriver nixies;

rotaryCoder Rot1;
rotaryCoder Rot2;

bool test = false;



void setup() {

  nixies.Setup();


  //Particle.variable("get_cur_tag", current_tag_iot);
  Particle.variable("get_cur_tag", current_pos_iot);
  // We are going to tell our device that D0 and D7 (which we named led1 and led2 respectively) are going to be output
  // (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)

  // It's important you do this here, inside the setup() function rather than outside it or in the loop function.

  mfrc522.Setup(SS_PIN, RST_PIN);
  SPI.begin();
	SPI.setClockSpeed(100000);

  mfrc522.PCD_Init();   // Init MFRC522 module
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("Firmware Version: 0x"));
  Serial.print(v, HEX);



  Rot1.init(A1,A2);
  Rot1.setMax(10);

  Rot2.init(A0,D1);
  Rot2.setMax(10);


  pinMode(led2, OUTPUT);

  //SPI.begin();
  //SPI.setClockSpeed(100000);
  //SPI.setDataMode(SPI_MODE0) ;


  nixies.SetBlink(1000,100);


}

// Next we have the loop function, the other essential part of a microcontroller program.
// This routine gets repeated over and over, as quickly as possible and as many times as possible, after the setup function is called.
// Note: Code that blocks for too long (like more than 5 seconds), can make weird things happen (like dropping the network connection).  The built-in delay function shown below safely interleaves required background activity, so arbitrarily long delays can safely be done if you need them.
static uint8_t counter = 0 ;
static int inc = 50;
//static const uint32_t leds[] = {0x00000001, 0x00000002, 0x00000004, 0x40000000, 0x80000000 };
static const uint32_t leds[] = {0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001 };

static int leftregister = 0;

void loop()
{

current_pos_iot = pos;
  //Serial.print(pos);
  //Serial.print("/");
  //Serial.println(pos2);
  //delay(250);

  //return;

  leftregister++;
  if(leftregister == 10)
  {
    leftregister = 0;
  }

  //nixies.LoadShiftRegister((1<<1) + (1<<(0+10)) + (1<<(6+20)));
  nixies.DispValue(333);
  nixies.SetBrightness(nixies.GetMaxBrightness()/2);
  nixies.CyclTask();

  //counter++;
  //if(counter == 5)
  //{
  //    counter = 0;
  //}



  // To blink the LED, first we'll turn it on...
  //delay(50);
  //digitalWrite(led2, HIGH);

  //SPI.transfer(13);
  //digitalWrite(D1, LOW);
  //SPI.transfer(buffer_tx, buffer_rx, 5, endSpiTransfer);
  //tone(D0,20000);
  // We'll leave it on for 1 second...
  //delay(50);
  //noTone(A0);
  //Serial.println("hello world!");
  //Serial.println(mfrc522.PCD_GetAntennaGain());

  //byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  //Serial.print(F("Firmware Version: 0x"));
  //Serial.print(v, HEX);

  // Nixies management !


 /*if( inc > 0)
 {
   if(nixies.GetBrightness() + inc > nixies.GetMaxBrightness())
   {
       inc = -50;
   }
 }
 else
 {
   if(nixies.GetBrightness() + inc <= 50)
   {
      inc = 50;
   }
 }
 nixies.SetBrightness(nixies.GetBrightness() + inc);
 */



  // RFID management !
  delay(10);
  if ( mfrc522.PICC_IsNewCardPresent())
  {
      //digitalWrite(led2, HIGH);
      if ( mfrc522.PICC_ReadCardSerial())
      {

          if(mfrc522.uid.size == sizeof(uint32_t)) // Compatible only
          {
              current_tag = *((uint32_t*)mfrc522.uid.uidByte);
              retry = 3;
          }
          //Serial.print(counter);
          //Serial.print("Tag");
          //Serial.println(current_tag);
      }
  }
  else
  {
      //digitalWrite(led2, LOW);
      retry--;
      if(retry == 0)
      {
          current_tag = 0 ;
          retry = 1;
      }
  }

  if(current_tag != last_tag)
  {
      if(current_tag == 0)
      {
          //Serial.println("Tag Lost!");
          current_tag_iot = 0 ;
      }
      else
      {
          //Serial.print("New Tag: ");
          //Serial.println(current_tag);
          current_tag_iot = current_tag;
      }
  }
  last_tag = current_tag;
  // And repeat!

}
