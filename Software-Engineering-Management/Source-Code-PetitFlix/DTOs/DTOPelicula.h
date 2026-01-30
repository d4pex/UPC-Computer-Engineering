#pragma once
#include "PassarelaPelicula.h"

class DTOPelicula {

private:
    string titol;
    string descripcio;
    string qualificacio;
    string dataEstrena;
    int duracio;

public:
    DTOPelicula() {
        titol = "";
        descripcio = "";
        qualificacio = "";
        dataEstrena = "";
        duracio = 0;
    }

    DTOPelicula(PassarelaPelicula& pp) {
        titol = pp.obteTitol();
        descripcio = pp.obteDescripcio();
        qualificacio = pp.obteQualificacio();
        dataEstrena = pp.obteDataEstrena();
        duracio = pp.obteDuracio();
    }

    DTOPelicula(const DTOPelicula& aux) {
        titol = aux.titol;
        descripcio = aux.descripcio;
        qualificacio = aux.qualificacio;
        dataEstrena = aux.dataEstrena;
        duracio = aux.duracio;
    }

    ~DTOPelicula() {}

    string obteTitol() const {
        return titol;
    }

    string obteDescripcio() const {
        return descripcio;
    }

    string obteQualificacio() const {
        return qualificacio;
    }

    string obteDataEstrena() const {
        return dataEstrena;
    }

    int obteDuracio() const {
        return duracio;
    }

    void posaTitol(const string& _titol) {
        titol = _titol;
    }

    void posaDescripcio(const string& _descripcio) {
        descripcio = _descripcio;
    }

    void posaQualificacio(const string& _qualificacio) {
        qualificacio = _qualificacio;
    }

    void posaDataEstrena(const string& _dataEstrena) {
        dataEstrena = _dataEstrena;
    }

    void posaDuracio(int _duracio) {
        duracio = _duracio;
    }
};
