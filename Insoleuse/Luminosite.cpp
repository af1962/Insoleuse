#include "Arduino.h"
#include "Luminosite.h"

// Constructeur : initialisation de la variable lux
Luminosite::Luminosite(int pwmInit)
{
    lux = pwmInit;
}

// Luminosit� Plus
void Luminosite::LumPlus(uint32_t dual_state)
{
    if (dual_state == 1)
    {
        lux = lux + 10;
        if (lux >= 100)
        {
            lux = 100;
        }
        Eclairage(1);
    }

}

// Luminosit� moins
void Luminosite::LumMoins(uint32_t dual_state)
{
    if (dual_state == 1)
    {
        lux = lux - 10;
        if (lux <= 0)
        {
            lux = 0;
        }
        Eclairage(1);
    }
}

// Eclairage poste de travail e=�tat du bouton
void Luminosite::Eclairage(uint32_t e)
{
    analogWrite(pinEclairage, 2.5 * lux * e);
}

// Retro�clairage e=�tat du bouton
void Luminosite::RetroEclairage(uint32_t e)
{
    analogWrite(pinRetroEclairage, 80 * e);   
}

