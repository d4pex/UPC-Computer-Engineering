#include "TxProperesEstrenes.h"

TxProperesEstrenes::TxProperesEstrenes(const string mod) {
    modalitat = mod;
    properesEstrenesPelicules.clear();
    properesEstrenesCapitols.clear();
}

bool TxProperesEstrenes::compararPerData(const DTOPelicula& a, const DTOPelicula& b) {
    return a.obteDataEstrena() < b.obteDataEstrena();
}

void TxProperesEstrenes::executar() {
    CercadoraPelicula cp;
    vector<vector<PassarelaPelicula>> properesPelicules = cp.cercaProperesPelicules();

    if (modalitat == "Completa" or modalitat == "Infantil") {
        CercadoraCapitol cc;
        vector<vector<PassarelaCapitol>> propersCapitols = cc.cercaPropersCapitols();

        if (modalitat == "Completa") {
            for (int i = 0; i < properesPelicules.size(); i++) {
                for (int j = 0; j < properesPelicules[i].size(); j++) {
                    properesEstrenesPelicules.push_back(DTOPelicula(properesPelicules[i][j]));
                }
            }

            for (int i = 0; i < propersCapitols.size(); i++) {
                for (int j = 0; j < propersCapitols[i].size(); j++) {
                    properesEstrenesCapitols.push_back(DTOCapitol(propersCapitols[i][j]));
                }
            }

        }
        else {  // Modalidad Infantil
            for (int i = 0; i < properesPelicules.size(); i++) {
                for (int j = 0; j < properesPelicules[i].size(); j++) {
                    if (properesPelicules[i][j].obteQualificacio() == "TP" ||
                        properesPelicules[i][j].obteQualificacio() == "7+" ||
                        properesPelicules[i][j].obteQualificacio() == "12+") {
                        properesEstrenesPelicules.push_back(DTOPelicula(properesPelicules[i][j]));
                    }
                }
            }

            for (int i = 0; i < propersCapitols.size(); i++) {
                for (int j = 0; j < propersCapitols[i].size(); j++) {
                    if (propersCapitols[i][j].obteQualificacio() == "TP" ||
                        propersCapitols[i][j].obteQualificacio() == "7+" ||
                        propersCapitols[i][j].obteQualificacio() == "12+") {
                        properesEstrenesCapitols.push_back(DTOCapitol(propersCapitols[i][j]));
                    }
                }
            }
        }
    }
    else if (modalitat == "Cinefil") {
        for (int i = 0; i < properesPelicules.size(); i++) {
            for (int j = 0; j < properesPelicules[i].size(); j++) {
                properesEstrenesPelicules.push_back(DTOPelicula(properesPelicules[i][j]));
            }
        }
    }

    sort(properesEstrenesPelicules.begin(), properesEstrenesPelicules.end(), compararPerData);
}


vector<DTOPelicula> TxProperesEstrenes::obteResultatPelicules() const {
    return properesEstrenesPelicules;
}

vector<DTOCapitol> TxProperesEstrenes::obteResultatCapitols() const {
    return properesEstrenesCapitols;
}
