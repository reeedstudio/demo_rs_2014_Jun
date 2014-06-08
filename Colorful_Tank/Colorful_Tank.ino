// demo of ....
// loovee 2013 - 10- 14

#include <Streaming.h>
#include <ChainableLED.h>

#define DEMO_INFO   "demo for RS - Colorful Tank\r\nDB Loovee & Jump \r\nhttps://github.com/reeedstudio/demo_rs_2014_Jun"

#define NUM_LEDS  6


#define SPEED   10


ChainableLED leds(7, 8, NUM_LEDS);


void setColor(int bit, int r, int g, int b)
{
    leds.setColorRGB(bit, r, g, b);  
}


void setColorAll(int r, int g, int b)
{
    for(int i=0; i<NUM_LEDS; i++)
    {
        leds.setColorRGB(i, r, g, b);
    }
}


void setColorIn(int r, int g, int b)
{   
    leds.setColorRGB(1, r, g, b);
    leds.setColorRGB(3, r, g, b);
    leds.setColorRGB(5, r, g, b);
}


void setColorOut(int r, int g, int b)
{
    leds.setColorRGB(0, r, g, b);
    leds.setColorRGB(2, r, g, b);
    leds.setColorRGB(4, r, g, b);
}


void setup()
{
    Serial.begin(115200);
    Serial.println(DEMO_INFO);

    setColorAll(255, 255, 0);
    
    for(int i=0; i<=255; i+=1)
    {
        setColorAll(i, 0, 0);
        delay(SPEED);
    }
}

void loop()
{
    for(int i=0; i<=255; i+=1)
    {
        setColorAll(255-i, i, 0);
        delay(SPEED);
    }
    
    for(int i=0; i<=255; i+=1)
    {
        setColorAll(0, 255-i, i);
        delay(SPEED);
    }
    
    for(int i=0; i<=255; i+=1)
    {
        setColorAll(i, 0, 255-i);
        delay(SPEED);
    }

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
