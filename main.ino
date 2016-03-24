#include "application.h"
#include "MFRC522.h"
#include "nixies_drv.h"
#include "pwm.h"
#include "rotary_coder.h"
#include "tags.h"


int led2 = D7;


// RFID pinout
#define SS_PIN DAC
#define RST_PIN WKP

uint32_t last_tag = 0 ;
uint32_t current_tag = 0 ;
uint32_t current_tag_iot = 0;

uint32_t current_pos_iot = 100;




tags tag;


NixiesDriver nixies;

rotaryCoder Rot1;
rotaryCoder Rot2;


void setup() {

  pinMode(led2, OUTPUT);

  nixies.Setup();

  //Particle.variable("get_cur_tag", current_tag_iot);
  Particle.variable("get_cur_tag", current_pos_iot);

  tag.Setup();


  Rot1.init(A1,A2);
  Rot1.setMax(nixies.GetMaxBrightness()/10);
  Rot1.setVal(Rot1.getMax()/2);

  Rot2.init(A0,D1);
  Rot2.setMax(10);

  nixies.SetBlink(1000,0);

}


void loop()
{

  if(tag.GetTagIsPresent())
  {
      digitalWrite(D7, HIGH);
      nixies.SetBlink(1000,100);
      nixies.DispValue(tag.GetTagId()&0xFF);
  }
  else
  {
      digitalWrite(D7, LOW);
      nixies.SetBlink(1000,0);
  }

  nixies.SetBrightness(Rot1.getVal()*10);
  nixies.CyclTask();

  tag.CyclTask();
}
