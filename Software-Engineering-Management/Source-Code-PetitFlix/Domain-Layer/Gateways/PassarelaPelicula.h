#pragma once
#include <iostream>
using namespace std;

class PassarelaPelicula {
private:
    string titol;
    string descripcio;
    string qualificacio;
    string dataEstrena;
    int duracio;

public:
    // Constructor
    PassarelaPelicula();

    // Getters
    string obteTitol() const;
    string obteDescripcio() const;
    string obteQualificacio() const;
    string obteDataEstrena() const;
    int obteDuracio() const;

    // Setters
    void posaTitol(string _titol);
    void posaDescripcio(string _descripcio);
    void posaQualificacio(string _qualificacio);
    void posaDataEstrena(string _dataEstrena);
    void posaDuracio(int _duracio);
};
