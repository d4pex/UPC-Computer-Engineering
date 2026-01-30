#include "CapaDePresentacio.h"

// Definició de la variable estàtica
CapaDePresentacio* CapaDePresentacio::instancePtr = nullptr;

CapaDePresentacio::CapaDePresentacio() {}

CapaDePresentacio* CapaDePresentacio::getInstance() {
    if (instancePtr == nullptr) {
        instancePtr = new CapaDePresentacio();
    }
    return instancePtr;
}

void CapaDePresentacio::iniciSessio() {
    string sU;
    string cU;
    cout << "** Inici sessio **" << endl;
    cout << "Sobrenom: ";
    cin >> sU;
    cout << "Contrasenya: ";
    cin >> cU;
    try {
        TxIniciSessio txI(sU, cU);
        txI.executar();
        cout << "Sessio iniciada correctament!" << endl << endl;
        sessioIniciada = true;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl << endl;
    }
}

void CapaDePresentacio::tancaSessio() {
    cout << "** Tancar sessio **" << endl;
    char c = ' ';
    cout << "Vols tancar la sessio (S/N): ";
    cin >> c;
    cout << endl;
    if (c == 'S') {
        TxTancaSessio txS;
        txS.executar();
        cout << "Sessio tancada correctament!" << endl << endl;
        sessioIniciada = false;
    }
    return;
}

void CapaDePresentacio::registrarUsuari() {
    string nU;
    string sU;
    string cU;
    string ceU;
    string dnU;
    int modU;
    cout << "** Registrar usuari **" << endl;
    cout << "Nom complet: ";
    cin >> nU;
    cout << "Sobrenom: ";
    cin >> sU;
    cout << "Contrasenya: ";
    cin >> cU;
    cout << "Correu electronic: ";
    cin >> ceU;
    cout << "Data naixament (DD/MM/AAAA): ";
    cin >> dnU;
    stringstream ss(dnU);
    string dia, mes, any;
    getline(ss, dia, '/');
    getline(ss, mes, '/');
    getline(ss, any);
    dnU = any + "-" + mes + "-" + dia;
    cout << "Modalitats de subscripcio disponibles: " << endl;
    cout << " > 1. Completa" << endl;
    cout << " > 2. Cinefil" << endl;
    cout << " > 3. Infantil" << endl;

    cout << "Escull modalitat: ";
    cin >> modU;
    cout << endl;

    if (modU < 1 || modU > 3) { // exc: ModalitatNoExisteix
        cout << "Error: Opcio no valida. Si us plau, tria 1, 2 o 3 per la subscripcio." << endl << endl;
        return;
    }

    string modalitatU;
    if (modU == 1) modalitatU = "Completa";
    else if (modU == 2) modalitatU = "Cinefil";
    else modalitatU = "Infantil";

    try {
        TxRegistraUsuari txR(nU, sU, cU, ceU, dnU, modalitatU);
        txR.executar();
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl << endl;
    }
}

void CapaDePresentacio::consultaUsuari() {
    cout << "** Consulta usuari **" << endl;
    DTOUsuari infoUsuari;
    TxConsultaUsuari txC;
    txC.executar();
    infoUsuari = txC.obteResultat();
    cout << "Nom complet: " << infoUsuari.obteNom() << endl;
    cout << "Sobrenom: " << infoUsuari.obteSobrenom() << endl;
    cout << "Correu electronic: " << infoUsuari.obteCorreuElectronic() << endl;
    string dataU = infoUsuari.obteDataNaixement();
    string dataNaixement = dataU.substr(8, 2) + "/" + dataU.substr(5, 2) + "/" + dataU.substr(0, 4);
    cout << "Data naixement (DD/MM/AAAA): " << dataNaixement << endl;
    cout << "Modalitat subscripcio: " << infoUsuari.obteSubscripcio() << endl;

    TxInfoVisualitzacions txIV;
    txIV.executar();

    int pelisvisualitzades = txIV.obteResultatpelis();
    int capitolsvisualitzats = txIV.obteResultatserie();

    cout << endl;
    cout << pelisvisualitzades;
    cout << " pel.licules visualitzades" << endl;
    cout << capitolsvisualitzats;
    cout << " capitols visualitzats" << endl;
}

