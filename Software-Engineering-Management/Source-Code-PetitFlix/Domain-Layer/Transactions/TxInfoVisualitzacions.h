#pragma once
#include "PassarelaUsuari.h"

using namespace std;

class TxInfoVisualitzacions {
private:
    int resultatpelis;
    int resultatserie;

public:
    TxInfoVisualitzacions();

    void executar();

    int obteResultatpelis();

    int obteResultatserie();
};