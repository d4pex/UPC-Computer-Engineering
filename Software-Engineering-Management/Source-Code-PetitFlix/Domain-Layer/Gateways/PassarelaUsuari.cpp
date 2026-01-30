#include "PassarelaUsuari.h"
#include "PetitFlix.h"
#include "ConnexioBD.h"
#include <fstream>

using namespace std;
PassarelaUsuari::PassarelaUsuari() {
	nom = "";
	sobrenom = "";
	contrasenya = "";
	correuElectronic = "";
	dataNaixement = "";
	modalitatSubscripcio = "";
}

PassarelaUsuari::PassarelaUsuari(string nU, string sU, string cU, string ceU, string dnU, string subU) {
	nom = nU;
	sobrenom = sU;
	contrasenya = cU;
	correuElectronic = ceU;
	dataNaixement = dnU;
	modalitatSubscripcio = subU;
}

void PassarelaUsuari::insereix() {
	try {
		ConnexioBD* con = ConnexioBD::getInstance();

		string sql = "SELECT * FROM usuari WHERE sobrenom = '" + sobrenom + "'";
		sql::ResultSet* res1 = con->consulta(sql);

		if (res1->next()) {	// exc: SobrenomExisteix
			throw runtime_error("Ja existeix un usuari amb aquest sobrenom");
		}

		sql = "SELECT * FROM usuari WHERE correu_electronic = '" + correuElectronic + "'";
		sql::ResultSet* res2 = con->consulta(sql);
		if (res2->next()) {	// exc: CorreuExisteix
			throw runtime_error("Ja existeix un usuari amb aquest correu electonic");
		}

		sql = "INSERT INTO usuari (sobrenom, nom, contrasenya, correu_electronic, data_naixement, modalitat) VALUES ('"
			+ sobrenom + "', '" + nom + "', '" + contrasenya + "', '"
			+ correuElectronic + "', '" + dataNaixement + "', '"
			+ modalitatSubscripcio + "')";

		con->executa(sql);
		cout << "Nou usuari registrat correctament!" << endl << endl;
	}
	catch (const sql::SQLException& e) {
		cerr << "Error SQL: " << e.what() << endl;
	}
	catch (const std::exception& e) {
		cerr << "Error al registrar l'usuari: " << e.what() << endl;
	}
}

void PassarelaUsuari::modifica() {}

void PassarelaUsuari::esborra() {
	try {
		ConnexioBD* con = ConnexioBD::getInstance();

		string sql = "SELECT * FROM usuari WHERE sobrenom = '" + sobrenom + "'";
		sql::ResultSet* res = con->consulta(sql);

		if (!res->next()) { // exc: UsuariNoExisteix
			throw runtime_error("No existeix cap usuari amb aquest sobrenom");
		}

		sql = "DELETE FROM usuari WHERE sobrenom = '" + sobrenom + "'";

		con->executa(sql);
	}
	catch (const sql::SQLException& e) {
		cerr << "Error SQL: " << e.what() << endl << endl;
	}
	catch (const exception& e) {
		cerr << "Error al esborrar l'usuari: " << e.what() << endl << endl;
	}
}

string PassarelaUsuari::obteSobrenom() {
	return sobrenom;
}

string PassarelaUsuari::obteContrasenya() {
	return contrasenya;
}

string PassarelaUsuari::obteNom() {
	return nom;
}

string PassarelaUsuari::obteCorreuElectronic() {
	return correuElectronic;
}

string PassarelaUsuari::obteDataNaixement() {
	return dataNaixement;
}

string PassarelaUsuari::obteSubscripcio() {
	return modalitatSubscripcio;
}


void PassarelaUsuari::posaSobrenom(string sobrenomU) {
	sobrenom = sobrenomU;
}

void PassarelaUsuari::posaNom(string nomU) {
	nom = nomU;
}

void PassarelaUsuari::posaContrasenya(string contraU) {
	contrasenya = contraU;
}

void PassarelaUsuari::posaCorreuElectronic(string correuU) {
	correuElectronic = correuU;
}

void PassarelaUsuari::posaDataNaixement(string neixU) {
	dataNaixement = neixU;
}

void PassarelaUsuari::posaSubscripcio(string subU) {
	modalitatSubscripcio = subU;
}