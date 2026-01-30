package edu.upc.epsevg.prop.oust.players.PerezQuerol;

import edu.upc.epsevg.prop.oust.GameStatus;
import edu.upc.epsevg.prop.oust.IAuto;
import edu.upc.epsevg.prop.oust.IPlayer;
import edu.upc.epsevg.prop.oust.PlayerMove;
import edu.upc.epsevg.prop.oust.PlayerType;
import edu.upc.epsevg.prop.oust.SearchType;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

/**
 * Implementación de un jugador automático para el juego Oust utilizando el algoritmo Minimax.
 * <p>
 * Esta clase define el comportamiento de un bot que decide sus movimientos explorando
 * el árbol de juego hasta una profundidad determinada. Incluye optimizaciones como la poda
 * Alpha-Beta y heurísticas específicas para la selección de movimientos suicidas y rápidos.
 * </p>
 * @author PerezQuerol
 */
public class PlayerMiniMax implements IPlayer, IAuto {

    /** Nombre del jugador que se mostrará en la interfaz. */
    protected String nomJugador;
    
    /** Profundidad fija de búsqueda para el algoritmo Minimax (si no se usa IDS). */
    protected int profunditatFixa;
    
    /** Bandera de control para detener la búsqueda inmediatamente en caso de timeout. */
    protected volatile boolean aturarCerca = false;
    
    /** Contador de nodos explorados durante la búsqueda para estadísticas. */
    protected long nodesExplorats = 0;
    
    /** Profundidad máxima alcanzada realmente durante la ejecución. */
    protected int maximaProfunditat = 0;

    /**
     * Constructor de la clase PlayerMiniMax.
     * @param profunditat La profundidad máxima inicial a la que buscará el algoritmo.
     */
    public PlayerMiniMax(int profunditat) { 
        this.nomJugador = "PEREZQUEROL";
        this.profunditatFixa = profunditat;
    }

    /**
     * Devuelve el nombre del jugador.
     * @return Cadena con el nombre del jugador.
     */
    @Override
    public String getName() { return nomJugador; }

    /**
     * Método invocado por el controlador del juego cuando se agota el tiempo de turno.
     * Activa la bandera {@code aturarCerca} para finalizar la búsqueda actual de forma segura.
     * Solo tiene efecto si la clase es una instancia de tipo IDS.
     */
    @Override
    public void timeout() {
        if (this.getClass().getSimpleName().contains("IDS")) {
            this.aturarCerca = true; 
        }
    }

    /**
     * Decide y ejecuta el mejor movimiento posible para el estado actual del juego.
     * <p>
     * Inicializa la búsqueda Minimax, gestiona el tiempo y construye el camino completo
     * de movimientos (incluyendo capturas en cadena) una vez decidido el movimiento inicial.
     * </p>
     * @param estatJoc El estado actual del tablero de juego.
     * @return Un objeto {@code PlayerMove} que contiene la lista de movimientos a realizar,
     * el número de nodos explorados, la profundidad alcanzada y el tipo de búsqueda.
     */
    @Override
    public PlayerMove move(GameStatus estatJoc) {
        // Versión fija (No IDS)
        nodesExplorats = 0;
        maximaProfunditat = 0;
        aturarCerca = false;

        GameStatusTunned estatActual = new GameStatusTunned(estatJoc);
        PlayerType elMeuColor = estatActual.getCurrentPlayer();

        // 1. Minimax normal
        Point mejorInicio = minimax(estatActual, profunditatFixa, elMeuColor);
        
        // 2. Generar el path completo
        List<Point> pathFinal;
        if (mejorInicio != null) {
            pathFinal = generarCaminoCompleto(estatActual, mejorInicio, elMeuColor);
        } else {
             // Fallback de emergencia
            List<Point> moves = estatActual.getMoves();
            pathFinal = new ArrayList<>();
            if(!moves.isEmpty()) pathFinal.add(moves.get(0));
        }

        return new PlayerMove(pathFinal, nodesExplorats, maximaProfunditat, SearchType.MINIMAX);
    }

    // --- HERRAMIENTA CLAVE PARA EL IDS ---
    