void CapaDePresentacio::modificaUsuari() {}

void CapaDePresentacio::esborraUsuari() {
    cout << "** Esborrar usuari **" << endl;
    cout << "Per confirmar l'esborrat, s'ha d'entrar la contrasenya ..." << endl;
    cout << "Contrasenya: ";
    string contrasenya;
    cin >> contrasenya;

    try {
        TxEsborraUsuari txeU(contrasenya);
        txeU.executar();
        cout << "Usuari esborrat correctament!" << endl << endl;
        sessioIniciada = false;
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl << endl;
    }
}

void CapaDePresentacio::visualitzaPelicula() {
    cout << "** Visualitzar Pelicula **" << endl;
    cout << "Nom pelicula: ";
    string nomP;
    cin.ignore(); // Ignorar el salto de línea anterior
    getline(cin, nomP);
    cout << endl;

    CtrlVisualitzaPelicula cVP;

    try {
        // Consulta la información del usuario
        DTOUsuari infoUsuari = cVP.consultaUsuari();

        // Consulta la información de la película
        DTOContingut infoContingut;
        DTOPelicula infoPelicula;
        cVP.consultaPelicula(nomP, infoContingut, infoPelicula);

        // Comprueba la edad del usuario
        cVP.comprovaEdat(infoUsuari, infoContingut);

        // Comprueba si la película ha sido estrenada
        cVP.comprovaEstrena(infoPelicula);

        // Muestra la información de la película
        cout << "Informacio pelicula ..." << endl;
        cout << "Nom pelicula: " << nomP << endl;
        cout << "Descripcio: " << infoContingut.obteDescripcio() << endl;
        cout << "Qualificacio edat: " << infoContingut.obteQualificacio() << endl;
        string dataE = infoPelicula.obteDataEstrena();
        string dataEstrena = dataE.substr(8, 2) + "/" + dataE.substr(5, 2) + "/" + dataE.substr(0, 4);
        cout << "Data estrena: " << dataEstrena << endl;
        cout << "Duracio: " << infoPelicula.obteDuracio() << endl;
        char c = ' ';
        cout << "Vols continuar amb la visualitzacio (S/N): ";
        cin >> c;
        cout << endl;

        if (c == 'S') {
            string dataVisualitzacio = cVP.visualitzaPelicula(infoUsuari, infoPelicula);
            dataVisualitzacio = dataVisualitzacio.substr(0, 16);
            cout << "Visualitzacio registrada: " << dataVisualitzacio << endl;

            vector<vector<DTOPelicula>> peliculesRelacionades = cVP.peliculesRelacionades(infoPelicula);
            if (peliculesRelacionades.empty()) {
                cout << "No n'hi ha ninguna pelicula relacionada amb la pelicula " << nomP << endl;
            }
            else {
                cout << "Pelicules relacionades: " << endl;

                for (unsigned int i = 0; i < peliculesRelacionades.size(); i++) {
                    cout << "- ";
                    for (unsigned int j = 0; j < peliculesRelacionades[i].size(); j++) {
                        DTOPelicula dto = peliculesRelacionades[i][j];

                        cout << dto.obteTitol() << "; ";
                        cout << dto.obteDescripcio() << "; ";
                        cout << dto.obteQualificacio() << "; ";
                        cout << dto.obteDuracio() << " min; ";

                        string dataEstrena = dto.obteDataEstrena();
                        string dataOriginal = dataEstrena.substr(0, 10);
                        stringstream ss(dataOriginal);
                        string any, mes, dia;
                        getline(ss, any, '-');
                        getline(ss, mes, '-');
                        getline(ss, dia);
                        string dataConvertida = dia + "/" + mes + "/" + any;
                        cout << dataConvertida << endl;
                    }
                }
            }
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl << endl;
    }
}

void CapaDePresentacio::visualitzaCapitol() {}

void CapaDePresentacio::consultaVisualitzacions() {
    cout << "** Consulta visualitzacions **" << endl;
    cout << endl;
    try {
        TxConsultarVisualitzacions txCV;
        txCV.executar();
        cout << "** Visualitzacions pel.licules **" << endl;
        cout << "*********************************" << endl;
        vector <DTOVisualitzaPel> vp;
        vp = txCV.obtePelis();
        for (int i = 0; i < vp.size(); i++) {
            string data = vp[i].obteData();
            string any = data.substr(0, 4);
            string mes = data.substr(5, 2);
            string dia = data.substr(8, 2);
            string hora = data.substr(11, 5);
            data = dia + "/" + mes + "/" + any + " " + hora;
            string titol = vp[i].obteTitol();
            string descripcio = vp[i].obteDescripcio();
            string qualificacio = vp[i].obteQualificacio();
            int num_visual = vp[i].obteNumVisual();
            cout << data << ": " << titol << "; " << descripcio << "; " << qualificacio << "; nombre de visualitzacions: " << num_visual << endl;
        }

        cout << endl;
        cout << "** Visualitzacions series **" << endl;
        cout << "****************************" << endl;
        vector <DTOVisualitzaSerie> vs;
        vs = txCV.obtecapitols();
        for (int i = 0; i < vs.size(); i++) {
            string data = vs[i].obteData();
            string any = data.substr(0, 4);
            string mes = data.substr(5, 2);
            string dia = data.substr(8, 2);
            string hora = data.substr(11, 5);
            data = dia + "/" + mes + "/" + any + " " + hora;
            string titol = vs[i].obteTitol();
            int temporada = vs[i].obteTemporada();
            int capitol = vs[i].obteCapitol();
            string qualificacio = vp[i].obteQualificacio();
            int num_visual = vs[i].obteNumVisual();
            cout << "- " << data << ": " << titol << "; " << qualificacio << "; Temporada " << temporada << ", capitulo " << capitol << "; nombre de visualitzacions: " << num_visual << endl;
        }

    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void CapaDePresentacio::properesEstrenes() {
    cout << "** Properes estrenes **" << endl;
    string modalitat;
    if (sessioIniciada == false) {
        cout << "Modalitat: ";
        cin >> modalitat;
        if (modalitat != "Completa" && modalitat != "Cinefil" && modalitat != "Infantil") {
            cout << endl;
            cout << "Modalitat introduida incorrecte. Opcions: Completa, Cinefil i Infantil." << endl << endl;
            return;
        }
    }
    else {
        DTOUsuari infoUsuari;
        TxConsultaUsuari txC;
        txC.executar();
        infoUsuari = txC.obteResultat();
        modalitat = infoUsuari.obteSubscripcio();
    }

    cout << endl << endl;
    TxProperesEstrenes txPE(modalitat);
    txPE.executar();

    vector<DTOPelicula> properesEstrenesPelicules = txPE.obteResultatPelicules();
    vector<DTOCapitol> properesEstrenesCapitols = txPE.obteResultatCapitols();

    if (properesEstrenesPelicules.empty() && properesEstrenesCapitols.empty()) {
        cout << "No n'hi han properes estrenes, totes les pelicules i series disponibles al nostre sistema, estan ja estrendas." << endl;
    }

    vector<pair<string, string>> properesEstrenes;
    vector<string> capítolsUnics;
    vector<string> peliculesUniques;

    for (unsigned int i = 0; i < properesEstrenesPelicules.size(); ++i) {
        string dataEstrena = properesEstrenesPelicules[i].obteDataEstrena();
        string dataOriginal = dataEstrena.substr(0, 10);
        stringstream ss(dataOriginal);
        string any, mes, dia;
        getline(ss, any, '-');
        getline(ss, mes, '-');
        getline(ss, dia);
        string dataConvertida = dia + "/" + mes + "/" + any;

        string infoPelicula = "[Pelicula]; " + properesEstrenesPelicules[i].obteTitol() + "; " + properesEstrenesPelicules[i].obteQualificacio() + "; " + to_string(properesEstrenesPelicules[i].obteDuracio()) + " min.";

        if (find(peliculesUniques.begin(), peliculesUniques.end(), properesEstrenesPelicules[i].obteTitol()) == peliculesUniques.end()) {
            properesEstrenes.push_back(make_pair(dataEstrena, infoPelicula));
            peliculesUniques.push_back(properesEstrenesPelicules[i].obteTitol());
        }
    }

    for (unsigned int i = 0; i < properesEstrenesCapitols.size(); ++i) {
        string dataEstrena = properesEstrenesCapitols[i].obteDataEstrena();
        string dataOriginal = dataEstrena.substr(0, 10);
        stringstream ss(dataOriginal);
        string any, mes, dia;
        getline(ss, any, '-');
        getline(ss, mes, '-');
        getline(ss, dia);
        string dataConvertida = dia + "/" + mes + "/" + any;

        string infoCapitol = "[Serie]; " + properesEstrenesCapitols[i].obteTitolSerie() + "; " + properesEstrenesCapitols[i].obteQualificacio() + "; Temporada " + to_string(properesEstrenesCapitols[i].obteNumeroTemporada()) + ".";

        string claveUnica = properesEstrenesCapitols[i].obteTitolSerie() + "-" + to_string(properesEstrenesCapitols[i].obteNumeroTemporada());

        bool jaExisteix = false;
        for (const string& capitulo : capítolsUnics) {
            if (capitulo == claveUnica) {
                jaExisteix = true;
                break;
            }
        }

        if (!jaExisteix) {
            properesEstrenes.push_back(make_pair(dataEstrena, infoCapitol));
            capítolsUnics.push_back(claveUnica);
        }
    }

    sort(properesEstrenes.begin(), properesEstrenes.end(), [](const pair<string, string>& a, const pair<string, string>& b) {
        return a.first < b.first;
        });

    int contador = 1;
    for (unsigned int i = 0; i < properesEstrenes.size(); ++i) {
        string dataEstrena = properesEstrenes[i].first;
        string dataOriginal = dataEstrena.substr(0, 10);
        stringstream ss(dataOriginal);
        string any, mes, dia;
        getline(ss, any, '-');
        getline(ss, mes, '-');
        getline(ss, dia);
        string dataConvertida = dia + "/" + mes + "/" + any;

        cout << contador << ".- ";
        cout << dataConvertida << " ";
        cout << properesEstrenes[i].second << endl;

        contador++;
    }
}

void CapaDePresentacio::ultimesNovetats() {}

void CapaDePresentacio::peliculesMesVistes() {
    cout << "** Pelicules mes visualitzades **" << endl << endl;
    vector<DTOPeliMesVisual> peliculesMesVistes;
    int contador = 1;
    TxPeliculesMesVistes txPMV(sessioIniciada);
    txPMV.executar();
    peliculesMesVistes = txPMV.obtePelis();
    for (int i = 0; i < peliculesMesVistes.size(); i++) {
        string titol = peliculesMesVistes[i].obteTitol();
        string qualificacio = peliculesMesVistes[i].obteQualificacio();
        int duracio = peliculesMesVistes[i].obteDuracio();
        int obteNumVisual = peliculesMesVistes[i].obteNumVisual();

        cout << contador << ".- " << titol << "; " << qualificacio << "; " << duracio << " min.; Visualitzacions: " << obteNumVisual;

        string data = peliculesMesVistes[i].obteData();
        if (data != "") {
            string any = data.substr(0, 4);
            string mes = data.substr(5, 2);
            string dia = data.substr(8, 2);
            data = dia + "/" + mes + "/" + any; 
            cout << " [VISTA: " << data << "]";
        }
        cout << endl;
        contador++;
    }
}

bool CapaDePresentacio::obteSessioIniciada() const {
    return sessioIniciada;
}