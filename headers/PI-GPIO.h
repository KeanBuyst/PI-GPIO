#pragma once

#include <thread>

#define Rad2Deg 57.29578f;
const float PI = 3.141592653589793f;

namespace GPIO {
    enum State {
        HIGH = 1u,
        LOW = 0u,
        OUTPUT = 1u,
        INPUT = 0u,
        FAIL = 2u
    };

    void SetMode(unsigned short pin, State state);
    void SetLevel(unsigned short pin, State state);
    void clear(unsigned short pin);
    void sleep(double seconds);
    State GetLevel(unsigned short pin);

    class PWM 
    {
        private:
        std::thread thread;
        unsigned short pin;
        bool running = true;
        bool wait = false;
        unsigned int frequency;
        float duty_cycle;

        void run();

        public:
        PWM(unsigned short pin,unsigned int frequency,float duty_cycle);
        ~PWM();
        
        void close();

        void setFrequency(unsigned int frequency);
        void setDutyCylce(float duty_cycle);
        void pause(bool pause);
    };
}