    /**
     * Genera la secuencia completa de movimientos para un turno, dado un movimiento inicial.
     * <p>
     * Esta función es crucial para el IDS ("Caja Fuerte"). Toma el mejor movimiento inicial
     * encontrado por Minimax y simula el resto del turno (capturas sucesivas) utilizando
     * una heurística rápida (Greedy) para completar la jugada antes de que se agote el tiempo.
     * </p>
     * @param estadoOriginal Estado del tablero antes de realizar el movimiento.
     * @param inicio El punto inicial donde se coloca la primera ficha.
     * @param miColor El color del jugador actual.
     * @return Lista de puntos que representan la secuencia completa de movimientos del turno.
     */
    public List<Point> generarCaminoCompleto(GameStatusTunned estadoOriginal, Point inicio, PlayerType miColor) {
        List<Point> camino = new ArrayList<>();
        camino.add(inicio);
        
        // Simulamos
        GameStatusTunned sim = new GameStatusTunned(estadoOriginal);
        sim.placeStone(inicio);
        
        // Rellenamos el resto con tu lógica inteligente
        while (!sim.isGameOver() && sim.getCurrentPlayer() == miColor) {
            if (sim.getMoves().isEmpty()) break;
            
            // Usamos tu movimientoRapido (inteligencia táctica)
            Point p = movimientoRapido(sim, miColor);
            
            if (p == null) break;
            sim.placeStone(p);
            camino.add(p);
            
            if (camino.size() > 100) break; // Seguridad
        }
        return camino;
    }

    // --- MINIMAX RECURSIVO (TU LÓGICA ORIGINAL) ---

    /**
     * Ejecuta la fase raíz del algoritmo Minimax para seleccionar el mejor movimiento inicial.
     * <p>
     * Filtra los movimientos suicidas y evalúa los candidatos restantes utilizando
     * la función recursiva {@code alphaBeta}.
     * </p>
     * @param estat Estado actual del tablero.
     * @param profunditat Profundidad máxima de búsqueda.
     * @param elMeuColor Color del jugador que está maximizando.
     * @return El punto (coordenada) que representa el mejor movimiento inicial encontrado.
     */
    protected Point minimax(GameStatusTunned estat, int profunditat, PlayerType elMeuColor) {
        if (aturarCerca) return null;

        List<Point> movimientos = estat.getMoves();
        Point mejorMov = null;
        double mejorValor = Double.NEGATIVE_INFINITY;
        
        // 1. Filtro de Seguridad
        List<Point> noSuicidas = new ArrayList<>();
        for (Point m : movimientos) {
            if (aturarCerca) return null;
            if (!estat.esSuicidioRapido(m, elMeuColor)) {
                noSuicidas.add(m);
            }
        }

        if (noSuicidas.isEmpty()) {
            if (maximaProfunditat == 0) maximaProfunditat = 1; 
            return elegirMejorSuicidio(estat, movimientos, elMeuColor);
        }
        
        double alpha = Double.NEGATIVE_INFINITY;
        double beta = Double.POSITIVE_INFINITY;

        for (Point mov : noSuicidas) {
            if (aturarCerca) return null;
            
            GameStatusTunned hijo = new GameStatusTunned(estat);
            hijo.placeStone(mov);
            
            boolean mantengoTurno = (hijo.getCurrentPlayer() == elMeuColor);
            double valor = alphaBeta(hijo, profunditat - 1, alpha, beta, mantengoTurno, elMeuColor);
            
            if (valor > mejorValor) {
                mejorValor = valor;
                mejorMov = mov;
            }
            alpha = Math.max(alpha, mejorValor);
        }
        
        if (aturarCerca) return null;
        return mejorMov != null ? mejorMov : movimientos.get(0);
    }

    /**
     * Algoritmo Alpha-Beta recursivo para evaluar estados del juego.
     * <p>
     * Explora el árbol de juego podando ramas no prometedoras (Alpha-Beta Pruning).
     * </p>
     * @param estat Estado del tablero a evaluar.
     * @param profunditat Profundidad restante para la búsqueda.
     * @param alpha El mejor valor encontrado hasta ahora para el maximizador.
     * @param beta El mejor valor encontrado hasta ahora para el minimizador.
     * @param esMax Indica si el nodo actual es un nodo maximizador (turno del jugador).
     * @param elMeuColor Color del jugador principal (Max).
     * @return El valor heurístico calculado para el estado.
     */
    protected double alphaBeta(GameStatusTunned estat, int profunditat, double alpha, double beta, boolean esMax, PlayerType elMeuColor) {
        nodesExplorats++;
        if (aturarCerca) return alpha; 
        
        int profActual = profunditatFixa - profunditat;
        if (profActual > maximaProfunditat) maximaProfunditat = profActual;

        if (estat.isGameOver() || profunditat <= 0) {
            return estat.evaluarRapido(elMeuColor, profunditat);
        }

        List<Point> movimientos = estat.getMoves();
        if (movimientos.isEmpty()) {
             return estat.evaluarRapido(elMeuColor, profunditat);
        }

        double mejorValor = esMax ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;

        for (Point mov : movimientos) {
            if (aturarCerca) return alpha; 

            GameStatusTunned hijo = new GameStatusTunned(estat);
            hijo.placeStone(mov);
            boolean siguienteEsMax = (hijo.getCurrentPlayer() == elMeuColor);
            
            double valor = alphaBeta(hijo, profunditat - 1, alpha, beta, siguienteEsMax, elMeuColor);

            if (esMax) {
                mejorValor = Math.max(mejorValor, valor);
                alpha = Math.max(alpha, mejorValor);
            } else {
                mejorValor = Math.min(mejorValor, valor);
                beta = Math.min(beta, mejorValor);
            }
            if (beta <= alpha) break;
        }
        return mejorValor;
    }

