/*
 * demo for RS - Servo
 * db Loovee & Jump
 * 2014-6-8
 */

#include <Servo.h>
#include <Streaming.h>

#define START_ANGLE     0
#define STOP_ANGLE      150
#define SPEED           10          

#define TEMP_UP         45
#define TEPM_DOWN       20

#define DEMO_INFO   "demo for RS - Servo\r\nDB Loovee & Jumo \r\nhttps://github.com/reeedstudio/demo_rs_2014_Jun"

const int pinServo = A2;

const int pinTemp  = A0;

int pos = 0;
int pos_buf = 0;


Servo myservo;              // create servo object to control a servo


int temperature_get()
{
    int B = 3975;
    int val = analogRead(pinTemp);
    float resistance = (float)(1023.0-val)*10000.0/(float)val;
    float temp = 1/(log(resistance/10000)/B+1/298.15)-273.15;
    
    return temp;
}


void setPos()
{
    int temp = temperature_get();
    
    temp = temp>TEMP_UP ? TEMP_UP : temp;
    temp = temp<TEPM_DOWN ? TEPM_DOWN :temp;
    
    cout << temp << endl;
    pos = map(temp, TEPM_DOWN, TEMP_UP, START_ANGLE, STOP_ANGLE);
    
    if(pos != pos_buf)
    {
        pos_buf = pos;
        myservo.write(STOP_ANGLE - pos);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println(DEMO_INFO);
    myservo.attach(pinServo);  // attaches the servo on pin 9 to the servo object
    
}


void loop()
{

    setPos();
    delay(100);
}