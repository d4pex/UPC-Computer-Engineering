#pragma once
#include <iostream>
#include "PassarelaVisualitzaPel.h"
#include <vector>
using namespace std;

class CercadoraVisualitzaPel {
private:

public:
    // Constructor
    CercadoraVisualitzaPel();

    // Método
    vector<PassarelaVisualitzaPel> cercaVisualitzacions(string sobrenomU);
    vector<PassarelaVisualitzaPel> cercaPelisMesVistes();
    PassarelaVisualitzaPel cercaVisualitzacioPerTitolSobrenom(string titolPelicula, string sobrenomU);
};