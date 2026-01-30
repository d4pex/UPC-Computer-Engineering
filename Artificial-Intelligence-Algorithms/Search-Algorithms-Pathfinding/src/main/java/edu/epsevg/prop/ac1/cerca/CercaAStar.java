package edu.epsevg.prop.ac1.cerca;

import edu.epsevg.prop.ac1.cerca.heuristica.Heuristica;
import edu.epsevg.prop.ac1.model.*;
import edu.epsevg.prop.ac1.resultat.ResultatCerca;

import java.util.*;

/**
 * Implementació de la cerca A*.
 *
 * - LNO -> Cua de Prioritat (PriorityQueue) ordenada per f(n) = g(n) + h(n).
 * - En aquesta implementació fem servir el camp 'depth' del Node per emmagatzemar f(n).
 *   (el camp 'g' segueix sent el cost acumulat real).
 * - LNT guarda el millor 'g' vist per a cada estat (poda de camins pitjors).
 */
public class CercaAStar extends CercaBase {

    private final Heuristica heur;

    public CercaAStar(boolean usarLNT, Heuristica heur) {
        super(usarLNT);
        this.heur = heur;
    }

    @Override
    public void ferCerca(Mapa inicial, ResultatCerca rc) {

        // LNO -> Cua de Prioritat (PriorityQueue)
        // S'ordena pel valor f(n). Mantindrem f(n) dins el camp 'depth' del Node.
        Comparator<Node> cmpF = Comparator.comparingInt(node -> node.depth);
        Frontera<Node> lno = new FronteraPrioritat<>(cmpF);

        // LNT -> Map<Estat, Cost (g))
        // Guardem el cost 'g' MÍNIM per arribar a cada estat (criteri d'optimalitat d'A*).
        Map<Mapa, Integer> lnt = usarLNT ? new HashMap<>() : null;

        // Creem node inicial i l'afegim
        int g_inicial = 0;
        int h_inicial = heur.h(inicial);
        int f_inicial = g_inicial + h_inicial;

        // Constructor: Node(estat, pare, accio, depth<-f, g)
        Node nodeInicial = new Node(inicial, null, null, f_inicial, g_inicial);
        lno.push(nodeInicial);
        rc.incNodesGenerats();

        if (usarLNT) {
            lnt.put(inicial, g_inicial); // Guardem el cost 'g'
        }

        // Informem de la memòria inicial (frontera + LNT si s'utilitza)
        actualitzaMemoria(rc, lno.size(), usarLNT ? lnt.size() : 0);

        // Bucle principal A*
        while (!lno.isEmpty()) {

            // Extraiem el node amb MENOR 'f' de la cua
            Node actual = lno.pop();
            rc.incNodesExplorats();

            // Control de Cicles (LNT) - Comprovació d'optimalitat:
            // Si ja tenim un camí MÉS CURT (menor 'g') a aquest estat,
            // ignorem aquest node (que ve d'un camí pitjor).
            if (usarLNT) {
                Integer millorG = lnt.get(actual.estat);
                if (millorG != null && actual.g > millorG) {
                    rc.incNodesTallats();
                    continue;
                }
            }

            // Prova de meta
            if (actual.estat.esMeta()) {
                // SOLUCIÓ TROBADA! (Garantida òptima si h(n) és admissible)
                rc.setCami(reconstrueixCami(actual)); // helper de 'CercaBase'
                return;
            }

            // Expandir accions possibles
            for (Moviment acc : actual.estat.getAccionsPossibles()) {

                Mapa nouEstat = actual.estat.mou(acc);

                // Calculem costos
                int nou_g = actual.g + 1;        // Assumim cost d'acció = 1
                int nou_h = heur.h(nouEstat);
                int nou_f = nou_g + nou_h;       // f = g + h

                // Creem el node fill
                // ATENCIÓ: en A* el camp 'depth' del Node conté 'f'
                Node fill = new Node(nouEstat, actual, acc, nou_f, nou_g);
                
                rc.incNodesGenerats();

                boolean afegir = true; // Flag per saber si l'afegim a la LNO

                // Control de cicles
                if (usarLNT) {
                    Integer anticG = lnt.get(nouEstat);
                    if (anticG != null && anticG <= nou_g) {
                        // Ja teníem un camí IGUAL o MILLOR (més curt) fins aquest estat
                        afegir = false;
                        rc.incNodesTallats();
                    } else {
                        // Estat nou o camí MÉS CURT → actualitzem millor 'g'
                        lnt.put(nouEstat, nou_g);
                    }
                } else {
                    // Sense LNT, fem control dins la branca actual (ancestres)
                    if (existeixEnRuta(nouEstat, actual)) {
                        afegir = false;
                        rc.incNodesTallats();
                    }
                }

                // Afegim el node si no és repetit/descartat
                if (afegir) {
                    lno.push(fill);
                }
            }

            // Actualitzar memòria màxima 
            actualitzaMemoria(rc, lno.size(), usarLNT ? lnt.size() : 0);
        }

        // Si sortim del bucle -> no hi ha solució
        rc.setCami(null);
    }
}