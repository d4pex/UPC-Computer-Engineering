#include "TxIniciSessio.h"
#include "PetitFlix.h"

TxIniciSessio::TxIniciSessio(string sU, string cU) {
	sobrenomU = sU;
	contrasenyaU = cU;
}

void TxIniciSessio::executar() {
	PetitFlix& pt = PetitFlix::getInstance();
	CercadoraUsuari cerca;
	PassarelaUsuari pu = cerca.cercaUsuari(sobrenomU);
	string contraPU = pu.obteContrasenya();
	if (contrasenyaU == contraPU) {
		pt.iniciaSessio(pu);
	}
	else {  // exc: ErrorContrasenya
		throw runtime_error("Hi ha hagut un error amb el sobrenom o la constrasenya");
	}
}