#include "CercadoraPelicula.h"

PassarelaPelicula CercadoraPelicula::cercaPelicula(string titolPelicula) {
    PassarelaPelicula pp;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT * FROM pelicula WHERE titol = '" + titolPelicula + "'";
    sql::ResultSet* res = con->consulta(sql);

    if (!res->next()) { // exc: PeliculaNoExisteix
        throw runtime_error("La pelicula no existeix a la base de dades.");
    }
    else {
        pp.posaTitol(res->getString("titol"));
        pp.posaDataEstrena(res->getString("data_estrena"));
        pp.posaDuracio(res->getInt("duracio"));
        delete res;
    }
    return pp;
}

vector<vector<PassarelaPelicula>> CercadoraPelicula::peliculesRelacionades(const string& titolPelicula) {
    vector<vector<PassarelaPelicula>> peliculesRelacionades;

    string consulta = "SELECT * FROM relacionat WHERE titol_x = '" + titolPelicula + "'";
    sql::ResultSet* res1 = ConnexioBD::getInstance()->consulta(consulta);

    while (res1->next()) {
        string titolPeliculaRelacionada = res1->getString("titol_y");

        PassarelaPelicula pp;

        string consultaContingut = "SELECT * FROM contingut WHERE titol = '" + titolPeliculaRelacionada + "'";
        sql::ResultSet* res2 = ConnexioBD::getInstance()->consulta(consultaContingut);
        if (res2->next()) {
            pp.posaTitol(res2->getString("titol"));
            pp.posaDescripcio(res2->getString("descripcio"));
            pp.posaQualificacio(res2->getString("qualificacio"));
        }
        delete res2;

        string consultaPelicula = "SELECT * FROM pelicula WHERE titol = '" + titolPeliculaRelacionada + "'";
        sql::ResultSet* res3 = ConnexioBD::getInstance()->consulta(consultaPelicula);
        if (res3->next()) {
            int duracio = res3->getInt("duracio");
            pp.posaDuracio(duracio > 0 ? duracio : -1);
            pp.posaDataEstrena(res3->getString("data_estrena"));
        }
        delete res3;

        vector<PassarelaPelicula> vectorPeliculaRelacionada;
        vectorPeliculaRelacionada.push_back(pp);

        peliculesRelacionades.push_back(vectorPeliculaRelacionada);
    }
    delete res1;

    return peliculesRelacionades;
}

vector<vector<PassarelaPelicula>> CercadoraPelicula::cercaProperesPelicules() {
    vector<vector<PassarelaPelicula>> properesPelicules;

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char dataActual[20];
    strftime(dataActual, sizeof(dataActual), "%Y-%m-%d %H:%M:%S", ltm);
    string data_actual = string(dataActual);

    string consulta = "SELECT * FROM pelicula WHERE data_estrena > '" + data_actual + "'";
    sql::ResultSet* res1 = ConnexioBD::getInstance()->consulta(consulta);

    while (res1->next()) {
        vector<PassarelaPelicula> properaPelicula;
        PassarelaPelicula pp;
        pp.posaTitol(res1->getString("titol"));
        pp.posaDataEstrena(res1->getString("data_estrena"));
        pp.posaDuracio(res1->getInt("duracio"));

        string titolProperaPelicula = res1->getString("titol");
        string consultaContingut = "SELECT * FROM contingut WHERE titol = '" + titolProperaPelicula + "'";
        sql::ResultSet* res2 = ConnexioBD::getInstance()->consulta(consultaContingut);
        if (res2->next()) {
            pp.posaQualificacio(res2->getString("qualificacio"));
        }

        properaPelicula.push_back(pp);
        properesPelicules.push_back(properaPelicula);
    }
    delete res1;

    return properesPelicules;
}


