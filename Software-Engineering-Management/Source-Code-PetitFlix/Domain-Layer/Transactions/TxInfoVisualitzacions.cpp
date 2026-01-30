#include "PassarelaUsuari.h"
#include "PassarelaVisualitzaPel.h"
#include "PassarelaVisualitzaSerie.h"
#include "TxInfoVisualitzacions.h"
#include "CercadoraVisualitzaPel.h"
#include "CercadoraVisualitzaSerie.h"
#include "PetitFlix.h"

TxInfoVisualitzacions::TxInfoVisualitzacions() {
    resultatpelis = 0;
    resultatserie = 0;
}

void TxInfoVisualitzacions::executar() {
    PetitFlix& pt = PetitFlix::getInstance();
    PassarelaUsuari usuari;
    usuari = pt.obteUsuari(); // obtenim passarela de Usuari loggejat

    string sobrenomU = usuari.obteSobrenom();

    CercadoraVisualitzaPel cvp;
    vector<PassarelaVisualitzaPel> visualitzacionsP;
    visualitzacionsP = cvp.cercaVisualitzacions(sobrenomU);

    CercadoraVisualitzaSerie cvs;
    vector<PassarelaVisualitzaSerie> visualitzacionsS;
    visualitzacionsS = cvs.cercaElemen(sobrenomU);

    resultatpelis = visualitzacionsP.size();

    resultatserie = visualitzacionsS.size();
}

int TxInfoVisualitzacions::obteResultatpelis() {
    return resultatpelis;
}

int TxInfoVisualitzacions::obteResultatserie() {
    return resultatserie;
}