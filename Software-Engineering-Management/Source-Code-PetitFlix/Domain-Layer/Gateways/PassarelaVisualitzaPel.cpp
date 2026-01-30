#include "PassarelaVisualitzaPel.h"

// Constructor por defecto
PassarelaVisualitzaPel::PassarelaVisualitzaPel() {
    // Inicializamos las variables con valores vacíos (por defecto).
    sobrenom = "";
    titolPelicula = "";
    data = "";
    numVisualitzacions = 0;
}

// Métodos Getter

string PassarelaVisualitzaPel::obteSobrenom() const {
    return sobrenom;
}

string PassarelaVisualitzaPel::obteTitolPelicula() const {
    return titolPelicula;
}

string PassarelaVisualitzaPel::obteData() const {
    return data;
}

int PassarelaVisualitzaPel::obteNumVisualitzacions() const {
    return numVisualitzacions;
}

// Métodos Setter

void PassarelaVisualitzaPel::posaSobrenom(string _sobrenom) {
    sobrenom = _sobrenom;
}

void PassarelaVisualitzaPel::posaTitolPelicula(string _titolPelicula) {
    titolPelicula = _titolPelicula;
}

void PassarelaVisualitzaPel::posaData(string _data) {
    data = _data;
}

void PassarelaVisualitzaPel::posaNumVisualitzacions(int _numVisualitzacions) {
    numVisualitzacions = _numVisualitzacions;
}

// Métodos Auxiliares

void PassarelaVisualitzaPel::insereix() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    char data[100];
    strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", ltm);
    string dataActual = string(data);

    string consulta = "SELECT * FROM visualitzacio_pelicula WHERE sobrenom_usuari = '" + sobrenom + "' AND titol_pelicula = '" + titolPelicula + "'";
    sql::ResultSet* res = ConnexioBD::getInstance()->consulta(consulta);

    if (res->next()) {
        // Si l'usuari ja ha vist la pel·lícula, actualitzar la data i el comptador de visualitzacions
        string updateQuery = "UPDATE visualitzacio_pelicula SET data = '" + dataActual + "', num_visualitzacions = num_visualitzacions + 1 "
            "WHERE sobrenom_usuari = '" + sobrenom + "' AND titol_pelicula = '" + titolPelicula + "'";
        ConnexioBD::getInstance()->executa(updateQuery);
    }
    else {
        // Si no existeix el registre, inserir una nova visualització
        string insertQuery = "INSERT INTO visualitzacio_pelicula (sobrenom_usuari, titol_pelicula, data, num_visualitzacions) "
            "VALUES ('" + sobrenom + "', '" + titolPelicula + "', '" + dataActual + "', 1)";
        ConnexioBD::getInstance()->executa(insertQuery);
    }

    posaData(dataActual);
    
    numVisualitzacions++;
}
