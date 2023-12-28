
#include <Nextion.h>
#include "Calcul.h"
#include "Buzzer.h"
#include "Luminosite.h"


// Classes externes
Calcul c;
Buzzer b;
Luminosite lum(10);

// Objets Nextion
// (page id, component id, component name)
NexPage page0 = NexPage(0, 0, "Main");
NexPage page1 = NexPage(1, 0, "Fin");

NexPage page2 = NexPage(2, 0, "Gravure");
NexProgressBar nexprogress = NexProgressBar(2, 3, "pourcent");
NexText txtTitre = NexText(2, 3, "titre");

NexDSButton bpBuzzer = NexDSButton(0, 1, "buzzer");
NexDSButton bpLumiere = NexDSButton(0, 2, "lumiere");
NexButton bpStart = NexButton(0, 3, "start");
NexText txtMode = NexText(0, 4, "mode");
NexDSButton bpCircuit = NexDSButton(0, 5, "circuit");
NexButton bpPlus = NexButton(0, 6, "plus");
NexButton bpMoins = NexButton(0, 7, "moins");
NexDSButton bpInc = NexDSButton(0, 9, "lent");
NexButton bpEplus = NexButton(0, 10, "eplus");
NexButton bpEmoins = NexButton(0, 11, "emoins");
NexDSButton bpRetro = NexDSButton(0, 12, "retro");

// Register objects to the touch event list
NexTouch* nex_listen_list[] = {
  &bpBuzzer,
  &bpLumiere,
  &bpStart,
  &bpCircuit,
  &bpPlus,
  &bpMoins,
  &bpInc,
  &bpEplus,
  &bpEmoins,
  &bpRetro,
  NULL
};

//----------PIN----------//
#define pinUV 4               //UV4

//-------Variables-------//
int
duree = 45,                 // Durée temps insolation par d�faut
inc = 15;                   // Incrément de la tempo

char texte[16];           // Tableau de caractére pour affiche de texte dans Nextion
float tpsGlobal = 0;      // float est nécessaire pour division avec décimales

uint32_t
dual_stateCircuit,        // Choix photogravure ou tropicalisation
dual_stateEclairage,      // Eclairage
dual_stateRetroEclairage, // Retroéclairage
dual_stateBuzzer,        // Buzzer On/Off
dual_stateInc;            // Incrément lent/rapide 

unsigned long previousMillis = 0;
unsigned long currentMillis;
const long interval = 1000;


bool
etatRetroEclairage;


//-VOID SETUP
void setup()
{
    pinMode(pinUV, OUTPUT);
 
    Serial.begin(9600);

    while (!Serial)    // D�lai n�cessaire pour laisser le temps � l'�cran de d�marrer et initialiser les variables
    {
        ;
    }

    //Initialize Nextion Library
    nexInit();
    nexprogress.setValue(0);

    // Register the push/pop event callback function
    // Pop : cocher la case "Send Component ID" de Touch Release Event dans Nextion
    // Push : cocher la case "Send Component ID" de Touch Press Event dans Nextion
    bpBuzzer.attachPush(bpBuzzerPushCallback, &bpBuzzer);
    bpLumiere.attachPush(bpLumierePushCallback, &bpLumiere);
    bpStart.attachPush(bpStartPushCallback, &bpStart);
    bpCircuit.attachPush(bpCircuitPushCallback, &bpCircuit);
    bpPlus.attachPush(bpPlusPushCallback, &bpPlus);
    bpMoins.attachPush(bpMoinsPushCallback, &bpMoins);
    bpInc.attachPush(bpIncPushCallback, &bpInc);
    bpEplus.attachPush(bpEplusPushCallback, &bpEplus);
    bpEmoins.attachPush(bpEmoinsPushCallback, &bpEmoins);
    bpRetro.attachPush(bpRetroPushCallback, &bpRetro);

    // Affichage par d�faut
    sprintf(texte, "%s", "Photogravure");
    txtMode.setText(texte);
    c.Affiche(duree);
    //txtMode.setText("Photogravure");
}