    // --- TU LÓGICA DE SUICIDIO ---
    
    /**
     * Selecciona el "mejor" movimiento suicida cuando no hay opciones seguras.
     * <p>
     * Evalúa las opciones suicidas basándose en el daño potencial (combo y grupos perdidos)
     * y la distancia al rival, intentando minimizar las pérdidas.
     * Incluye protección contra Timeouts dentro del bucle de evaluación.
     * </p>
     * @param s Estado actual del juego.
     * @param suicidios Lista de movimientos identificados como suicidas.
     * @param miColor Color del jugador.
     * @return El punto correspondiente al movimiento suicida menos perjudicial.
     */
    private Point elegirMejorSuicidio(GameStatusTunned s, List<Point> suicidios, PlayerType miColor) {
        Point mejor = suicidios.get(0);
        int mejorScore = Integer.MIN_VALUE;

        // Si hay timeout, devolvemos rápido para no gastar tiempo
        if (aturarCerca) return mejor;

        for (Point mov : suicidios) {
            if (aturarCerca) break;
            nodesExplorats++; 
            GameStatusTunned despuesDeMi = new GameStatusTunned(s);
            despuesDeMi.placeStone(mov);

            int combo = despuesDeMi.calcularComboRivalFast(miColor);
            byte miByte = (byte) (miColor == PlayerType.PLAYER1 ? 1 : 2);
            int grupo = despuesDeMi.calcularGrupoFast(mov.x, mov.y, miByte);
            int dist = despuesDeMi.distanciaMinimaRivalFast(mov, miColor);

            int score = -(combo * 1000 + grupo * 100) + dist;

            if (score > mejorScore) {
                mejorScore = score;
                mejor = mov;
            }
        }
        return mejor;
    }

    // --- TU LÓGICA DE MOVIMIENTO RÁPIDO ---
    
    /**
     * Determina rápidamente el siguiente movimiento a realizar durante la expansión de un turno (combos).
     * <p>
     * Utiliza una heurística Greedy (voraz) priorizando:
     * 1. Evitar suicidios.
     * 2. Realizar capturas que permitan mantener el turno.
     * 3. Maximizar la distancia al rival si no hay capturas disponibles.
     * </p>
     * @param s Estado actual del juego.
     * @param miColor Color del jugador.
     * @return El punto seleccionado para el siguiente movimiento.
     */
    protected Point movimientoRapido(GameStatusTunned s, PlayerType miColor) {
        List<Point> moves = s.getMoves();
        if (moves.isEmpty()) return null;

        // Si hay timeout, devolvemos el primero para no causar Illegal Wait
        if (aturarCerca) return moves.get(0);

        List<Point> movesSeguros = new ArrayList<>();
        for (Point m : moves) {
            nodesExplorats++;
            if (!s.esSuicidioRapido(m, miColor)) {
                movesSeguros.add(m);
            }
        }
        
        if (movesSeguros.isEmpty()) {
            return elegirMejorSuicidio(s, moves, miColor);
        }

        // Prioridad: Captura segura
        for (Point m : movesSeguros) {
            GameStatusTunned sim = new GameStatusTunned(s);
            byte miByte = (byte)(miColor == PlayerType.PLAYER1 ? 1 : 2);
            int antes = sim.contarFichasFast(miByte);
            sim.placeStone(m);
            if (sim.getCurrentPlayer() == miColor) {
                 int despues = sim.contarFichasFast(miByte);
                 if (despues > antes) return m;
            }
        }

        // Prioridad: Distancia
        Point mejorLejos = movesSeguros.get(0);
        int maxDist = -1;
        for (Point m : movesSeguros) {
            int dist = s.distanciaMinimaRivalFast(m, miColor);
            if (dist > maxDist) {
                maxDist = dist;
                mejorLejos = m;
            }
        }
        return mejorLejos;
    }
}