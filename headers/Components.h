#pragma once

#include <PI-GPIO.h>

namespace Components {
    class Servo
    {
        private:
        GPIO::PWM* pwm;
        unsigned short max;
        public:
        Servo(unsigned short pin,unsigned short max = 180);
        ~Servo();
        void setAngle(float angle);
    };

    class Kinematics 
    {
        public:
        virtual void toPoint(float x, float y, float z) = 0;
    };

    class K3 : Kinematics
    {
        public:
        K3(Servo* s1, Servo* s2, Servo* s3);

        void toPoint(float x, float y, float z);

        Servo* base;
        Servo* femor;
        Servo* tibia;
    };
}