// Fin d'exposition
void Fin(String message)
{
    page1.show();
    digitalWrite(pinUV, LOW);
    nexprogress.setValue(0);
    if (dual_stateBuzzer == 1)
    {
        b.BuzzerOn(3, 3, 70);
    }
    else
    {
        delay(2500);
    }
    page0.show();
    bpLumiere.setValue(dual_stateEclairage);
    bpRetro.setValue(0);
    bpInc.setValue(dual_stateInc);
    bpBuzzer.setValue(dual_stateBuzzer);
    duree = tpsGlobal;
    bpCircuit.setValue(dual_stateCircuit);
    txtMode.setText(texte);
    c.Affiche(duree);
}

// Fonction choix du circuit
void bpCircuitPushCallback(void* ptr)
{
    bpCircuit.getValue(&dual_stateCircuit);
    if (dual_stateCircuit == 1)
    {
        sprintf(texte, "%s", "Tropicalisation");
        duree = 300;
    }
    if (dual_stateCircuit == 0)
    {
        sprintf(texte, "%s", "Photogravure");
        duree = 45;
    }
    txtMode.setText(texte);
    c.Affiche(duree);
}


// Fonction incr�mentation Lent/Rapide
void bpIncPushCallback(void* ptr)
{
    bpInc.getValue(&dual_stateInc);

    if (dual_stateInc == 1)
    {
        inc = 60;
    }
    if (dual_stateInc == 0)
    {
        inc = 15;
    }
}

// Fonction incr�mentation tempo
void bpPlusPushCallback(void* ptr)
{
    duree = duree + inc;
    c.Affiche(duree);
}

// Fonction d�cr�mentation tempo
void bpMoinsPushCallback(void* ptr)
{
    duree = duree - inc;
    if (duree <= 0)
    {
        duree = 0;
    }
    c.Affiche(duree);
}

// Bouton start
void bpStartPushCallback(void* ptr)
{
    lum.RetroEclairage(0);   
    tpsGlobal = duree;
    page2.show();
    txtTitre.setText(texte);
    if (dual_stateCircuit == 0)              // Choix des images progressbar
    {
        nexprogress.setImgbpic(5);
       nexprogress.setImgppic(6);
    }
    else
    {
        nexprogress.setImgbpic(6);
        nexprogress.setImgppic(7);
    }
    MarcheUV();
    Fin("Fin");
}

// Marche des UV 
void MarcheUV()
{      
    digitalWrite(pinUV, HIGH); 
    while (duree >= 0)
    {  currentMillis = millis();
       nexprogress.setValue(((tpsGlobal - duree) / tpsGlobal) * 100);

        if (currentMillis - previousMillis >= interval)
        {
           previousMillis = currentMillis;
           c.Affiche(duree);
           duree--;
        }
    }
     digitalWrite(pinUV, LOW);
}

// Eclairage : R�cup�re l'�tat du bouton 1 ou 0
void bpLumierePushCallback(void* ptr)
{    
    bpLumiere.getValue(&dual_stateEclairage);
    lum.Eclairage(dual_stateEclairage);
}

// Eclairage plus
void bpEplusPushCallback(void* ptr)
{
    lum.LumPlus(dual_stateEclairage);
}

// Eclairage moins
void bpEmoinsPushCallback(void* ptr)
{
    lum.LumMoins(dual_stateEclairage);
}

// Buzzer marche/arr�t
void bpBuzzerPushCallback(void* ptr)
{       
    bpBuzzer.getValue(&dual_stateBuzzer); 
}

// R�tro�clairage   
void bpRetroPushCallback(void* ptr)
{       
    bpRetro.getValue(&dual_stateRetroEclairage);
    lum.RetroEclairage(dual_stateRetroEclairage);    
}

//-VOID LOOP
void loop()
{
    nexLoop(nex_listen_list);

}