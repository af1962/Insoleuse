#include "Arduino.h"
#include <NexText.h>
class Calcul
{
private:
	uint8_t minutes;
	uint8_t secondes;
	char minSec[5];
	NexText tps = NexText(0, 8, "tempo");


public:
	void Affiche(int temps);
};
