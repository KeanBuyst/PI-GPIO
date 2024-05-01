#include "PI-GPIO.h"
#include <iostream>
#include <fstream>

#include <chrono>
#include <functional>

std::string format(char instruction, unsigned short pin, GPIO::State state);
void write(std::string data);

void GPIO::SetMode(unsigned short pin, State state)
{
    // g <pin> <input/output>
    std::string buffer = format('g',pin,state);
    write(buffer);
}

void GPIO::SetLevel(unsigned short pin, State state)
{
    std::string buffer = format('o',pin,state);
    write(buffer);
}

void GPIO::sleep(double seconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds((long long) (seconds * 1000000)));
}

void GPIO::clear(unsigned short pin)
{
    SetLevel(pin,State::LOW);
    SetMode(pin,State::INPUT);
}

GPIO::State GPIO::GetLevel(unsigned short pin)
{
    std::string buffer = format('l',pin,State::LOW);
    write(buffer);

    std::ifstream in("/proc/gpio");
    if (!in.is_open())
    {
        std::cout << "PI-GPIO: failed to read from gpio driver\n";
        return GPIO::State::FAIL;
    }
    char byte;
    in.read(&byte,1);
    in.close();
    if (byte == '1')
        return GPIO::State::HIGH;
    else
        return GPIO::State::LOW;
}

std::string format(char instruction, unsigned short pin, GPIO::State state){
    char buffer[6];
    sprintf(buffer,"%c %u %u",instruction,pin,state);
    return std::string(buffer);
}

void write(std::string data)
{
    std::ofstream out("/proc/gpio");
    if (!out.is_open())
    {
        std::cout << "PI-GPIO: failed to write to gpio driver\n";
        return;
    }
    out << data;
    out.close();
}

GPIO::PWM::PWM(unsigned short pin,unsigned int frequency,float duty_cycle) : pin(pin),frequency(frequency),duty_cycle(duty_cycle), thread(std::bind(&GPIO::PWM::run, this))
{
    SetMode(pin,State::OUTPUT);
}

GPIO::PWM::~PWM()
{
    running = false;
    std::cout << "Closing PWM thread " << thread.get_id() << std::endl;
    thread.join();
    clear(pin);
}

void GPIO::PWM::setFrequency(unsigned int frequency)
{
    this->frequency = frequency;
}

void GPIO::PWM::setDutyCylce(float duty_cycle)
{
    this->duty_cycle = duty_cycle;
}

void GPIO::PWM::pause(bool pause)
{
    this->wait = pause;
}

void GPIO::PWM::run()
{
    while (running)
    {
        if (wait) continue;
        // send PWM signal
        double pulse_period = 1.0 / frequency;
        double pulse_width = pulse_period * (duty_cycle / 100);
        double remaining = pulse_period - pulse_width;
        SetLevel(pin,State::HIGH);
        sleep(pulse_width);
        SetLevel(pin,State::LOW);
        sleep(remaining);
    }
}