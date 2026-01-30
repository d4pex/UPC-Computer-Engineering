package edu.epsevg.prop.lab.c4;

/**
 * Jugador basat en Minimax amb poda Alfa-Beta per al joc Connecta 4.
 * VERSIÓ MILLORADA: Inclou ordenació de columnes (velocitat) i heurística posicional.
 *
 * @author Alumne
 */
public class JugadorMinMax implements Jugador, IAuto {

    // Usem un valor molt alt (1.000.000.000) per evitar desbordaments (overflow) al sumar/restar profunditats
    private static final int INFINIT_POSITIU = 1000000000;
    private static final int INFINIT_NEGATIU = -INFINIT_POSITIU;

    /** Profunditat màxima de cerca de l'algorisme */
    private final int profunditatMaxima;

    /** Nom que es mostrarà a la interfície gràfica */
    private String nomDelJugador;

    /** El meu color de fitxa en aquesta partida (1 o -1) */
    private int meuColor;

    /** Color de la fitxa del rival (l'oposat al meu) */
    private int colorRival;

    /** Estadístiques per analitzar el rendiment (nodes fulla visitats) */
    private long nodesExplorats;
    private long podesAlphaBeta;

    /**
     * Constructor del jugador.
     * @param profunditat Profunditat màxima de l'arbre de cerca (nivells).
     */
    public JugadorMinMax(int profunditat) {
        this.profunditatMaxima = profunditat;
        this.nomDelJugador = "MinMaxPro(" + profunditat + ")";
    }

    @Override
    public String nom() {
        return nomDelJugador;
    }

    /**
     * Decideix el millor moviment donat un tauler i un color.
     * Punt d'entrada de l'algorisme.
     */
    @Override
    public int moviment(Tauler t, int color) {
        this.meuColor = color;
        this.colorRival = -color; // Si jo sóc 1, el rival és -1 (i viceversa)
        this.nodesExplorats = 0;
        this.podesAlphaBeta = 0;
        
        // INICI CRONÒMETRE
        long tempsInici = System.currentTimeMillis();

        int millorColumna = -1;
        int millorValor = INFINIT_NEGATIU;
        int alpha = INFINIT_NEGATIU;
        int beta = INFINIT_POSITIU;

        int midaTauler = t.getMida();

        // MILLORA 1: Generem l'ordre d'exploració (del Centre cap a les Afores)
        // Això fa que la poda Alfa-Beta sigui molt més efectiva i ràpida.
        int[] ordreColumnes = generarOrdreColumnes(midaTauler);

        // Iterem sobre les columnes seguint l'ordre optimitzat
        for (int col : ordreColumnes) {
            if (!t.movpossible(col)) {
                continue;
            }

            // Clonem el tauler per simular el moviment sense afectar l'original
            Tauler taulerSimulat = new Tauler(t);
            taulerSimulat.afegeix(col, meuColor);
            
            // Crida recursiva: ara li toca al rival (esTornMax = false)
            int valorActual = minimax(
                taulerSimulat,
                profunditatMaxima - 1,
                alpha,
                beta,
                false, // Torn del rival (MIN)
                col,   // Última columna jugada
                meuColor // Qui ha fet l'últim moviment
            );

            // Si trobem un moviment millor, l'actualitzem
            if (valorActual > millorValor) {
                millorValor = valorActual;
                millorColumna = col;
            }

            // Actualitzem Alpha (el millor valor que puc assegurar-me jo)
            if (valorActual > alpha) {
                alpha = valorActual;
            }
        }

        // Mecanisme de seguretat: si no s'ha trobat res (molt rar), triar la primera columna vàlida
        if (millorColumna == -1) {
            for (int col = 0; col < midaTauler; col++) {
                if (t.movpossible(col)) {
                    millorColumna = col;
                    break;
                }
            }
        }
        
        // FINAL CRONÒMETRE
        long tempsFinal = System.currentTimeMillis();
        long tempsMoviment = tempsFinal - tempsInici;

        // Mostrem per consola els nodes on s'ha calculat l'heurística (requisit de l'enunciat)
        System.out.println("Nodes explorats (Heurística calculada): " + nodesExplorats + " | Podes: " + podesAlphaBeta + " | Temps Moviment: " + tempsMoviment + " ms");

        return millorColumna;
    }

