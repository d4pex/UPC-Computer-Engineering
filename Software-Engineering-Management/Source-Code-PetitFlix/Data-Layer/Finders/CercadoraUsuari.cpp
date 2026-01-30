#include "CercadoraUsuari.h"
#include "PassarelaUsuari.h"
#include "ConnexioBD.h"
#include <iostream>


using namespace std;

PassarelaUsuari CercadoraUsuari::cercaUsuari(string sobrenomU) {
	PassarelaUsuari u;

	ConnexioBD* con = ConnexioBD::getInstance();

	string sql = "SELECT * FROM usuari WHERE sobrenom = '" + sobrenomU + "'";
	sql::ResultSet* res = con->consulta(sql);
	// Si no troba cap fila, activa excepció
	if (!res->next()) {		// exc: UsuariNoExisteix
		throw runtime_error("Hi ha hagut un error amb el sobrenom o la contrasenya");
	}
	else {
		u.posaSobrenom(res->getString("sobrenom"));
		u.posaNom(res->getString("nom"));
		u.posaContrasenya(res->getString("contrasenya"));
		u.posaCorreuElectronic(res->getString("correu_electronic"));
		u.posaDataNaixement(res->getString("data_naixement"));
		u.posaSubscripcio(res->getString("modalitat"));
		delete res;
	}
	return u;
}