#pragma once
#include <iostream>
#include <string>
#include "ConnexioBD.h"
#include <ctime>
#include <iomanip>
using namespace std;

class PassarelaVisualitzaPel {
private:
	string sobrenom;
	string titolPelicula;
	string data;
	int numVisualitzacions;

public:
	PassarelaVisualitzaPel();

	// Getters 
	string obteSobrenom() const;
	string obteTitolPelicula() const;
	string obteData() const;
	int obteNumVisualitzacions() const;

	void posaSobrenom(string _sobrenom);
	void posaTitolPelicula(string _titolPelicula);
	void posaData(string _data);
	void posaNumVisualitzacions(int _numVisualitzacions);

	void insereix();
};