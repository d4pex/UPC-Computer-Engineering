#include "CercadoraVisualitzaPel.h"
#include "ConnexioBD.h"
#include <vector>

using namespace std;

// Constructor de la clase
CercadoraVisualitzaPel::CercadoraVisualitzaPel() {}

vector <PassarelaVisualitzaPel> CercadoraVisualitzaPel::cercaVisualitzacions(string sobrenomU) {
    vector<PassarelaVisualitzaPel> pelis;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT * FROM visualitzacio_pelicula WHERE sobrenom_usuari = '" + sobrenomU + "' ORDER BY data DESC";
    sql::ResultSet* res = con->consulta(sql);

    // Iterar sobre tots els resultats
    while (res->next()) {
        PassarelaVisualitzaPel aux;
        aux.posaSobrenom(res->getString("sobrenom_usuari"));
        aux.posaTitolPelicula(res->getString("titol_pelicula"));
        aux.posaData(res->getString("data"));
        aux.posaNumVisualitzacions(res->getInt("num_visualitzacions"));

        pelis.push_back(aux); // afegir pelicula al vector
    }
    delete res;

    return pelis;
}

vector<PassarelaVisualitzaPel> CercadoraVisualitzaPel::cercaPelisMesVistes() {
    vector<PassarelaVisualitzaPel> pelisOrdenades;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT titol_pelicula, SUM(num_visualitzacions) AS total_visualitzacions "
        "FROM visualitzacio_pelicula "
        "GROUP BY titol_pelicula "
        "ORDER BY total_visualitzacions DESC";
    sql::ResultSet* res = con->consulta(sql);

    int i = 0;
    while (res->next() and i < 5) {
        PassarelaVisualitzaPel aux;
        aux.posaTitolPelicula(res->getString("titol_pelicula"));
        aux.posaNumVisualitzacions(res->getInt("total_visualitzacions"));

        pelisOrdenades.push_back(aux);
        i++;
    }
    delete res;

    return pelisOrdenades;
}

PassarelaVisualitzaPel CercadoraVisualitzaPel::cercaVisualitzacioPerTitolSobrenom(string titolPelicula, string sobrenomU) {
    PassarelaVisualitzaPel peli;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT * FROM visualitzacio_pelicula WHERE titol_pelicula = '" + titolPelicula + "' AND sobrenom_usuari = '" + sobrenomU + "'";
    sql::ResultSet* res = con->consulta(sql);

    // Verificar si se encontró una fila con los datos solicitados
    if (res->next()) {
        peli.posaSobrenom(res->getString("sobrenom_usuari"));
        peli.posaTitolPelicula(res->getString("titol_pelicula"));
        peli.posaData(res->getString("data"));
        peli.posaNumVisualitzacions(res->getInt("num_visualitzacions"));
    }

    // Liberar memoria
    delete res;

    return peli;
}
