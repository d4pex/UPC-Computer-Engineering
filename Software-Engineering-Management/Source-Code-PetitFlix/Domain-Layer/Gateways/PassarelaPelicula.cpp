#include "PassarelaPelicula.h"

// Constructor
PassarelaPelicula::PassarelaPelicula() {
    titol = "";
    descripcio = "";
    qualificacio = "";
    dataEstrena = "";
    duracio = 0;
}

// Getters
string PassarelaPelicula::obteTitol() const {
    return titol;
}

string PassarelaPelicula::obteDescripcio() const {
    return descripcio;
}

string PassarelaPelicula::obteQualificacio() const {
    return qualificacio;
}

string PassarelaPelicula::obteDataEstrena() const {
    return dataEstrena;
}

int PassarelaPelicula::obteDuracio() const {
    return duracio;
}

// Setters
void PassarelaPelicula::posaTitol(string _titol) {
    titol = _titol;
}

void PassarelaPelicula::posaDescripcio(string _descripcio) {
    descripcio = _descripcio;
}

void PassarelaPelicula::posaQualificacio(string _qualificacio) {
    qualificacio = _qualificacio;
}

void PassarelaPelicula::posaDataEstrena(string _dataEstrena) {
    dataEstrena = _dataEstrena;
}

void PassarelaPelicula::posaDuracio(int _duracio) {
    duracio = _duracio;
}
