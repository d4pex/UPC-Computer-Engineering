#pragma once
#include <iostream>
#include <vector>
#include "PassarelaPelicula.h"
#include "PassarelaCapitol.h"
#include "CercadoraPelicula.h"
#include "CercadoraCapitol.h"
#include "DTOCapitol.h"
#include "DTOPelicula.h"

using namespace std;

class TxProperesEstrenes {
private:
    string modalitat;
    vector<DTOPelicula> properesEstrenesPelicules;
    vector<DTOCapitol> properesEstrenesCapitols;

public:
    TxProperesEstrenes(const string mod);
    void executar();
    static bool compararPerData(const DTOPelicula& a, const DTOPelicula& b);
    vector<DTOPelicula> obteResultatPelicules() const;
    vector<DTOCapitol> obteResultatCapitols() const;
};
