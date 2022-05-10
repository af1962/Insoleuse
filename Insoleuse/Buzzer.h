#include "Arduino.h"
#define buzzer 10        //Buzzer 13

class Buzzer
{
public:
	Buzzer(int p);
	void BuzzerOn(int r, int n, int f);
};