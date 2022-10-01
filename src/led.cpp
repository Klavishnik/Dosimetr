#include "led.h"


//Конструктор класса. Задаем занчения канала и пина, к которому подключен диод
Led::Led(uint8_t pin_number, uint8_t chanel_number )
{
    _pin = pin_number;
    _channel = chanel_number;
}

//Устанавливаем параметры работы таймера и канала
bool Led::init()
{

    ledcSetup(_channel, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(_pin, _channel);
    return true;
}

//Регулировка яркости
void Led::ledcAnalogWrite(uint8_t channel, uint32_t value)
{
  uint32_t valueMax = LEDC_BRIGHTNESS_MAX;
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void Led::setBrightness(uint32_t value)
{
    ledcAnalogWrite(_channel, value);
}

//Выключить светодиод
void Led::off()
{
    setBrightness(OFF);
}

void Led::on()
{
    setBrightness(ON);
}