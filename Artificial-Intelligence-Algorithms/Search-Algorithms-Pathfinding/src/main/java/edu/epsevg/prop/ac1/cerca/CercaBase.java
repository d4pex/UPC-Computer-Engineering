package edu.epsevg.prop.ac1.cerca;

import edu.epsevg.prop.ac1.model.Mapa;
import edu.epsevg.prop.ac1.model.Moviment;
import edu.epsevg.prop.ac1.resultat.ResultatCerca;

import java.util.*;

/**
 * Superclasse base amb utilitats compartides per BFS, DFS, IDS i A*.
 * Proporciona:
 *  - Control de cicles per ruta.
 *  - Gestió de LNT per iteració/execució (amb poda per profunditat mínima).
 *  - Reconstrucció de camí.
 *  - Actualització de memòria pic.
 *  - Creació de successors (Node fill).
 */
public abstract class CercaBase extends Cerca {

    protected CercaBase(boolean usarLNT) {
        super(usarLNT);
    }

    /** Comprova si 'estat' ja és a la ruta (pujant per 'pare'). */
    protected boolean existeixEnRuta(Mapa estat, Node pare) {
        for (Node it = pare; it != null; it = it.pare) {
            if (estat.equals(it.estat)) return true;
        }
        return false;
    }

    /**
     * Aplica LNT: si ja hem vist 'estat' a una profunditat <= depth → poda (retorna true).
     * Si no, registra 'estat' amb 'depth' i retorna false.
     */
    protected boolean podaPerLNT(Mapa estat, int depth, Map<Mapa,Integer> lnt) {
        Integer millor = lnt.get(estat);
        if (millor != null && millor <= depth) return true;
        lnt.put(estat, depth);
        return false;
    }

    /** Reconstrueix el camí (de l’arrel a la meta) a partir d’un node final. */
    protected List<Moviment> reconstrueixCami(Node nodeFinal) {
        LinkedList<Moviment> cami = new LinkedList<>();
        for (Node it = nodeFinal; it != null && it.pare != null; it = it.pare) {
            cami.addFirst(it.accio);
        }
        return cami;
    }

    /** Actualitza la memòria pic a partir de mides de frontera i LNT. */
    protected void actualitzaMemoria(ResultatCerca rc, int midaFrontera, int midaLnt) {
        rc.updateMemoria(midaFrontera + midaLnt);
    }

    /** Crea el node fill (depth i g incrementen en +1, cost uniforme). */
    protected Node creaFill(Node pare, Moviment accio, Mapa nouEstat) {
        return new Node(nouEstat, pare, accio, pare.depth + 1, pare.g + 1);
    }
}
