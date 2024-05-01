#include <PI-GPIO.h>
#include <Components.h>
#include <math.h>

#include <iostream>

using namespace GPIO;
using namespace Components;

void move(K3& leg,float min,float max,float speed);

int main()
{
    Servo base(18);
    Servo femor(23);
    Servo tibia(24);

    K3 leg(&base,&femor,&tibia);

    move(leg,-100,100,2);

    sleep(2);
}

void move(K3& leg,float min,float max,float time)
{
    // speed = mm/s
    const float smoothness = 6;

    float distance = max - min;
    float speed = (distance / time) / smoothness;
    for (float pos = min; pos <= max; pos += speed)
    {
        leg.toPoint(pos,0,80);
        sleep(1 / smoothness);
    }
}