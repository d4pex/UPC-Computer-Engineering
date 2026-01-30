#pragma once
#include <iostream>
#include <vector>
#include "DTOVisualitzaPel.h"
#include "DTOVisualitzaSerie.h"

using namespace std;

class TxConsultarVisualitzacions {
private:
	vector<DTOVisualitzaPel> pelis;
	vector<DTOVisualitzaSerie> capitols;

public:
	TxConsultarVisualitzacions();

	void executar();

	vector<DTOVisualitzaPel> obtePelis();

	vector<DTOVisualitzaSerie> obtecapitols();

};