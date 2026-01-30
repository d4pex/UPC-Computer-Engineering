#include "TxEsborraUsuari.h"
#include "PetitFlix.h"

TxEsborraUsuari::TxEsborraUsuari(string cU)
{
	contrasenyaU = cU;
}

void TxEsborraUsuari::executar() {
	PetitFlix& pt = PetitFlix::getInstance();
	PassarelaUsuari pu = pt.obteUsuari();
	string cU = pu.obteContrasenya();
	if (contrasenyaU == cU) {
		pt.tancaSessio();
		pu.esborra();
	}
	else {  // exc: ErrorContrasenya
		throw runtime_error("La contrasenya no es correcta, l'usuari no s'esborrat");
	}
}