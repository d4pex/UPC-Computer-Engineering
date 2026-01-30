#pragma once
#include <vector>
#include "DTOPeliMesVisual.h"

using namespace std;

class TxPeliculesMesVistes {

private:
	vector<DTOPeliMesVisual> pelis;
	bool sessioIniciada;
public:
	TxPeliculesMesVistes(bool sessio);

	void executar();

	vector<DTOPeliMesVisual> obtePelis();

};