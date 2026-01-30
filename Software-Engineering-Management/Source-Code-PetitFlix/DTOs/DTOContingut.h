#pragma once
#include "PassarelaContingut.h"

class DTOContingut {

private:
    string titol;
    string descripcio;
    string qualificacio;

public:
    // Constructor por defecto
    DTOContingut() {
        titol = "";
        descripcio = "";
        qualificacio = "";
    }

    // Constructor que recibe la passarela de contingut
    DTOContingut(PassarelaContingut& pc) {
        titol = pc.obteTitol();
        descripcio = pc.obteDescripcio();
        qualificacio = pc.obteQualificacio();
    }

    // Constructor por copia
    DTOContingut(const DTOContingut& aux) {
        titol = aux.titol;
        descripcio = aux.descripcio;
        qualificacio = aux.qualificacio;
    }

    // Destructor
    ~DTOContingut() {}

    // Métodos de acceso
    string obteTitol() const {
        return titol;
    }

    string obteDescripcio() const {
        return descripcio;
    }

    string obteQualificacio() const {
        return qualificacio;
    }
};
