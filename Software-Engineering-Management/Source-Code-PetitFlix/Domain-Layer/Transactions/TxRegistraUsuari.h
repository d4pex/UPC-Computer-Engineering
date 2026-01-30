#pragma once
#include "PassarelaUsuari.h"

using namespace std;

class TxRegistraUsuari {
private:
	string nomU;
	string sobrenomU;
	string contrasenyaU;
	string correuElectronicU;
	string dataNaixementU;
	string modalitatU;

public:
	TxRegistraUsuari(string nU, string sU, string cU, string ceU, string dnU, string modU);

	void executar();
};