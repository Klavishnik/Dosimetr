//ВАЖНО! Схемы подключения по образцу вы можете найти в "extras/Example schemes/ESP32 & ESP8266/" и выбрать свою плату
// Подключаем необходимые библиотеки
#include <Arduino.h>
#include "CG_RadSens.h" 
#include <Wire.h>        // I2C-библиотека
#include <GyverOLED.h>   // Библиотека для OLED Gyver'а идеально подойдёт для понимания методики работы с OLED-экраном, к тому же тут сразу есть русский шрифт
#include "led.h"


#define LED_PIN            13
#define LEDC_CHANNEL_0     0

#define TIME_STATIC 500

GyverOLED<SSH1106_128x64> oled;   // Инициализируем OLED-экран
CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS); 
Led led(LED_PIN, LEDC_CHANNEL_0);

uint32_t timer_oled; // таймер обновления дисплея
uint32_t timer_cnt; // Таймер опроса интенсивности излучения и ипульсов для OLED-экрана
uint32_t timer_imp; // Таймер опроса импульсов для пьезоизлучателя
uint32_t timer_static; // Таймер статического замера
uint32_t timer_static_start;
uint32_t timer_led;


float dynval;  // Переменная для динамического значения интенсивности
float statval; // Переменная для статического значения интенсивности
uint32_t impval;  // Переменная для кол-ва импульсов
uint32_t pulsesPrev;  // Переменная, содержащая кол-во импульсов за прошлый цикл

String dynint;
String nimp;
String statint;
String static_time_str;

void ledcAnalogWrite(uint8_t channel, uint32_t value)
{
  uint32_t valueMax = 255;
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}



void setup() 
{

    Serial.begin(9600);
    oled.init();           // Инициализируем OLED в коде
    oled.clear();
    oled.update();

    oled.setScale(2);      // Устанавливаем размер шрифта


    radSens.init();
    oled.clear();
    oled.update();

    led.init();
    led.on();
    delay(3000);
    led.off();
    oled.clear();

    timer_static_start = millis(); // время начало замера
}

void loop() 
{
    if(millis() - timer_led > 200)
        led.off();

    if( impval < radSens.getNumberOfPulses())
    {
        led.on();
        timer_led = millis();
    }

    if (millis() - timer_cnt > 1000)
    {      // Записываем в объявленные глобальные переменные необходимые значения
        timer_cnt = millis();
        dynval = radSens.getRadIntensyDynamic(); 
        statval = radSens.getRadIntensyStatic();
        impval = radSens.getNumberOfPulses();
        timer_static = millis(); // обновим значение 
    }

    if( millis() - timer_static_start > TIME_STATIC * 1000)
    {
        statint = "Ст:  ";
        statint += statval;
        timer_static_start = millis();
    }

    if (millis() - timer_oled > 1000) 
    {  
    //Записываем переменные в строки и выводим их на OLED-экран
        oled.clear();
        timer_oled = millis();
        dynint = "Дин: ";
        dynint += dynval;
        nimp = "Имп: ";
        nimp += impval;
        oled.setCursor(0, 0);
        oled.print(dynint);
        oled.setCursor(0, 2);
        
        oled.print(statint);
        oled.setCursor(0, 4);
        oled.print(nimp);

        oled.setCursor(0, 6);

        static_time_str =  (millis() - timer_static_start) / 1000;
        static_time_str += "/500";
        oled.print(static_time_str);

        oled.update();
    }

    Serial.println(radSens.getNumberOfPulses());
}