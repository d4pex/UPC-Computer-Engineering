#include "CtrlVisualitzaPelicula.h"

CtrlVisualitzaPelicula::CtrlVisualitzaPelicula() {}

DTOUsuari CtrlVisualitzaPelicula::consultaUsuari() {
    TxConsultaUsuari txCU;
    txCU.executar();
    return txCU.obteResultat();
}

void CtrlVisualitzaPelicula::consultaPelicula(const string& nomPelicula, DTOContingut& contingut, DTOPelicula& pelicula) {
    CercadoraPelicula cercaP;
    PassarelaPelicula pp = cercaP.cercaPelicula(nomPelicula);

    if (pp.obteTitol().empty()) { // exc: PeliculaNoExisteix
        throw runtime_error("La pelicula no existeix.");
    }

    CercadoraContingut cercaC;
    PassarelaContingut pc = cercaC.cercaContingut(nomPelicula);

    if (pc.obteDescripcio().empty()) { // exc: ContingutNoExisteix
        throw runtime_error("El contingut no existeix.");
    }

    pelicula = DTOPelicula(pp);
    contingut = DTOContingut(pc);
}

void CtrlVisualitzaPelicula::comprovaEdat(DTOUsuari& usuari, DTOContingut& contingut) {
    string dataNaixement = usuari.obteDataNaixement();
    int anyN, mesN, diaN;
    int result = sscanf(dataNaixement.c_str(), "%d-%d-%d", &anyN, &mesN, &diaN);
    if (result != 3) {
        throw runtime_error("Format de la data de naixement incorrecte.");
    }

    time_t tempsActual = time(0);
    tm* dataActual = localtime(&tempsActual);

    int edatUsuari = dataActual->tm_year + 1900 - anyN;
    if (dataActual->tm_mon + 1 < mesN || (dataActual->tm_mon + 1 == mesN && dataActual->tm_mday < diaN)) {
        edatUsuari--;
    }

    string qualificacio = contingut.obteQualificacio();
    int edatMinima = stoi(qualificacio.substr(0, qualificacio.size() - 1));

    if (edatUsuari < edatMinima) { // exc: EdatNoApropiada
        throw runtime_error("La pelicula no es adequada per a la teva edat.");
    }
}

void CtrlVisualitzaPelicula::comprovaEstrena(DTOPelicula& pelicula) {
    string dataEstrena = pelicula.obteDataEstrena();

    struct tm estrena = {};
    istringstream ss(dataEstrena);
    ss >> get_time(&estrena, "%Y-%m-%d");
    if (ss.fail()) {
        throw runtime_error("Format de la data d'estrena incorrecte.");
    }

    time_t dataEstrenaTime = mktime(&estrena);
    time_t tempsActual = time(0);

    if (dataEstrenaTime > tempsActual) { // exc: Pel·liculaNoEstrenada
        string dataFormatada = dataEstrena.substr(8, 2) + "/" + dataEstrena.substr(5, 2) + "/" + dataEstrena.substr(0, 4);
        throw runtime_error("La pelicula no ha estat estrenada encara. Data d'estrena: " + dataFormatada);
    }
}

string CtrlVisualitzaPelicula::visualitzaPelicula(DTOUsuari& usuari, DTOPelicula& Pelicula) {
    PassarelaVisualitzaPel pVP;

    pVP.posaSobrenom(usuari.obteSobrenom());
    pVP.posaTitolPelicula(Pelicula.obteTitol());

    pVP.insereix();

    return pVP.obteData();
}

static bool compararPerData(const vector<DTOPelicula>& a, const vector<DTOPelicula>& b) {
    return a[0].obteDataEstrena() > b[0].obteDataEstrena();
}

vector<vector<DTOPelicula>> CtrlVisualitzaPelicula::peliculesRelacionades(DTOPelicula& Pelicula) {
    CercadoraPelicula cP;

    vector<vector<PassarelaPelicula>> peliculesRelacionadesPP = cP.peliculesRelacionades(Pelicula.obteTitol());

    vector<vector<DTOPelicula>> peliculesRelacionades;

    for (unsigned int i = 0; i < peliculesRelacionadesPP.size(); ++i) {
        vector<DTOPelicula> filaDTO;
        for (unsigned int j = 0; j < peliculesRelacionadesPP[i].size(); ++j) {
            PassarelaPelicula pp = peliculesRelacionadesPP[i][j];

            DTOPelicula dto;
            dto.posaTitol(pp.obteTitol());
            dto.posaDataEstrena(pp.obteDataEstrena());
            dto.posaDuracio(pp.obteDuracio());
            dto.posaDescripcio(pp.obteDescripcio());
            dto.posaQualificacio(pp.obteQualificacio());
            filaDTO.push_back(dto);
        }
        peliculesRelacionades.push_back(filaDTO);
    }

    vector<vector<DTOPelicula>> peliculesUniques;

    for (unsigned int i = 0; i < peliculesRelacionades.size(); ++i) {
        const vector<DTOPelicula>& peli = peliculesRelacionades[i];

        bool existeix = false;
        for (unsigned int j = 0; j < peliculesUniques.size(); ++j) {
            const vector<DTOPelicula>& unica = peliculesUniques[j];
            if (unica[0].obteTitol() == peli[0].obteTitol()) {
                existeix = true;
                break;
            }
        }

        if (!existeix) {
            peliculesUniques.push_back(peli);
        }
    }

    sort(peliculesUniques.begin(), peliculesUniques.end(), compararPerData);

    return peliculesUniques;
}



