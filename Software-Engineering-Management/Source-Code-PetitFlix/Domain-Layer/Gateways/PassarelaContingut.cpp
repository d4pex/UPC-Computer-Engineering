#include "PassarelaContingut.h"

// Constructor
PassarelaContingut::PassarelaContingut() {
    titol = "";
    descripcio = "";
    qualificacio = "";
    tipus = "";
}

// Getters
string PassarelaContingut::obteTitol() const {
    return titol;
}

string PassarelaContingut::obteDescripcio() const {
    return descripcio;
}

string PassarelaContingut::obteQualificacio() const {
    return qualificacio;
}

string PassarelaContingut::obteTipus() const {
    return tipus;
}

// Setters
void PassarelaContingut::posaTitol(string _titol) {
    titol = _titol;
}

void PassarelaContingut::posaDescripcio(string _descripcio) {
    descripcio = _descripcio;
}

void PassarelaContingut::posaQualificacio(string _qualificacio) {
    qualificacio = _qualificacio;
}

void PassarelaContingut::posaTipus(string _tipus) {
    tipus = _tipus;
}
