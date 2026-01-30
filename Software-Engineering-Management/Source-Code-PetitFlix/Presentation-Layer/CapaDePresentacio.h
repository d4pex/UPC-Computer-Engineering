#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "TxIniciSessio.h"
#include "TxTancaSessio.h"
#include "TxRegistraUsuari.h"
#include "TxEsborraUsuari.h"
#include "TxConsultaUsuari.h"
#include "TxInfoVisualitzacions.h"
#include "CtrlVisualitzaPelicula.h"
#include "TxConsultarVisualitzacions.h"
#include "TxProperesEstrenes.h"
#include "TxPeliculesMesVistes.h"
#include "DTOUsuari.h"
#include "DTOPelicula.h"
#include "DTOContingut.h"
#include "DTOCapitol.h"
#include "DTOPeliMesVisual.h"

class CapaDePresentacio {
private:
    bool sessioIniciada = false;

    static CapaDePresentacio* instancePtr;
    CapaDePresentacio();

public:
    CapaDePresentacio(const CapaDePresentacio& obj) = delete;

    static CapaDePresentacio* getInstance();

    void iniciSessio();
    void tancaSessio();
    void registrarUsuari();
    void consultaUsuari();
    void modificaUsuari();
    void esborraUsuari();
    void visualitzaPelicula();
    void visualitzaCapitol();
    void consultaVisualitzacions();
    void properesEstrenes();
    void ultimesNovetats();
    void peliculesMesVistes();

    bool obteSessioIniciada() const;
};