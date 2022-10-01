#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <Arduino.h>

// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

//
#define LEDC_BRIGHTNESS_MAX  255

#define OFF     0
#define ON      LEDC_BRIGHTNESS_MAX

class Led
{
    private:
        uint8_t _pin;
        bool _state;
        int _brightness;
        uint8_t _channel; 

        void ledcAnalogWrite(uint8_t channel, uint32_t value);

    public:
        Led(uint8_t pin_number, uint8_t chanel_number ); //Конструктор класса, позволяет задать 
        bool init(); //Инициализация светодиода, пина, таймера
        void setBrightness(uint32_t value);
        void off();        
        void on();
};



#endif // _LED_H_