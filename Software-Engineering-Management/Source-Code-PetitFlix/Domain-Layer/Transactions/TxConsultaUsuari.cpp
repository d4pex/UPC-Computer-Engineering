#include "TxConsultaUsuari.h"
#include "PetitFlix.h"

TxConsultaUsuari::TxConsultaUsuari() {}

void TxConsultaUsuari::executar() {
    PetitFlix& pt = PetitFlix::getInstance();
    usuari = pt.obteUsuari(); 
    DTOUsuari aux(usuari);
    resultat = aux;
}

DTOUsuari TxConsultaUsuari::obteResultat() {
    return resultat;
}
