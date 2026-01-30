#include "CercadoraVisualitzaSerie.h"
#include "ConnexioBD.h"
#include <vector>

using namespace std;

CercadoraVisualitzaSerie::CercadoraVisualitzaSerie() {

}

vector<PassarelaVisualitzaSerie> CercadoraVisualitzaSerie::cercaElemen(string sobrenomU) {
    vector<PassarelaVisualitzaSerie> capitols;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT * FROM visualitzacio_capitol WHERE sobrenom_usuari = '" + sobrenomU + "' ORDER BY data DESC";
    sql::ResultSet* res = con->consulta(sql);

    // Iterar sobre todos los resultados
    while (res->next()) {
        PassarelaVisualitzaSerie aux;
        aux.posaSobrenom(res->getString("sobrenom_usuari"));
        aux.posaTitolSerie(res->getString("titol_serie"));
        aux.posaNumVisualitzacions(res->getInt("num_visualitzacions"));
        aux.posaNumTemporada(res->getInt("num_temporada"));
        aux.posaNumCapitol(res->getInt("num_capitol"));
        aux.posaData(res->getString("data"));

        capitols.push_back(aux); // Añadir capítulo al vector
    }

    // Liberar memoria de los resultados
    delete res;

    return capitols;
}