#pragma once
#include "PassarelaUsuari.h"

class DTOUsuari {
private:
	string sobrenom;
	string nom;
	string contrasenya;
	string correuElectronic;
	string dataNaixement;
	string subscripcio;

public:
	DTOUsuari() {
		sobrenom = "";
		nom = "";
		contrasenya = "";
		correuElectronic = "";
		dataNaixement = "";
		subscripcio = "";
	}

	DTOUsuari(PassarelaUsuari usu) {
		sobrenom = usu.obteSobrenom();
		nom = usu.obteNom();
		contrasenya = usu.obteContrasenya();
		correuElectronic = usu.obteCorreuElectronic();
		dataNaixement = usu.obteDataNaixement();
		subscripcio = usu.obteSubscripcio();
	}
	// Constructor por copia
	DTOUsuari(const DTOUsuari& aux) {
		sobrenom = aux.sobrenom;
		nom = aux.nom;
		contrasenya = aux.contrasenya;
		correuElectronic = aux.correuElectronic;
		dataNaixement = aux.dataNaixement;
		subscripcio = aux.subscripcio;
	}

	~DTOUsuari() {
		// No hay recursos dinámicos que liberar, 
		// pero este destructor se incluye por claridad.
	}

	string obteSobrenom() const {
		return sobrenom;
	}

	string obteNom() const {
		return nom;
	}

	string obteContrasenya() const {
		return contrasenya;
	}

	string obteCorreuElectronic() const {
		return correuElectronic;
	}

	string obteDataNaixement() const {
		return dataNaixement;
	}

	string obteSubscripcio() const {
		return subscripcio;
	}

};
