#pragma once
#include <iostream>
using namespace std;

class PassarelaContingut {
private:
    string titol;
    string descripcio;
    string qualificacio;
    string tipus;

public:
    PassarelaContingut();

    // Getters
    string obteTitol() const;
    string obteDescripcio() const;
    string obteQualificacio() const;
    string obteTipus() const;

    // Setters
    void posaTitol(string _titol);
    void posaDescripcio(string _descripcio);
    void posaQualificacio(string _qualificacio);
    void posaTipus(string _tipus);
};
