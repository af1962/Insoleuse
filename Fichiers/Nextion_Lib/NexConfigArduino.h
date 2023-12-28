
/*
* Redefini le port série suivant l'arduino utilisé
*/


#if defined(__AVR_ATmega328P__)
#define nexSerial Serial

#elif defined(__AVR_ATmega2560__)
#define nexSerial Serial2

#endif
