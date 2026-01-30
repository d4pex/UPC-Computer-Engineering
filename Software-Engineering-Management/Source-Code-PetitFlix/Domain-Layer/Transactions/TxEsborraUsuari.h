#pragma once
#include "PassarelaUsuari.h"
using namespace std;

class TxEsborraUsuari
{
public:
	TxEsborraUsuari(string cU);

	void executar();
private:
	string contrasenyaU;
};