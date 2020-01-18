#ifndef BUZZER_H
#define BUZZER_H

uint8_t buzzPin = 10;

void buzInit(uint8_t pin)
{
    buzzPin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void beep(uint16_t timeIntervel, uint8_t beeps)
{
    for (uint8_t i = 0; i < beeps; i++)
    {
        digitalWrite(buzzPin, HIGH);
        delay(timeIntervel);
        digitalWrite(buzzPin, LOW);
        delay(timeIntervel);
    }
}
#endif