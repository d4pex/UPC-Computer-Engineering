#include "PassarelaCapitol.h"

PassarelaCapitol::PassarelaCapitol() {
    titolSerie = "";
    titolCapitol = "";
    qualificacio = "";
    dataEstrena = "";
    numeroTemporada = 0;
}

string PassarelaCapitol::obteTitolSerie() const {
    return titolSerie;
}

string PassarelaCapitol::obteTitolCapitol() const {
    return titolCapitol;
}

string PassarelaCapitol::obteQualificacio() const {
    return qualificacio;
}

string PassarelaCapitol::obteDataEstrena() const {
    return dataEstrena;
}

int PassarelaCapitol::obteNumeroTemporada() const {
    return numeroTemporada;
}

void PassarelaCapitol::posaTitolSerie(string _titolSerie) {
    titolSerie = _titolSerie;
}

void PassarelaCapitol::posaTitolCapitol(string _titolCapitol) {
    titolCapitol = _titolCapitol;
}

void PassarelaCapitol::posaQualificacio(string _qualificacio) {
    qualificacio = _qualificacio;
}

void PassarelaCapitol::posaDataEstrena(string _dataEstrena) {
    dataEstrena = _dataEstrena;
}

void PassarelaCapitol::posaNumeroTemporada(int _numeroTemporada) {
    numeroTemporada = _numeroTemporada;
}
