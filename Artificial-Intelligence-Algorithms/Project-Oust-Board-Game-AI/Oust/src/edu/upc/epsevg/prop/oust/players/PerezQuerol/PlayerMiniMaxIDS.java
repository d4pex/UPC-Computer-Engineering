package edu.upc.epsevg.prop.oust.players.PerezQuerol;

import edu.upc.epsevg.prop.oust.GameStatus;
import edu.upc.epsevg.prop.oust.PlayerMove;
import edu.upc.epsevg.prop.oust.SearchType;
import edu.upc.epsevg.prop.oust.PlayerType;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

/**
 * Implementación de un jugador automático utilizando Iterative Deepening Search (IDS).
 * <p>
 * Esta clase extiende {@link PlayerMiniMax} para añadir gestión dinámica del tiempo.
 * En lugar de buscar a una profundidad fija, incrementa la profundidad progresivamente (1, 2, 3...)
 * hasta que se recibe una señal de timeout.
 * <p>
 * <b>Estrategia de Seguridad ("Caja Fuerte"):</b>
 * Para evitar exceder el tiempo límite y sufrir penalizaciones ("Illegal Wait"), este jugador:
 * <ol>
 * <li>Calcula un camino completo y válido al inicio (Profundidad 0).</li>
 * <li>En cada nivel de profundidad completado, genera y guarda el turno completo (incluyendo combos) en {@code bestPathGlobal}.</li>
 * <li>Si ocurre un timeout durante el cálculo de una profundidad, descarta el trabajo incompleto y devuelve inmediatamente el último {@code bestPathGlobal} seguro.</li>
 * </ol>
 * * @author PerezQuerol
 */
public class PlayerMiniMaxIDS extends PlayerMiniMax {

    /**
     * Constructor del jugador IDS.
     * Inicializa la profundidad base a 0 (ya que será dinámica) y establece el nombre del jugador.
     */
    public PlayerMiniMaxIDS() {
        super(0); 
        this.nomJugador = "PEREZQUEROL_IDS";
    }

    /**
     * Ejecuta el movimiento calculando la mejor jugada posible dentro del tiempo asignado.
     * <p>
     * El método realiza un bucle de profundización iterativa:
     * 1. Genera una solución inicial rápida por seguridad.
     * 2. Llama a Minimax con profundidad creciente.
     * 3. Si una profundidad se completa sin timeout, expande el movimiento inicial a un turno completo (combos) usando la heurística táctica y lo guarda como el mejor hasta ahora.
     * 4. Si se detecta un timeout, se interrumpe el bucle y se devuelve la solución guardada de la profundidad anterior.
     * * @param estatJoc Estado actual del juego.
     * @return El mejor movimiento (camino completo) encontrado antes del timeout.
     */
    @Override
    public PlayerMove move(GameStatus estatJoc) {
        nodesExplorats = 0;
        maximaProfunditat = 0;
        aturarCerca = false;

        GameStatusTunned estatActual = new GameStatusTunned(estatJoc);
        PlayerType elMeuColor = estatActual.getCurrentPlayer();

        List<Point> movimentsLegals = estatActual.getMoves();
        if (movimentsLegals.isEmpty()) {
            return new PlayerMove(null, 0, 0, SearchType.MINIMAX_IDS);
        }

        // --- INICIALIZACIÓN DE SEGURIDAD ---
        // Generamos un camino completo válido inicial (Profundidad 0)
        // por si el timeout salta antes de acabar la profundidad 1.
        Point primerMov = movimentsLegals.get(0);
        List<Point> bestPathGlobal = generarCaminoCompleto(estatActual, primerMov, elMeuColor);
        
        int profunditatActual = 1;

        // --- BUCLE IDS ---
        while (!aturarCerca) {
            
            // 1. Minimax nos da la mejor PIEDRA INICIAL para esta profundidad
            Point mejorInicioRonda = minimax(estatActual, profunditatActual, elMeuColor);
            
            // 2. CHECK DE TIMEOUT
            // Si minimax devolvió null o la bandera saltó, significa que el cálculo
            // de esta profundidad está incompleto o corrupto. Lo descartamos.
            if (aturarCerca || mejorInicioRonda == null) {
                break; // Salimos y usamos bestPathGlobal de la vuelta anterior
            }
            
            // 3. GENERAR PATH COMPLETO (INTELIGENTE)
            // Como todavía tenemos tiempo, calculamos el resto del turno (combos) usando
            // la heurística táctica (movimientoRapido) y lo guardamos.
            // Esto asegura que lo que guardamos en la "Caja Fuerte" es una jugada terminada.
            List<Point> pathCompletoRonda = generarCaminoCompleto(estatActual, mejorInicioRonda, elMeuColor);
            
            // 4. ACTUALIZAR CAJA FUERTE
            // Guardamos el resultado completo de esta profundidad como el mejor conocido.
            bestPathGlobal = pathCompletoRonda;
            maximaProfunditat = profunditatActual;
            
            profunditatActual++;
            if (profunditatActual > 100) break; // Límite de seguridad
        }            

        return new PlayerMove(bestPathGlobal, nodesExplorats, maximaProfunditat, SearchType.MINIMAX_IDS);
    }
}
