// demo for RS - Bomb
// loovee 2014-6-8

#include <Wire.h>
#include <Streaming.h>
#include <TTSDisplay.h>
#include <TTSBuzzer.h>
#include <TTSLed.h>
#include <EEPROM.h>
#include <TTSLight.h>

#define DEMO_INFO   "demo for RS - Bomb\r\nDB Loovee & Jump \r\nhttps://github.com/reeedstudio/demo_rs_2014_Jun"

TTSDisplay display;

TTSBuzzer buz;

#define SPEED   200                                 // water speed


TTSLed led1(TTSLED1);                               // instantiate an object of led 1
TTSLed led2(TTSLED2);                               // instantiate an object of led 2
TTSLed led3(TTSLED3);                               // instantiate an object of led 3
TTSLed led4(TTSLED4);                               // instantiate an object of led 4

TTSLight light;


int which_wire = 1;

const int wire1 = 9;
const int wire2 = 10;
const int wire3 = 11;

const int pinVib = A5;

void wire_init()
{
    pinMode(wire1, OUTPUT);
    digitalWrite(wire1, HIGH);

    pinMode(wire2, OUTPUT);
    digitalWrite(wire2, HIGH);

    pinMode(wire3, OUTPUT);
    digitalWrite(wire3, HIGH);

    pinMode(pinVib, OUTPUT);
    digitalWrite(pinVib, LOW);
}


void setSeed()
{
    int seed = EEPROM.read(0);
    
    seed++;
    if(seed>200)seed = 0;
    
    EEPROM.write(0, seed);
    randomSeed(seed); 
}

void explode()
{
    led1.on();
    led4.off();

    buz.on();
    delay(SPEED);

    led2.on();
    led1.off();

    delay(SPEED);

    led3.on();
    led2.off();

    delay(SPEED);

    led4.on();
    led3.off();

    delay(SPEED);
}


void count_down_buzzer(int timeleft)
{
    if(timeleft>1000)
    {
        if(timeleft%100 == 0)
        {
            buz.on();
        }
        else if(timeleft%100 == 70)
        {
            buz.off();
        }
    }
    else
    {
        if(timeleft%100 == 99 || timeleft%100 == 66 || timeleft%100 == 33)
        {
            buz.on();
        }
        else if(timeleft%100 == 88 || timeleft%100 == 55 || timeleft%100 == 22)
        {
            buz.off();
        }
    }
}

// 1: connected
// 0: disconnect
bool wire_state(int pin)
{
    return digitalRead(pin);
}


// 0: nothing happen
// 1: boom clear
// 2: you dead
int bomb_rock_state()
{
    if(wire_state(wire1) && wire_state(wire2) && wire_state(wire3))return 0;

    switch(which_wire)
    {
        case 1:
        if(!wire_state(wire1) && wire_state(wire2) && wire_state(wire3))return 1;
        break;

        case 2:
        if(wire_state(wire1) && !wire_state(wire2) && wire_state(wire3))return 1;
        break;

        case 3:
        if(wire_state(wire1) && wire_state(wire2) && !wire_state(wire3))return 1;
        break;

        default:
        ;
    }

    return 2;
}

void count_down()
{

    buz.off();
    display.pointOn();
    for(int i = 3000; i>=0; i--)
    {
        display.num(i);
        delay(5);

        if(bomb_rock_state() == 1)
        {
            buz.off();
            while(1);
        }
        else if(bomb_rock_state() == 2)
        {
            buz.on();
            digitalWrite(pinVib, HIGH);
            while(1)
            {
                explode();
            }
        }
        count_down_buzzer(i);
    }

    display.pointOff();

    buz.on();
    digitalWrite(pinVib, HIGH);
    while(1)
    {
        explode();
    }
}



void setup()
{

    Serial.begin(115200);
    Serial.println(DEMO_INFO);
    setSeed();
    wire_init();
    which_wire = random(1, 4);
    
    delay(5000);
    buz.on();
    delay(100);
    buz.off();
    
    while(1)
    {
        int value = light.get();
        cout << "value = " << value << endl;
        
        if(value > 200)break;
        delay(100);
    }

}


void loop()
{
    count_down();

    delay(1000);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/