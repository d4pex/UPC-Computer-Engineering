#pragma once
#include "ConnexioBD.h"
#include "PassarelaPelicula.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>

class CercadoraPelicula {
public:
    PassarelaPelicula cercaPelicula(string titolPelicula);
    vector<vector<PassarelaPelicula>> peliculesRelacionades(const string& titolPelicula);
    vector<vector<PassarelaPelicula>> cercaProperesPelicules();
};
