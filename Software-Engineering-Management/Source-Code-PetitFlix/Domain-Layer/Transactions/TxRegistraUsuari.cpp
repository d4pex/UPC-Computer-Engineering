#include "TxRegistraUsuari.h"

TxRegistraUsuari::TxRegistraUsuari(string nU, string sU, string cU, string ceU, string dnU, string modU) {
    nomU = nU;
    sobrenomU = sU;
    contrasenyaU = cU;
    correuElectronicU = ceU;
    dataNaixementU = dnU;
    modalitatU = modU;
}

void TxRegistraUsuari::executar() {
    PassarelaUsuari pu(nomU, sobrenomU, contrasenyaU, correuElectronicU, dataNaixementU, modalitatU);
    pu.insereix();
}