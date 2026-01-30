#pragma once
#include <iostream>
using namespace std;

class PassarelaUsuari {

private:
	string sobrenom;//id
	string nom;
	string contrasenya;
	string correuElectronic;
	string dataNaixement;
	string modalitatSubscripcio;

public:
	//Constuctora
	PassarelaUsuari();
	PassarelaUsuari(string nU, string sU, string cU, string ceU, string dnU, string subU);

	//Altres
	void insereix(); //{exc UsuariExisteix}{exc CorreuExisiteix}
	void modifica();
	void esborra();

	//Getters
	string obteSobrenom();
	string obteNom();
	string obteContrasenya();
	string obteCorreuElectronic();
	string obteDataNaixement();
	string obteSubscripcio();

	//Setters
	void posaSobrenom(string sobrenomU);
	void posaNom(string nomU);
	void posaContrasenya(string contraU);
	void posaCorreuElectronic(string correuU);
	void posaDataNaixement(string neixU);
	void posaSubscripcio(string subU);
};