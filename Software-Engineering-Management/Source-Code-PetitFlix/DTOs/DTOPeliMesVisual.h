#pragma once
#include "PassarelaPelicula.h"
#include "PassarelaVisualitzaPel.h"
#include "PassarelaContingut.h"

using namespace std;

class DTOPeliMesVisual {
private:
	string titol;
	string qualificacio;
	int duracio;
	int num_visual;
	string data;

public:
	// Constructora por defecto
	DTOPeliMesVisual() {
		titol = "";
		qualificacio = "";
		duracio = 0;
		num_visual = 0;
		data = "";
	}

	// Constructora con parámetros
	DTOPeliMesVisual(PassarelaContingut pc, PassarelaPelicula pp, PassarelaVisualitzaPel vp) {
		titol = vp.obteTitolPelicula();
		qualificacio = pc.obteQualificacio();
		duracio = pp.obteDuracio();
		num_visual = vp.obteNumVisualitzacions();
	}

	void posaData(PassarelaVisualitzaPel vp) {
		data = vp.obteData();
	}

	// Getters
	string obteTitol() const {
		return titol;
	}

	string obteQualificacio() const {
		return qualificacio;
	}

	int obteDuracio() const {
		return duracio;
	}

	int obteNumVisual() const {
		return num_visual;
	}
	string obteData() const {
		return data;
	}
};