    /**
     * Algorisme Minimax recursiu amb poda Alfa-Beta.
     * * @param t Tauler actual de la simulació.
     * @param profunditat Nivells restants per explorar.
     * @param alpha Millor valor garantit per a MAX.
     * @param beta Millor valor garantit per a MIN.
     * @param esTornMax True si és el meu torn (intentar maximitzar), False si és rival (minimitzar).
     * @param ultimaColumna Columna on s'ha tirat la fitxa anterior.
     * @param ultimColor Color de la fitxa anterior.
     * @return La puntuació del tauler.
     */
    private int minimax(
        Tauler t,
        int profunditat,
        int alpha,
        int beta,
        boolean esTornMax,
        int ultimaColumna,
        int ultimColor
    ) {
        // 1) CAS BASE: Comprovar si l'últim moviment ha guanyat la partida
        if (ultimaColumna != -1 && t.solucio(ultimaColumna, ultimColor)) {
            if (ultimColor == meuColor) {
                // Victòria meva: com més ràpida (més profunditat restant), millor.
                return INFINIT_POSITIU - (profunditatMaxima - profunditat);
            } else {
                // Victòria del rival: com més tard arribi, menys dolenta (intentar allargar).
                return INFINIT_NEGATIU + (profunditatMaxima - profunditat);
            }
        }

        // 2) CAS BASE: Tauler ple o límit de profunditat assolit
        // Aquí és on calculem l'heurística (nodes fulla realment avaluats)
        if (!t.espotmoure() || profunditat == 0) {
            nodesExplorats++;
            return heuristica(t);
        }

        int mida = t.getMida();
        // MILLORA 1: Usem l'ordre optimitzat també a la recursivitat
        int[] ordreColumnes = generarOrdreColumnes(mida);

        // 3) CAS MAX: És el meu torn, vull maximitzar la puntuació
        if (esTornMax) {
            int millorValor = INFINIT_NEGATIU;

            for (int col : ordreColumnes) {
                if (!t.movpossible(col)) continue;

                Tauler taulerFill = new Tauler(t);
                taulerFill.afegeix(col, meuColor);

                int valor = minimax(
                    taulerFill,
                    profunditat - 1,
                    alpha,
                    beta,
                    false, // Següent torn: Rival (MIN)
                    col,
                    meuColor
                );

                if (valor > millorValor) millorValor = valor;
                if (valor > alpha) alpha = valor;
                
                // PODA BETA: El rival ja té una opció millor per a ell en una altra branca, tallar.
                if (beta <= alpha) {
                    podesAlphaBeta++;
                    break; 
                }
            }
            return millorValor;

        } else {
            // 4) CAS MIN: Torn del rival, vol minimitzar la meva puntuació
            int millorValor = INFINIT_POSITIU;

            for (int col : ordreColumnes) {
                if (!t.movpossible(col)) continue;

                Tauler taulerFill = new Tauler(t);
                taulerFill.afegeix(col, colorRival);

                int valor = minimax(
                    taulerFill,
                    profunditat - 1,
                    alpha,
                    beta,
                    true, // Següent torn: Jo (MAX)
                    col,
                    colorRival
                );

                if (valor < millorValor) millorValor = valor;
                if (valor < beta) beta = valor;
                
                // PODA ALFA: Jo ja tinc una opció millor en una altra branca, tallar.
                if (beta <= alpha) {
                    podesAlphaBeta++;
                    break; 
                }
            }
            return millorValor;
        }
    }

    /**
     * Helper: Genera l'ordre de columnes començant pel centre (Ex: 4, 3, 5, 2...)
     * Això permet trobar bones jugades abans i podar més ràpid.
     */
    private int[] generarOrdreColumnes(int mida) {
        int[] ordre = new int[mida];
        int centre = mida / 2;
        int posicio = 0;
        
        ordre[posicio++] = centre;
        for (int i = 1; i < mida; i++) {
            if (centre - i >= 0) ordre[posicio++] = centre - i;
            if (centre + i < mida) ordre[posicio++] = centre + i;
        }
        return ordre;
    }

    /**
     * Funció Heurística: Avalua com de bo és el tauler per a mi.
     * Combina:
     * 1. Punts per control del centre (estratègic).
     * 2. Punts per línies i amenaces (tàctic).
     */
    private int heuristica(Tauler t) {
        int puntuacio = 0;
        
        // MILLORA 2: Bonus Posicional (Control del Centre)
        // Ajuda a desempatar quan no hi ha línies clares.
        int mida = t.getMida();
        int colCentral = mida / 2;
        
        for(int f = 0; f < mida; f++) {
            int fitxa = t.getColor(f, colCentral);
            if (fitxa == meuColor) puntuacio += 40;      // Bonus per tenir centre
            else if (fitxa == colorRival) puntuacio -= 40; // Penalització si el té el rival
        }

        // Avaluació clàssica de línies en totes direccions
        puntuacio += avaluaDireccio(t, 1, 0);  // Horitzontal (-)
        puntuacio += avaluaDireccio(t, 0, 1);  // Vertical (|)
        puntuacio += avaluaDireccio(t, 1, 1);  // Diagonal (\)
        puntuacio += avaluaDireccio(t, 1, -1); // Diagonal (/)
        
        return puntuacio;
    }

    /**
     * Recorre totes les seqüències possibles de 4 caselles en una direcció.
     */
    private int avaluaDireccio(Tauler t, int deltaFila, int deltaCol) {
        int mida = t.getMida();
        int totalPunts = 0;

        for (int f = 0; f < mida; f++) {
            for (int c = 0; c < mida; c++) {
                // Calculem la posició final de la línia de 4
                int fFinal = f + 3 * deltaFila;
                int cFinal = c + 3 * deltaCol;

                // Si la línia surt fora del tauler, la ignorem
                if (fFinal < 0 || fFinal >= mida || cFinal < 0 || cFinal >= mida) {
                    continue;
                }

                int fitxesMeves = 0;
                int fitxesRival = 0;

                // Mirem les 4 caselles de la línia
                for (int k = 0; k < 4; k++) {
                    int ff = f + k * deltaFila;
                    int cc = c + k * deltaCol;
                    int colorCasella = t.getColor(ff, cc);

                    if (colorCasella == meuColor) {
                        fitxesMeves++;
                    } else if (colorCasella == colorRival) {
                        fitxesRival++;
                    }
                }

                // Només puntuem línies "pures" (sense fitxes del rival barrejades)
                if (fitxesMeves > 0 && fitxesRival == 0) {
                    totalPunts += valorSegonsQuantitat(fitxesMeves);
                } else if (fitxesRival > 0 && fitxesMeves == 0) {
                    totalPunts -= valorSegonsQuantitat(fitxesRival);
                }
            }
        }
        return totalPunts;
    }

    /**
     * Retorna punts segons quantes fitxes tenim connectades.
     */
    private int valorSegonsQuantitat(int n) {
        switch (n) {
            case 1: return 1;       // Una fitxa sola
            case 2: return 10;      // Dues fitxes
            case 3: return 50;      // Tres fitxes (amenaça)
            case 4: return 100000;  // Quatre fitxes (victòria)
            default: return 0;
        }
    }
}