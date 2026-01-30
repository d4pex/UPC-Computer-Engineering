#include "TxPeliculesMesVistes.h"

#include "PassarelaPelicula.h"
#include "CercadoraPelicula.h"

#include "PassarelaVisualitzaPel.h"
#include "CercadoraVisualitzaPel.h"

#include "PassarelaContingut.h"
#include "CercadoraContingut.h"

#include "PetitFlix.h"
#include "PassarelaUsuari.h"

// Constructor
TxPeliculesMesVistes::TxPeliculesMesVistes(bool sessio) {
    pelis = vector<DTOPeliMesVisual>();
    sessioIniciada = sessio;
}

void TxPeliculesMesVistes::executar() {
    CercadoraVisualitzaPel cvp;
    vector<PassarelaVisualitzaPel> pelisMesVistes;
    pelisMesVistes = cvp.cercaPelisMesVistes();
    for (int i = 0; i < pelisMesVistes.size(); i++) {
        string titolP = pelisMesVistes[i].obteTitolPelicula();

        CercadoraPelicula cp;
        PassarelaPelicula pp;
        pp = cp.cercaPelicula(titolP);

        CercadoraContingut cc;
        PassarelaContingut pc;
        pc = cc.cercaContingut(titolP);

        DTOPeliMesVisual aux(pc, pp, pelisMesVistes[i]);

        if (sessioIniciada) {
            PetitFlix& pt = PetitFlix::getInstance();
            PassarelaUsuari usu;
            usu = pt.obteUsuari();
            string sobrenom = usu.obteSobrenom();

            PassarelaVisualitzaPel pvp;
            pvp = cvp.cercaVisualitzacioPerTitolSobrenom(titolP, sobrenom);

            if (pvp.obteData() != "") {
                aux.posaData(pvp);
            }

        }
        pelis.push_back(aux);
    }
}

// Método obtePelis
vector<DTOPeliMesVisual> TxPeliculesMesVistes::obtePelis() {
    return pelis;
}