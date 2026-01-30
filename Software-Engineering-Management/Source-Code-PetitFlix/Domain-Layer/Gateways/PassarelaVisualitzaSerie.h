#pragma once
#include <iostream>
using namespace std;

class PassarelaVisualitzaSerie {
private:
    string sobrenom;
    string titolSerie;
    int numVisualitzacions;
    int numTemporada;
    int numCapitol;
    string data;

public:
    PassarelaVisualitzaSerie();

    // Getters
    string obteSobrenom() const;
    string obteTitolSerie() const;
    int obteNumVisualitzacions() const;
    int obteNumTemporada() const;
    int obteNumCapitol() const;
    string obteData() const;

    // Setters
    void posaSobrenom(string _sobrenom);
    void posaTitolSerie(string _titolSerie);
    void posaNumVisualitzacions(int _numVisualitzacions);
    void posaNumTemporada(int _numTemporada);
    void posaNumCapitol(int _numCapitol);
    void posaData(string _data);
};