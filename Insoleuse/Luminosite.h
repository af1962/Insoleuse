#include <Arduino.h>
#define pinEclairage 11
#define pinRetroEclairage 3

class Luminosite
{
private:
	int lux;

public:
	Luminosite(int pwmInit);
	void LumPlus(uint32_t e);
	void LumMoins(uint32_t e);
	void Eclairage(uint32_t e);
	void RetroEclairage(uint32_t e);
};

