#pragma once
#include "CercadoraUsuari.h"

using namespace std;

class TxIniciSessio
{
private:
	string sobrenomU;
	string contrasenyaU;

public:
	TxIniciSessio(string sU, string cU);

	void executar();
};