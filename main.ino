#include "application.h"
#include "MFRC522.h"
#include "nixies_drv.h"
#include "pwm.h"
#include "rotary_coder.h"


int led2 = D7;


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




  nixies.SetBlink(1000,100);


}


void loop()
{

  nixies.DispValue(current_tag_iot);
  nixies.SetBrightness(nixies.GetMaxBrightness()/2);
  nixies.CyclTask();


  // RFID management !
  delay(10);
  if ( mfrc522.PICC_IsNewCardPresent())
  {
      digitalWrite(led2, HIGH);
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
      digitalWrite(led2, LOW);
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
