#include "application.h"
#include "MFRC522.h"
#include "nixies_drv.h"
#include "pwm.h"
#include "rotary_coder.h"
#include "tags.h"
#include "eventList.h"
#include "led.h"
#include "timeout.h"



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

Led wink_led;

NixiesDriver nixies;

TimeOut timeOut;

rotaryCoder Rot1;
int rot1Prev;
rotaryCoder Rot2;

#define SYNC_TIME (10 * 60 * 1000) // synchronize time every 10min.
unsigned long lastSync = 0;
String debug_string;
String getCdeString;

void timeOutCallback(uint32_t dispValue)
{
    if(dispValue == 0)
    {
        nixies.SetBlink(1000,0); // off !
    }
    else if(dispValue == timeOut.Infinity())
    {
        nixies.SetBlink(250,0); // blink fast !
        nixies.DispValue(999);
    }
    else
    {
        nixies.SetBlink(500,50); // blink low !
        nixies.DispValue(dispValue);
    }


}

void tagCallback(uint32_t tag_id)
{
    current_pos_iot = tag_id;
    event_list.newIdEvent(tag_id);
    if(event_list.getCurrentEvent() != nullptr)
    {
        timeOut.newEvent(); // Reset timeout !

        switch(event_list.getCurrentEvent()->getStatus())
        {
            case Event::EVENT_STATUS_NO_CONFIGURED :
                nixies.DispValue(999);
                nixies.SetBlink(1000,50);
            break;
            case Event::EVENT_STATUS_IN_PROGRESS:
                event_list.getCurrentEvent()->resetAlarm();
                nixies.DispValue(event_list.getCurrentEvent()->getRemainingDays());
                nixies.SetBlink(1000,100);
            break;
            case Event::EVENT_STATUS_END:

                nixies.SetBlink(1000,50);
            break;

            default:
                nixies.SetBlink(1000,0);

        }
        //digitalWrite(D7, HIGH);
    }
    else
    {
        nixies.SetBlink(1000,0);
        //digitalWrite(D7, LOW);
    }
}

int setUpdateCdeCallback(String json)
{
    if(event_list.updateEvent(json + "&"))
    {
        tag.Refresh();
    }
    //debug_string = json + "&";
}

int setDeleteCdeCallback(String json)
{
  event_list.eraseEvent((uint32_t)strtol(json, nullptr,16));
  //debug_string = json + "&";
}

void setup() {

  pinMode(led2, OUTPUT);

  nixies.Setup();

  wink_led.Setup(D7);


  tag.Setup(&tagCallback);


  Rot1.init(A1,A2);
  Rot1.setMax(nixies.GetMaxBrightness()/50);
  Rot1.setVal(Rot1.getMax());
  Rot1.setMin(10);

  Rot2.init(A0,D1,true);
  timeOut.Setup(10000, 600000, &timeOutCallback);
  Rot2.setMin(timeOut.getPosMin());
  Rot2.setMax(timeOut.getPosMax());
  Rot2.setVal(timeOut.getPosDef());

  nixies.SetBlink(1000,0);

  event_list.init();
  wink_led.setWinks( 0 );

  Particle.variable("get_cur_tag", current_pos_iot);
  Particle.variable("debug", debug_string);
  Particle.variable("get_cde", getCdeString);
  Particle.function("update_cde", setUpdateCdeCallback);
  Particle.function("delete_cde", setDeleteCdeCallback);

}

void loop()
{

    if (millis() - lastSync > SYNC_TIME) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
    }

    //event.parseString("00000011\\12345\\1\\2\\3\\coucou&");
    //debug_string = event_list.toString();
    getCdeString = event_list.toString();
    //current_pos_iot = event.debugInt;

    //debug_string = event.toString();
    //current_pos_iot = event.getCountDown();

    if(Rot1.getVal() != rot1Prev)
    {
        timeOut.newEvent();
        rot1Prev = Rot1.getVal();
    }

    if(timeOut.getDispStatus())
    {
        nixies.SetBrightness(Rot1.getVal()*50);
    }
    else
    {
        nixies.SetBrightness(0); // off !
    }

    nixies.CyclTask();

    tag.CyclTask();
    wink_led.CyclTask();
    timeOut.CyclTask(Rot2.getVal());
    //debug_string = "pos=" + timeOut.getPos();

    wink_led.setWinks(event_list.getAlarmCounter());
}
