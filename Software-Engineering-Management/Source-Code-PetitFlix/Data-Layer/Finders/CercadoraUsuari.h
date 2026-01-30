#pragma once  //  compilador sabe que no debe incluir ese archivo más de una vez en el mismo archivo de compilación. Esto es útil para evitar la duplicación de definiciones, lo que podría causar errores de compilación (por ejemplo, redefinición de clases o funciones).
#include <iostream>
#include "PassarelaUsuari.h"
using namespace std;

class CercadoraUsuari
{
private:

public:
	PassarelaUsuari cercaUsuari(string sobrenomU);
};