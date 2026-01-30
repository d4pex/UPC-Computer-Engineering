#pragma once 
#include <iostream>
#include "PassarelaVisualitzaSerie.h"
#include <vector>
using namespace std;

class CercadoraVisualitzaSerie {
private:

public:
	// Constructor
	CercadoraVisualitzaSerie();

	// Metode
	vector<PassarelaVisualitzaSerie> cercaElemen(string sobrenomU);

};