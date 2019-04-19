#ifndef Satelite_h
#define Satelite_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
//#else
//#include <WProgram.h>
//#include <pins_arduino.h>
#endif

class Satelite
{
    public:
    Satelite();
    //void imprimirMensagem();
    float areaRetangulo(int x, int y);
    float areaTriangulo(int x, int y);
    int authenticate(char login[], char password[]);
    //int escolheArea();

    private:
};

#endif