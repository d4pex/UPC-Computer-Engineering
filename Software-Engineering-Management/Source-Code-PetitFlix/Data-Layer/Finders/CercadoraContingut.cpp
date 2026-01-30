#include "CercadoraContingut.h"
#include "ConnexioBD.h"
#include <iostream>
using namespace std;

PassarelaContingut CercadoraContingut::cercaContingut(const string titolContingut) {
    PassarelaContingut pc;

    ConnexioBD* con = ConnexioBD::getInstance();

    string sql = "SELECT * FROM contingut WHERE titol = '" + titolContingut + "'";
    sql::ResultSet* res = con->consulta(sql);

    if (!res->next()) {
        delete res;
        throw std::runtime_error("Error: El contingut amb titol '" + titolContingut + "' no existeix.");
    }

    pc.posaTitol(res->getString("titol"));
    pc.posaDescripcio(res->getString("descripcio"));
    pc.posaQualificacio(res->getString("qualificacio"));
    pc.posaTipus(res->getString("tipus"));

    delete res;
    return pc;
}
