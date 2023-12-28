#include "Buzzer.h"

Buzzer::Buzzer()
{
    pinMode(10, OUTPUT);
}

void Buzzer::BuzzerOn(int r, int n, int f)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < n; j++)
        {
            digitalWrite(buzzer, HIGH);
            delay(f);
            digitalWrite(buzzer, LOW);
            delay(f);
        }
        delay(1000);
    }
}