#pragma once
#include "PassarelaCapitol.h"

class DTOCapitol {

private:
    string titolSerie;
    string qualificacio;
    string dataEstrena;
    int numeroTemporada;

public:
    DTOCapitol() {
        titolSerie = "";
        qualificacio = "";
        dataEstrena = "";
        numeroTemporada = 0;
    }

    DTOCapitol(PassarelaCapitol& cc) {
        titolSerie = cc.obteTitolSerie();
        qualificacio = cc.obteQualificacio();
        dataEstrena = cc.obteDataEstrena();
        numeroTemporada = cc.obteNumeroTemporada();
    }

    DTOCapitol(const DTOCapitol& aux) {
        titolSerie = aux.titolSerie;
        qualificacio = aux.qualificacio;
        dataEstrena = aux.dataEstrena;
        numeroTemporada = aux.numeroTemporada;
    }

    ~DTOCapitol() {}

    string obteTitolSerie() const {
        return titolSerie;
    }

    string obteQualificacio() const {
        return qualificacio;
    }

    string obteDataEstrena() const {
        return dataEstrena;
    }

    int obteNumeroTemporada() const {
        return numeroTemporada;
    }

    void posaTitolSerie(const string& _titolSerie) {
        titolSerie = _titolSerie;
    }

    void posaQualificacio(const string& _qualificacio) {
        qualificacio = _qualificacio;
    }

    void posaDataEstrena(const string& _dataEstrena) {
        dataEstrena = _dataEstrena;
    }

    void posaNumeroTemporada(const int& _numeroTemporada) {
        numeroTemporada = _numeroTemporada;
    }
};
