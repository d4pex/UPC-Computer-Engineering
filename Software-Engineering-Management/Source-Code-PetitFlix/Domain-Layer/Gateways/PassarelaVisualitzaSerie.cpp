#include "PassarelaVisualitzaSerie.h"

// Constructor por defecto
PassarelaVisualitzaSerie::PassarelaVisualitzaSerie() {
    sobrenom = "";
    titolSerie = "";
    numVisualitzacions = 0;
    numTemporada = 0;
    numCapitol = 0;
    data = "";
}

// Getters
string PassarelaVisualitzaSerie::obteSobrenom() const {
    return sobrenom;
}

string PassarelaVisualitzaSerie::obteTitolSerie() const {
    return titolSerie;
}

int PassarelaVisualitzaSerie::obteNumVisualitzacions() const {
    return numVisualitzacions;
}

int PassarelaVisualitzaSerie::obteNumTemporada() const {
    return numTemporada;
}

int PassarelaVisualitzaSerie::obteNumCapitol() const {
    return numCapitol;
}

string PassarelaVisualitzaSerie::obteData() const {
    return data;
}

// Setters
void PassarelaVisualitzaSerie::posaSobrenom(string _sobrenom) {
    sobrenom = _sobrenom;
}

void PassarelaVisualitzaSerie::posaTitolSerie(string _titolSerie) {
    titolSerie = _titolSerie;
}

void PassarelaVisualitzaSerie::posaNumVisualitzacions(int _numVisualitzacions) {
    numVisualitzacions = _numVisualitzacions;
}

void PassarelaVisualitzaSerie::posaNumTemporada(int _numTemporada) {
    numTemporada = _numTemporada;
}

void PassarelaVisualitzaSerie::posaNumCapitol(int _numCapitol) {
    numCapitol = _numCapitol;
}

void PassarelaVisualitzaSerie::posaData(string _data) {
    data = _data;
}