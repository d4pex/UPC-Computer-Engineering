#pragma once
#include <iostream>
using namespace std;

class PassarelaCapitol {
private:
    string titolSerie;
    string titolCapitol;
    string qualificacio;
    string dataEstrena;
    int numeroTemporada;

public:
    PassarelaCapitol();

    string obteTitolSerie() const;
    string obteTitolCapitol() const;
    string obteQualificacio() const;
    string obteDataEstrena() const;
    int obteNumeroTemporada() const;

    void posaTitolSerie(string _titolSerie);
    void posaTitolCapitol(string _titolCapitol);
    void posaQualificacio(string _qualificacio);
    void posaDataEstrena(string _dataEstrena);
    void posaNumeroTemporada(int _numeroTemporada);
};