#pragma once
#include "TxConsultaUsuari.h"
#include "CercadoraPelicula.h"
#include "CercadoraContingut.h"
#include "PassarelaVisualitzaPel.h"
#include "DTOUsuari.h"
#include "DTOContingut.h"
#include "DTOPelicula.h"
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

class CtrlVisualitzaPelicula {
public:
    CtrlVisualitzaPelicula();

    DTOUsuari consultaUsuari();
    void consultaPelicula(const string& nomPelicula, DTOContingut& contingut, DTOPelicula& pelicula);
    void comprovaEdat(DTOUsuari& usuari, DTOContingut& contingut);
    void comprovaEstrena(DTOPelicula& pelicula);
    string visualitzaPelicula(DTOUsuari& usuari, DTOPelicula& Pelicula);
    vector<vector<DTOPelicula>> peliculesRelacionades(DTOPelicula& Pelicula);
};