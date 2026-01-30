#pragma once
#include "ConnexioBD.h"
#include "PassarelaCapitol.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class CercadoraCapitol
{
private:

public:
	vector<vector<PassarelaCapitol>> cercaPropersCapitols();
};