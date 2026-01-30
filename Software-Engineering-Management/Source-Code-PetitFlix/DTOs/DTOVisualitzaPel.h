#pragma once
#include "PassarelaContingut.h"
#include "PassarelaVisualitzaPel.h"

using namespace std;

class DTOVisualitzaPel {
private:
	string data;
	string titol;
	string descripcio;
	string qualificacio;
	int num_visual;

public:
	DTOVisualitzaPel() {
		data = "";
		titol = "";
		descripcio = "";
		qualificacio = "";
		num_visual = 0;
	}

	DTOVisualitzaPel(PassarelaContingut pc, PassarelaVisualitzaPel vp) {
		data = vp.obteData();
		titol = vp.obteTitolPelicula();
		descripcio = pc.obteDescripcio();
		qualificacio = pc.obteQualificacio();
		num_visual = vp.obteNumVisualitzacions();
	}

	string obteData() const {
		return data;
	}

	string obteTitol() const {
		return titol;
	}

	string obteDescripcio() const {
		return descripcio;
	}

	string obteQualificacio() const {
		return qualificacio;
	}

	int obteNumVisual() const {
		return num_visual;
	}
};