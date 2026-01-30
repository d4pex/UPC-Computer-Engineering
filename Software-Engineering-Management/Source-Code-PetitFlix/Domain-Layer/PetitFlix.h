#pragma once
#include <iostream>
#include "PassarelaUsuari.h"

class PetitFlix {
private:

    PassarelaUsuari usuari;

    PetitFlix() {}

public:

    static PetitFlix& getInstance() {
        static PetitFlix instance;
        return instance;
    }

    PassarelaUsuari obteUsuari() {
        return usuari;
    }

    void iniciaSessio(PassarelaUsuari u) {
        this->usuari = u;
    }

    void tancaSessio()
    {
        PassarelaUsuari res;
        this->usuari = res;
    }

};