// ir remote car with shield bot
// 


#include <Shieldbot.h>
#include <IRSendRev.h>
#include <Streaming.h>

Shieldbot shieldbot = Shieldbot();

#define beepon()        digitalWrite(3, HIGH)
#define beepoff()       digitalWrite(3, LOW)

void setup()
{

    Serial.begin(115200);
    shieldbot.setMaxSpeed(255);//255 is max

    pinMode(3, OUTPUT);
    IR.Init(A5);
    cout << "hello world" << endl;

}

unsigned char dta[20];

long timer_stop = 0;

void loop()
{
    if(millis()-timer_stop > 1000)  // stop
    {
        timer_stop = millis();
        shieldbot.drive(0, 0);
        shieldbot.fastStop();
    }
    
    if(IR.IsDta())
    {
        IR.Recv(dta);

        if(dta[0] != 9 || dta[6] != dta[7] || dta[8] != dta[9])
        {
            cout << "get data err" << endl;
        }
        else
        {
            timer_stop = millis();
            if(dta[6] == 100 && dta[8] == 100)          // beep
            {
            
                cout << "beep" << endl;
                for(int i=0; i<2; i++)
                {
                    beepon();
                    delay(200);
                    beepoff();
                    delay(200);
                }

                if(IR.IsDta())IR.Recv(dta);
            }

            else
            {
                if(dta[6] == dta[8])
                {
                    if(dta[6] <= 127)
                    {
                        dta[6] = dta[6]*0.8;
                    }
                    else
                    {
                        dta[6] = 255-dta[6];
                        dta[6] = 256-dta[6]*0.8;
                    }
                }

                cout << "get data" << endl;
                for(int i=0; i<4; i++)
                {
                    cout << dta[6+i] << '\t';
                }
                cout << endl;

                shieldbot.drive(dta[6], dta[8]);

                if(dta[6] == 0 && dta[8] == 0)
                {
                    shieldbot.fastStop();
                }
            }

        }
        cout << endl;
    }
}

