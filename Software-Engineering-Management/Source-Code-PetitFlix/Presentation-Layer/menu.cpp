#include <iostream>
#include "CapaDePresentacio.h"
#include <locale>

using namespace std;

int main() {
    // Per poder mostrar accents per la terminal
    locale::global(std::locale("es_ES.UTF-8"));

    CapaDePresentacio* capa = CapaDePresentacio::getInstance();
    int opcio;
    do {
        cout << "*********************" << endl;
        cout << "    Menu Principal   " << endl;
        cout << "*********************" << endl;

        cout << "1. Iniciar sessió" << endl;
        cout << "2. Registrar usuari" << endl;
        cout << "3. Consultes" << endl;
        cout << "4. Sortir" << endl;
        cout << "Escriu opció: ";
        cin >> opcio;
        cout << endl;

        switch (opcio) {

        case 1: {
            // INICI SESSIO
            capa->iniciSessio();
            //  Comprobar si la sessió ha sigut iniciada, per saber a quin menu anar
            bool iniciSessio = capa->obteSessioIniciada();
            if (!iniciSessio) break;

            int opcioLoggejat;
            do {
                cout << "*********************" << endl;
                cout << "    Menu Principal   " << endl;
                cout << "*********************" << endl;

                cout << "1. Gestió usuaris" << endl;
                cout << "2. Visualitzar" << endl;
                cout << "3. Consultes" << endl;
                cout << "4. Tancar sessió" << endl;
                cout << "5. Sortir" << endl;
                cout << "Escriu opció: ";
                cin >> opcioLoggejat;
                cout << endl;

                switch (opcioLoggejat) {
                case 1: {
                    int opcioGestionarUsuari;
                    do {
                        cout << "-----------------------" << endl;
                        cout << "     Gestió Usuari     " << endl;
                        cout << "-----------------------" << endl;

                        cout << "1. Consulta usuari" << endl;
                        cout << "2. Modifica usuari" << endl;
                        cout << "3. Esborrar usuari" << endl;
                        cout << "4. Tornar" << endl;
                        cout << "Escriu opció: ";
                        cin >> opcioGestionarUsuari;
                        cout << endl;

                        switch (opcioGestionarUsuari) {
                        case 1: // Cas d'ús Consulta Usuari
                            capa->consultaUsuari();
                            cout << endl;
                            break;
                        case 2: // Cas d'ús Modifica Usuari
                            capa->modificaUsuari();
                            cout << "No fem aquest cas d'ús, ja que som un grup de dos." << endl;
                            break;
                        case 3: // Cas d'ús Esborra Usuari
                            capa->esborraUsuari();
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Opció no valida" << endl;
                        }
                    } while (opcioGestionarUsuari != 4 and capa->obteSessioIniciada());
                    break;
                }
                case 2: {
                    // Visualitzar
                    int opcioVisualitzar;
                    do {
                        cout << "---------------------------" << endl;
                        cout << "        Visualitzar        " << endl;
                        cout << "---------------------------" << endl;

                        cout << "1. Visualitzar pel·lícula" << endl;
                        cout << "2. Visualitzar capítol" << endl;
                        cout << "3. Consulta visualitzacions" << endl;
                        cout << "4. Tornar" << endl;
                        cout << "Escriu opció: ";
                        cin >> opcioVisualitzar;
                        cout << endl;

                        switch (opcioVisualitzar) {
                        case 1: // Cas d'ús Visualitzar pel·licula
                            capa->visualitzaPelicula();
                            break;
                        case 2: // Cas d'ús Visualitzar capitol
                            capa->visualitzaCapitol();
                            cout << "No fem aquest cas d'ús, ja que som un grup de dos." << endl;
                            break;
                        case 3: // Cas d'ús Consulta visualitzacions
                            capa->consultaVisualitzacions();
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Opció no valida" << endl;
                        }
                    } while (opcioVisualitzar != 4);
                    break;
                }
                case 3:
                    // Consultes
                    int opcioConsultes;
                    do {
                        cout << "---------------------------" << endl;
                        cout << "         Consultes         " << endl;
                        cout << "---------------------------" << endl;

                        cout << "1. Properes estrenes" << endl;
                        cout << "2. Ultimes novetats" << endl;
                        cout << "3. Pel·lícules més vistes" << endl;
                        cout << "4. Tornar" << endl;
                        cout << "Escriu opció: ";
                        cin >> opcioConsultes;
                        cout << endl;

                        switch (opcioConsultes) {
                        case 1: // Cas d'ús Consultar Properes estrenes
                            capa->properesEstrenes();
                            break;
                        case 2: // Cas d'ús Ultimes novetats
                            capa->ultimesNovetats();
                            cout << "No fem aquest cas d'ús, ja que som un grup de dos." << endl;
                            break;
                        case 3: // Cas d'ús Pel·lícules mes vistes
                            capa->peliculesMesVistes();
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Opció no valida" << endl;
                        }
                    } while (opcioConsultes != 4);
                    break;
                case 4:
                    // Cas d'ús Tancar sessió
                    capa->tancaSessio();
                    break;
                case 5:
                    //Sortir
                    //TODO
                    break;
                default:
                    cout << "Opció no valida" << endl;
                }
            } while (opcioLoggejat != 5 and capa->obteSessioIniciada());
            break;
        }
        case 2:
            // Registrar usuari
            capa->registrarUsuari();
            break;
        case 3:
            // Consultes
            int opcioConsultes;
            do {
                cout << "---------------------------" << endl;
                cout << "         Consultes         " << endl;
                cout << "---------------------------" << endl;

                cout << "1. Properes estrenes" << endl;
                cout << "2. Últimes novetats" << endl;
                cout << "3. Pel·lícules més vistes" << endl;
                cout << "4. Tornar" << endl;
                cout << "Escriu opció: ";
                cin >> opcioConsultes;
                cout << endl;

                switch (opcioConsultes) {
                case 1: // Cas d'ús Consultar Properes estrenes
                    capa->properesEstrenes();
                    break;
                case 2: // Cas d'ús Ultimes novetats
                    capa->ultimesNovetats();
                    cout << "No fem aquest cas d'ús, ja que som un grup de dos." << endl;
                    break;
                case 3: // Cas d'ús Pel·lícules mes vistes
                    capa->peliculesMesVistes();
                    break;
                case 4:
                    // Tornar
                    break;
                default:
                    cout << "Opció no valida" << endl;
                }
            } while (opcioConsultes != 4);
            break;
        case 4:
            // Sortir
            break;
        default:
            cout << "Opció no valida" << endl;
        }
    } while (opcio != 4);
    return 0;
}
