#include "Components.h"

#include <iostream>
#include <math.h>

const float lenFemor = 80;
const float lenTibia = 90;

Components::Servo::Servo(unsigned short pin,unsigned short max) : max(max)
{
    pwm = new GPIO::PWM(pin,50,0);
};

Components::Servo::~Servo()
{
    delete pwm;
};

void Components::Servo::setAngle(float angle)
{
    // servo 100% at 12. 0% at 2. While at 50 hertz
    if (angle > max || angle < 0)
    {
        std::cout << "PI-GPIO: provided angle out of bounds\n";
        return;
    }
    float dc = (angle/max * 10) + 2;
    pwm->setDutyCylce(dc);
}

Components::K3::K3(Servo* s1, Servo* s2, Servo* s3)
{
    base = s1;
    femor = s2;
    tibia = s3;
}

void Components::K3::toPoint(float x, float y, float z)
{
    // z is depth (never negative)
    // x is left and right shift
    // y is up and down

    // x and z flipped because servo is upside down
    float pivot = atan2(z,x) * Rad2Deg + 90;
    base->setAngle(pivot);

    float R = sqrt(x * x + z * z); // get point R

    float diagnol = sqrt(R * R + y * y);
    // rule of cosine = ( c^2 = a^2 + b^2 - 2abcos(*) ) c is oppersite side
    float tibiaRot = acos((lenTibia * lenTibia + lenFemor * lenFemor - diagnol * diagnol) / ( 2 * lenTibia * lenFemor )) * Rad2Deg;
    tibia->setAngle(180 - tibiaRot);
    float femorRot = (acos((lenFemor * lenFemor + diagnol * diagnol - lenTibia * lenTibia) / (2 * lenFemor * diagnol)) + atan2(R,y)) * Rad2Deg + 90;
    femor->setAngle(180 - femorRot);
}