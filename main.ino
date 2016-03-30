#include "application.h"
#include "MFRC522.h"
#include "nixies_drv.h"
#include "pwm.h"
#include "rotary_coder.h"
#include "tags.h"
#include "eventList.h"



int led2 = D7;


// RFID pinout
#define SS_PIN DAC
#define RST_PIN WKP

uint32_t last_tag = 0 ;
uint32_t current_tag = 0 ;
uint32_t current_tag_iot = 0;

uint32_t current_pos_iot = 100;


EventsList event_list;

tags tag;


NixiesDriver nixies;

rotaryCoder Rot1;
rotaryCoder Rot2;

#define SYNC_TIME (10 * 60 * 1000) // synchronize time every 10min.
unsigned long lastSync = 0;
String debug_string;


void callback(uint32_t tag_id)
{
    current_pos_iot = tag_id;
    event_list.newIdEvent(tag_id);
    if(event_list.getCurrentEvent() != nullptr)
    {
        nixies.DispValue(event_list.getCurrentEvent()->getRemainingDays());
        digitalWrite(D7, HIGH);
        //try to find id in list event
        // if found, update display...

        // if not found, create a new event
    }
    else
    {
        digitalWrite(D7, LOW);
    }
}

void setup() {

  pinMode(led2, OUTPUT);

  nixies.Setup();

  //Particle.variable("get_cur_tag", current_tag_iot);
  Particle.variable("get_cur_tag", current_pos_iot);
  Particle.variable("debug", debug_string);

  tag.Setup(&callback);


  Rot1.init(A1,A2);
  Rot1.setMax(nixies.GetMaxBrightness()/10);
  Rot1.setVal(Rot1.getMax()/2);

  Rot2.init(A0,D1);
  Rot2.setMax(10);

  nixies.SetBlink(1000,0);

  event_list.init();



}

void loop()
{

    if (millis() - lastSync > SYNC_TIME) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
        digitalWrite(D7, HIGH);

    }
    else
    {
        //event.init(15,Time.now() - (10 * 24 * 60 * 60));
        String updateStr = String::format("F641850\\%X\\0\\0\\0\\Happy bithday&",
            Time.now() + (10 * 24 * 60 * 60) );
        //event_list.updateEvent(updateStr);
    }

    //event.parseString("00000011\\12345\\1\\2\\3\\coucou&");
    debug_string = event_list.toString();
    //current_pos_iot = event.debugInt;

    //debug_string = event.toString();
    //current_pos_iot = event.getCountDown();

    if(tag.GetTagIsPresent())
    {
        //digitalWrite(D7, HIGH);
        nixies.SetBlink(1000,100);

        //nixies.DispValue(tag.GetTagId()&0xFF);
    }
    else
    {
        //digitalWrite(D7, LOW);
        nixies.SetBlink(1000,0);
    }

    nixies.SetBrightness(Rot1.getVal()*10);
    nixies.CyclTask();

    tag.CyclTask();
}
