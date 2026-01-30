#pragma once
#include "PassarelaContingut.h"
#include "PassarelaVisualitzaSerie.h"

using namespace std;

class DTOVisualitzaSerie {
private:
    string data;
    string titol;
    int temporada;
    int capitol;
    string qualificacio;
    int num_visual;

public:
    DTOVisualitzaSerie() {
        data = "";
        titol = "";
        temporada = 0;
        capitol = 0;
        qualificacio = "";
        num_visual = 0;
    }

    DTOVisualitzaSerie(PassarelaContingut pc, PassarelaVisualitzaSerie vs) {
        data = vs.obteData();
        titol = vs.obteTitolSerie();
        temporada = vs.obteNumTemporada();
        capitol = vs.obteNumCapitol();
        qualificacio = pc.obteQualificacio();
        num_visual = vs.obteNumVisualitzacions();
    }

    string obteData() const {
        return data;
    }

    string obteTitol() const {
        return titol;
    }

    int obteTemporada() const {
        return temporada;
    }

    int obteCapitol() const {
        return capitol;
    }

    string obteQualificacio() const {
        return qualificacio;
    }

    int obteNumVisual() const {
        return num_visual;
    }

};
