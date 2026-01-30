#include "CercadoraCapitol.h"

vector<vector<PassarelaCapitol>> CercadoraCapitol::cercaPropersCapitols() {
    vector<vector<PassarelaCapitol>> propersCapitols;

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char dataActual[20];
    strftime(dataActual, sizeof(dataActual), "%Y-%m-%d %H:%M:%S", ltm);
    string data_actual = string(dataActual);

    string consulta = "SELECT * FROM capitol WHERE data_estrena > '" + data_actual + "'";
    sql::ResultSet* res1 = ConnexioBD::getInstance()->consulta(consulta);

    while (res1->next()) {
        vector<PassarelaCapitol> properCapitol;
        PassarelaCapitol pc;
        pc.posaTitolSerie(res1->getString("titol_serie"));
        pc.posaQualificacio(res1->getString("qualificacio"));
        pc.posaDataEstrena(res1->getString("data_estrena"));
        pc.posaNumeroTemporada(res1->getInt("numero_temporada"));

        properCapitol.push_back(pc);
        propersCapitols.push_back(properCapitol);
    }
    delete res1;

    return propersCapitols;
}

