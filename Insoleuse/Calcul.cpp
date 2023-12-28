#include "Calcul.h"

void Calcul::Affiche(int temps)
{
	minutes = temps /60;
	secondes = temps % 60;
	sprintf(minSec, "%0.2d:%0.2d", minutes, secondes);
	tps.setText(minSec);
}
