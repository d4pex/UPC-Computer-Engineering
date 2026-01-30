#pragma once
#include "PassarelaUsuari.h"
#include "DTOUsuari.h"

using namespace std;

class TxConsultaUsuari
{
private:
	DTOUsuari resultat;
	PassarelaUsuari usuari;

public:
	TxConsultaUsuari();

	void executar();

	DTOUsuari obteResultat();
};