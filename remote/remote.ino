/*
  remote_lvc.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-3-18

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/
#include <Streaming.h>
#include <IRSendRev.h>

#define MAXSPEED        80
#define MAXSPEEDN       -80

#define VAL1MIN      -265
#define VAL1MAX       262
#define VAL2MIN      -270
#define VAL2MAX       260

/*

STATIC1 = 511
STATIC2 = 516

MAX1 = 773
MIN1 = 249

MAX2 = 775
MIN2 = 246

*/

#define START1      0x53
#define START2      0x01
#define END1        0x2f
#define END2        0x45
#define DTALEN      6

#define DIRFF       0
#define DEIRR       1

#define __DEBUG     1

#if __DEBUG
#define __print(X)        Serial.print(X)
#define __println(X)    Serial.println(X)
#else
#define __print(X)
#define __println(X)
#endif


const int pinSensor1 = A2;
const int pinSensor2 = A1;


int speedLeft  = 0;
int speedRight = 0;

int val1    = 0;
int valbuf1 = 0;
int val2    = 0;
int valbuf2 = 0;

int comVal1 = 0;
int comVal2 = 0;


unsigned char dta_ir[] = {9, 90, 91, 11, 31, 4, 0, 0, 0, 0};                // ir data

void testVal()
{
    int max1 = 0;
    int max2 = 0;
    int min1 = 1023;
    int min2 = 1023;
    
    comVal1 = getAnalog(pinSensor1);
    comVal2 = getAnalog(pinSensor2);
    
    
    cout << "comVal1 = " << comVal1 << endl;
    cout << "comVal2 = " << comVal2 << endl;
    
    while(1)
    {
        int val = getAnalog(pinSensor1);
        
        if(val>max1)
        {
            max1 = val;
            cout << "MAX1 = " << max1 << endl;
        }
        else if(val<min1)
        {
            min1 = val;
            cout << "MIN = " << min1 << endl;
        }
        
        val = getAnalog(pinSensor2);
        
        
        if(val>max2)
        {
            max2 = val;
            cout << "MAX2 = " << max2 << endl;
        }
        else if(val<min2)
        {
            min2 = val;
            cout << "MIN2 = " << min2 << endl;
        }
        
    }
}

void set_speed(unsigned char speedLeft, unsigned char speedRight)
{
    dta_ir[6] = speedLeft;
    dta_ir[7] = dta_ir[6];

    dta_ir[8] = speedRight;
    dta_ir[9] = dta_ir[8];

    IR.Send(dta_ir, 38);
}


/*********************************************************************************************************
** Function name:           getAnalog
** Descriptions:            read analog value
*********************************************************************************************************/
int getAnalog(int pin)
{
    unsigned int sum = 0;
    for(int i = 0; i<64; i++)
    {
        sum += analogRead(pin);
    }
    sum = ((sum >> 6) & 0x03ff);        // diff

    return sum;
}

/*********************************************************************************************************
** Function name:           cmpLvc
** Descriptions:            compare
*********************************************************************************************************/
bool cmpLvc(int a, int b)
{
    int tmp = a - b;
    return abs(tmp) > 1 ? 1 : 0;
}

/*********************************************************************************************************
** Function name:           makeDirSpeed
** Descriptions:            make direction and speed
*********************************************************************************************************/
void makeDirSpeed(int x, int y)
{

    y++;
    cout << "x = " << x << "\ty = " << y << endl;
    
    if(y == 0)                                                // go ahead or go back
    {

        speedLeft  = abs(x);
        speedRight = speedLeft;
        if(x >= 0)
        {
            __println("forward");
        }
        else
        {
            speedLeft = 0x80 | speedLeft;
            speedRight = speedLeft;
            __println("goback");
        }

    }
    else if(x == 0)
    {
        //y_abs = abs(y);
        speedLeft  = map(abs(y), 0, 100, 0, 50);
        speedRight = speedLeft;

        if(y > 0)        // right-rev
        {
            cout << "right" << endl;
            speedRight  = speedRight | 0x80;
        }
        else            // left-rev
        {
            cout << "left" << endl;
            speedLeft  = speedLeft | 0x80;
        }
    }
    else if(x >= 0 && y<= 0 && abs(x) >= abs(y))            // ahead-left
    {
        __println("forward-left");
        speedLeft   = x+y;
        speedRight  = x;
    }
    else if(x >= 0 && y <= 0 && abs(x) < abs(y))            // left-reverse
    {
        __println("left-rev");
        speedLeft  = 100 - min(abs(x), abs(y));

        //speedLeft  = map(speedLeft, 0, MAXSPEED, 0, MAXSPEEDTURN);
        speedRight = speedLeft;
        speedLeft  = speedLeft | 0x80;
    }
    else if(x >=0 && y >= 0 && abs(x) >= abs(y))            // ahead-right
    {
        __println("forward-right");
        speedLeft   = x;
        speedRight  = x-y;

    }
    else if(x >= 0 && y >= 0 && abs(x) < abs(y))            // right-reverse
    {
        __println("right-rev");
        speedLeft   = 100 - min(abs(x), abs(y));
        //speedLeft  = map(speedLeft, 0, MAXSPEED, 0, MAXSPEEDTURN);
        speedRight  = speedLeft;
        speedRight  = speedRight | 0x80;

    }
    else if(x <= 0 && y <= 0 && abs(x) > abs(y))            // back-left
    {
        speedRight = 0x80 + abs(x);
        speedLeft  = 0x80 + abs(x) - abs(y);
        __println("back-left");
    }
    else if(x <= 0 && y >=0 && abs(x) > abs(y))
    {
        speedLeft  = 0x80 + abs(x);
        speedRight = 0x80 + abs(x) - abs(y);
        __println("back-right");
    }
    else return ;

   // __print("right speed: ");
   // __println(speedRight);
   // __print("left  speed: ");
   // __println(speedLeft);
   
   
    // sendToRfbee();

    set_speed(speedLeft, speedRight);
}

/*********************************************************************************************************
** Function name:           setup
** Descriptions:            setup
*********************************************************************************************************/
void setup()
{
    Serial.begin(115200);
    
    
    // testVal();
    
    comVal1 = getAnalog(pinSensor1);
    comVal2 = getAnalog(pinSensor2);
    
    cout << "comVal1 = " << comVal1 << endl;
    cout << "comVal2 = " << comVal2 << endl;
    __println("init over");
}


/*********************************************************************************************************
** Function name:           loop
** Descriptions:            loop
*********************************************************************************************************/
void loop()
{

    val1 = getAnalog(pinSensor1) - comVal1;
    val2 = getAnalog(pinSensor2) - comVal2;
    
    //cout << val1 << '\t' << val2 << endl;
    
    if(getAnalog(pinSensor2) > 1000)
    {
        cout << "beep on" << endl;
        set_speed(100, 100);
        delay(100);
        while(getAnalog(pinSensor2)>1000);
    }
    else
    {

        val1 = constrain(val1, VAL1MIN, VAL1MAX);
        val2 = constrain(val2, VAL2MIN, VAL2MAX);

        val1 = map(val1, VAL1MIN, VAL1MAX, MAXSPEED, MAXSPEEDN);
        val2 = map(val2, VAL2MIN, VAL2MAX, MAXSPEED, MAXSPEEDN);

        if(val1 != valbuf1 || val2 != valbuf2)
        {

            
            makeDirSpeed(val1, val2);
           // __println();
        }
    }
    valbuf1 = val1;
    valbuf2 = val2;

    delay(10);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/