#include "TxConsultarVisualitzacions.h"
#include "PassarelaUsuari.h"

#include "PassarelaVisualitzaPel.h"
#include "PassarelaContingut.h"
#include "PassarelaVisualitzaSerie.h"

#include "CercadoraVisualitzaPel.h"
#include "CercadoraContingut.h"
#include "CercadoraVisualitzaSerie.h"

#include "PetitFlix.h"

// Constructor por defecto
TxConsultarVisualitzacions::TxConsultarVisualitzacions() {
    // Inicializar los vectores de películas y capítulos como vacíos
    pelis = vector<DTOVisualitzaPel>();
    capitols = vector<DTOVisualitzaSerie>();
}

// Método executar
void TxConsultarVisualitzacions::executar() {
    // obtenir usuari loggejat
    PetitFlix& pt = PetitFlix::getInstance();
    PassarelaUsuari usu;
    usu = pt.obteUsuari();
    string sobrenomU = usu.obteSobrenom();

    // obtenir pelicules visualitzades;
    CercadoraVisualitzaPel cvp;
    vector<PassarelaVisualitzaPel> pvp;
    pvp = cvp.cercaVisualitzacions(sobrenomU);

    // obtenir descripcio i qualificacio edat de cada pel·lícula visualitzada
    // ficar-ho al atribut privat pelis;
    for (int i = 0; i < pvp.size(); i++) {
        string titolp = pvp[i].obteTitolPelicula();

        PassarelaContingut pc;
        CercadoraContingut cc;

        pc = cc.cercaContingut(titolp);

        DTOVisualitzaPel aux(pc, pvp[i]);
        pelis.push_back(aux);
    }

    // obtenir capitols visualitzats
    CercadoraVisualitzaSerie cvs;
    vector<PassarelaVisualitzaSerie> pvs;
    pvs = cvs.cercaElemen(sobrenomU);

    // obtenir qualificacio edat de cada capitol visualitzat
    // ficar-ho al atribut privat capitols
    for (int i = 0; i < pvs.size(); i++) {
        string titolp = pvs[i].obteTitolSerie();

        PassarelaContingut pc;
        CercadoraContingut cc;

        pc = cc.cercaContingut(titolp);

        DTOVisualitzaSerie aux(pc, pvs[i]);
        capitols.push_back(aux);
    }

    if (pvp.size() == 0 and pvs.size() == 0) {  // exc: No-Visualitzacions
        throw std::runtime_error("No hi ha cap visualitzacio sobre cap contingut per aquest usuari");
    }
}


vector<DTOVisualitzaPel> TxConsultarVisualitzacions::obtePelis() {
    return pelis;
}

vector<DTOVisualitzaSerie> TxConsultarVisualitzacions::obtecapitols() {
    return capitols;
}
