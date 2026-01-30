package edu.upc.epsevg.prop.oust.players.PerezQuerol;

import edu.upc.epsevg.prop.oust.GameStatus;
import edu.upc.epsevg.prop.oust.PlayerType;
import java.awt.Point;
import java.util.List;

/**
 * Versión optimizada de la clase GameStatus para el juego Oust.
 * <p>
 * Esta clase mantiene una representación paralela del tablero utilizando una matriz de bytes ({@code byte[][]})
 * en lugar de objetos {@code PlayerType} o mapas complejos. Esto permite realizar operaciones de
 * lectura, clonación y simulación de movimientos órdenes de magnitud más rápido que la clase base.
 * <p>
 * Es fundamental para que el algoritmo Minimax pueda explorar profundidades elevadas sin exceder el tiempo límite.
 * * @author PerezQuerol
 */
public class GameStatusTunned extends GameStatus {

    /**
     * Representación optimizada del tablero.
     * 0 = Vacío, 1 = Player 1, 2 = Player 2.
     * El acceso a matriz primitiva es mucho más rápido que el acceso a HashMaps u objetos.
     */
    public byte[][] fastBoard; 
    
    /** Tamaño del lado del tablero hexagonal. */
    private int n;
    
    /** Dimensión total de la matriz cuadrada necesaria para contener el hexágono (2*n - 1). */
    private int dim;

    /** Desplazamientos de fila para los 6 vecinos hexagonales. */
    private static final int[] DR = {0, 0, 1, 1, -1, -1};
    
    /** Desplazamientos de columna para los 6 vecinos hexagonales. */
    private static final int[] DC = {1, -1, 0, 1, 0, -1};

    /**
     * Constructor que inicializa el estado optimizado a partir de un GameStatus normal.
     * @param gs Estado del juego original proporcionado por el sistema.
     */
    public GameStatusTunned(GameStatus gs) {
        super(gs);
        initFastBoard(gs.getSize());
        copiarTableroDeSuper();
    }

    /**
     * Constructor de copia (Deep Copy).
     * Esencial para las simulaciones de Minimax, permite clonar el estado rápidamente.
     * @param copy Instancia de GameStatusTunned a clonar.
     */
    public GameStatusTunned(GameStatusTunned copy) {
        super(copy);
        this.n = copy.n;
        this.dim = copy.dim;
        this.fastBoard = new byte[dim][dim];
        for (int i = 0; i < dim; i++) {
            System.arraycopy(copy.fastBoard[i], 0, this.fastBoard[i], 0, dim);
        }
    }

    /**
     * Inicializa la estructura de datos del tablero rápido.
     * @param size Tamaño del tablero (radio).
     */
    private void initFastBoard(int size) {
        this.n = size;
        this.dim = 2 * n - 1;
        this.fastBoard = new byte[dim][dim];
    }

    /**
     * Sincroniza el tablero rápido (fastBoard) con el estado actual de la clase padre (GameStatus).
     * Traduce los PlayerType a bytes (1 y 2).
     */
    public void copiarTableroDeSuper() {
        for (int i = 0; i < dim; i++) {
            int cInici = Math.max(0, i - n + 1);
            int cFinal = Math.min(dim, n + i);
            for (int j = cInici; j < cFinal; j++) {
                PlayerType p = super.getColor(i, j);
                if (p == PlayerType.PLAYER1) fastBoard[i][j] = 1;
                else if (p == PlayerType.PLAYER2) fastBoard[i][j] = 2;
                else fastBoard[i][j] = 0;
            }
        }
    }

    /**
     * Coloca una ficha en el tablero y actualiza inmediatamente la representación optimizada.
     * @param point Coordenadas donde colocar la ficha.
     */
    @Override
    public void placeStone(Point point) {
        super.placeStone(point);
        // Actualizamos la copia rápida tras el movimiento del motor de juego
        copiarTableroDeSuper();
    }
    
    // --- EVALUACIÓN PRINCIPAL ---

    /**
     * Función de evaluación heurística altamente optimizada.
     * Calcula una puntuación para el estado del tablero basada en múltiples factores ponderados.
     * * @param elMeuColor Color del jugador que está evaluando (Max).
     * @param profundidadRestante Profundidad restante en el árbol (para premiar victorias rápidas).
     * @return Puntuación numérica del estado (positivo favorece a elMeuColor).
     */
    public double evaluarRapido(PlayerType elMeuColor, int profundidadRestante) {
        // 1. Detección de Final de Partida
        if (this.isGameOver()) {
            if (this.GetWinner() == elMeuColor) return 1000000000.0 + (profundidadRestante * 10000.0);
            else if (this.GetWinner() == null) return 0; // Empate
            else return -1000000000.0 - (profundidadRestante * 10000.0);
        }

        byte miByte = (byte) (elMeuColor == PlayerType.PLAYER1 ? 1 : 2);
        byte suByte = (byte) (elMeuColor == PlayerType.PLAYER1 ? 2 : 1);

        // 2. Generación de mapas de calor (distancias) usando BFS rápido
        int[][] miMapa = generarMapaDistanciasFast(miByte);
        int[][] suMapa = generarMapaDistanciasFast(suByte);

        double puntuacion = 0;
        int misFichas = 0;
        int susFichas = 0;
        int miTerritorio = 0;
        int suTerritorio = 0;

        // 3. Análisis posicional y conteo
        for (int i = 0; i < dim; i++) {
            int cInici = Math.max(0, i - n + 1);
            int cFinal = Math.min(dim, n + i);
            for (int j = cInici; j < cFinal; j++) {
                int miDist = miMapa[i][j];
                int suDist = suMapa[i][j];

                if (miDist != -1) {
                    miTerritorio++;
                    if (miDist == 0) { // Es una ficha mía
                        misFichas++;
                        // Penalización por estar muy cerca del enemigo (vulnerable)
                        if (suDist >= 1 && suDist <= 2) puntuacion -= 50000;
                        // Bonificación por control central/estratégico
                        if (i > 1 && i < dim - 2 && (j == 2 || j == dim - 3)) puntuacion += 5000;
                    }
                }
                if (suDist != -1) {
                    suTerritorio++;
                    if (suDist == 0) susFichas++;
                }
            }
        }

        // 4. Análisis de Grupos (Suma de cuadrados de tamaños)
        double miScoreGrupos = calcularScoreGruposFast(miByte);
        double suScoreGrupos = calcularScoreGruposFast(suByte);

        // 5. Agregación de pesos ponderados
        puntuacion += (misFichas - susFichas) * 8000000; 
        puntuacion += (miScoreGrupos - suScoreGrupos) * 20000000;
        puntuacion += (miTerritorio - suTerritorio) * 20000;

        return puntuacion;
    }

    // --- NUEVAS FUNCIONES PARA SUICIDIO INTELIGENTE ---

    /**
     * Calcula recursivamente cuántas fichas puede capturar el rival encadenando turnos (combos).
     * @param miColor El color del jugador que está en peligro de ser comido.
     * @return Número máximo de fichas que el rival puede eliminar en una secuencia.
     */
    public int calcularComboRivalFast(PlayerType miColor) {
        PlayerType rival = (miColor == PlayerType.PLAYER1) ? PlayerType.PLAYER2 : PlayerType.PLAYER1;
        
        // Si no es el turno del rival, no puede iniciar el combo ahora
        if (this.getCurrentPlayer() != rival) return 0; 

        byte miByte = (byte) (miColor == PlayerType.PLAYER1 ? 1 : 2);
        int maxComidas = 0;
        
        List<Point> moves = this.getMoves();
        for (Point m : moves) {
            GameStatusTunned copia = new GameStatusTunned(this);
            int antes = copia.contarFichasFast(miByte);
            copia.placeStone(m); 
            int despues = copia.contarFichasFast(miByte);
            
            int comidas = antes - despues;
            if (comidas > 0) {
                // Recursión: si comió y mantiene turno, ¿cuántas más puede comer?
                maxComidas = Math.max(maxComidas, comidas + copia.calcularComboRivalFast(miColor));
            }
        }
        return maxComidas;
    }

    /**
     * Calcula el tamaño del grupo conectado al que pertenece una ficha específica mediante BFS.
     * @param r Fila de la ficha.
     * @param c Columna de la ficha.
     * @param color Color de la ficha (byte).
     * @return Cantidad de fichas conectadas en el mismo grupo.
     */
    public int calcularGrupoFast(int r, int c, byte color) {
        if (fastBoard[r][c] != color) return 0;
        
        boolean[][] visited = new boolean[dim][dim];
        // Colas para BFS manual (más rápido que objetos Queue)
        int[] qR = new int[dim * dim];
        int[] qC = new int[dim * dim];
        int head = 0, tail = 0;
        
        qR[tail] = r; qC[tail] = c; tail++;
        visited[r][c] = true;
        int count = 0;
        
        while(head < tail) {
            int currR = qR[head]; int currC = qC[head]; head++;
            count++;
            
            for (int k = 0; k < 6; k++) {
                int nr = currR + DR[k]; int nc = currC + DC[k];
                 if (nr >= 0 && nr < dim) {
                    int jStart = Math.max((nr - n) + 1, 0);
                    int jEnd = Math.min(n + nr, dim);
                    if (nc >= jStart && nc < jEnd) {
                        if (!visited[nr][nc] && fastBoard[nr][nc] == color) {
                            visited[nr][nc] = true;
                            qR[tail] = nr; qC[tail] = nc; tail++;
                        }
                    }
                 }
            }
        }
        return count;
    }

    // --- MÉTODOS HELPER RÁPIDOS ---

    /**
     * Comprueba si realizar un movimiento resulta en una pérdida neta inmediata de fichas.
     * Utilizado para filtrar movimientos evidentemente malos en la fase de selección.
     * @param mov El movimiento a evaluar.
     * @param miColor El color del jugador que mueve.
     * @return true si el movimiento causa una reducción de fichas propias en respuesta inmediata.
     */
    public boolean esSuicidioRapido(Point mov, PlayerType miColor) {
        GameStatusTunned copia = new GameStatusTunned(this);
        copia.placeStone(mov); 
        PlayerType rival = (miColor == PlayerType.PLAYER1) ? PlayerType.PLAYER2 : PlayerType.PLAYER1;
        
        // Si al mover no le damos el turno al rival, no es suicidio inmediato
        if (copia.getCurrentPlayer() != rival) return false;

        byte miByte = (byte) (miColor == PlayerType.PLAYER1 ? 1 : 2);
        int misFichasAntes = copia.contarFichasFast(miByte);
        
        // Verificamos si alguna respuesta inmediata del rival reduce nuestras fichas
        List<Point> movesRival = copia.getMoves();
        for (Point m : movesRival) {
            GameStatusTunned simRival = new GameStatusTunned(copia);
            simRival.placeStone(m);
            int misFichasDespues = simRival.contarFichasFast(miByte);
            if (misFichasDespues < misFichasAntes) return true; 
        }
        return false;
    }

    /**
     * Cuenta rápidamente el número total de fichas de un color en el tablero.
     * @param colorByte Color a contar (1 o 2).
     * @return Cantidad total de fichas.
     */
    public int contarFichasFast(byte colorByte) {
        int count = 0;
        for (int i = 0; i < dim; i++) {
            int cIni = Math.max(0, i - n + 1);
            int cFin = Math.min(dim, n + i);
            for (int j = cIni; j < cFin; j++) {
                if (fastBoard[i][j] == colorByte) count++;
            }
        }
        return count;
    }

    /**
     * Calcula la distancia mínima (en casillas) desde un punto hasta la ficha enemiga más cercana.
     * Utiliza BFS (Breadth-First Search) con límite de profundidad 4 para eficiencia.
     * @param origen Punto de inicio.
     * @param miColor Color del jugador (se busca la distancia al color opuesto).
     * @return Distancia en casillas. Retorna 5 si no hay enemigos cerca (límite).
     */
    public int distanciaMinimaRivalFast(Point origen, PlayerType miColor) {
        byte rivalByte = (byte) (miColor == PlayerType.PLAYER1 ? 2 : 1);
        int capacity = dim * dim;
        int[] qR = new int[capacity];
        int[] qC = new int[capacity];
        int[] qD = new int[capacity];
        boolean[][] visited = new boolean[dim][dim];
        int head = 0, tail = 0;

        qR[tail] = origen.x; qC[tail] = origen.y; qD[tail] = 0; tail++;
        visited[origen.x][origen.y] = true;

        while(head < tail) {
            int r = qR[head]; int c = qC[head]; int d = qD[head]; head++;
            
            // Si encontramos rival, devolvemos distancia
            if (d > 0 && fastBoard[r][c] == rivalByte) return d;
            
            // Poda: si está muy lejos, dejamos de buscar para ahorrar tiempo
            if (d >= 4) continue;

            for (int k=0; k<6; k++) {
                int nr = r + DR[k]; int nc = c + DC[k];
                if (nr>=0 && nr<dim) {
                    int jStart = Math.max((nr-n)+1, 0);
                    int jEnd = Math.min(n+nr, dim);
                    if (nc >= jStart && nc < jEnd && !visited[nr][nc]) {
                        visited[nr][nc] = true;
                        qR[tail] = nr; qC[tail] = nc; qD[tail] = d + 1; tail++;
                    }
                }
            }
        }
        return 5; // Valor por defecto "lejos"
    }

    /**
     * Genera un mapa completo de distancias para todas las casillas hacia el color objetivo.
     * Utilizado por la función de evaluación para calcular el control del territorio.
     * @param targetColor Color hacia el cual se calculan las distancias.
     * @return Matriz de enteros con las distancias.
     */
    private int[][] generarMapaDistanciasFast(byte targetColor) {
        int[][] dist = new int[dim][dim];
        for(int i=0; i<dim; i++) for(int j=0; j<dim; j++) dist[i][j] = -1;

        int capacity = dim * dim * 2;
        int[] qR = new int[capacity];
        int[] qC = new int[capacity];
        int head = 0; int tail = 0;

        // Inicializamos BFS multisource con todas las fichas del color objetivo
        for (int i = 0; i < dim; i++) {
            int cInici = Math.max(0, i - n + 1);
            int cFinal = Math.min(dim, n + i);
            for (int j = cInici; j < cFinal; j++) {
                if (fastBoard[i][j] == targetColor) {
                    dist[i][j] = 0;
                    qR[tail] = i; qC[tail] = j; tail++;
                }
            }
        }

        while (head < tail) {
            int r = qR[head]; int c = qC[head]; head++;
            int dActual = dist[r][c];
            if (dActual >= 2) continue; // Solo nos interesa el territorio inmediato

            for (int k = 0; k < 6; k++) {
                int nr = r + DR[k]; int nc = c + DC[k];
                if (nr >= 0 && nr < dim) {
                     int jStart = Math.max((nr - n) + 1, 0);
                     int jEnd = Math.min(n + nr, dim);
                     if (nc >= jStart && nc < jEnd) {
                         if (dist[nr][nc] == -1) {
                             dist[nr][nc] = dActual + 1;
                             qR[tail] = nr; qC[tail] = nc; tail++;
                         }
                     }
                }
            }
        }
        return dist;
    }

    /**
     * Calcula la suma de los cuadrados de los tamaños de todos los grupos de un color.
     * Fomenta la creación de grupos grandes y consolidados.
     * @param color Color a evaluar.
     * @return Valor numérico (score) de los grupos.
     */
    private double calcularScoreGruposFast(byte color) {
        boolean[][] visitado = new boolean[dim][dim];
        double scoreTotal = 0;
        int[] qR = new int[dim*dim];
        int[] qC = new int[dim*dim];

        for (int i = 0; i < dim; i++) {
            int cInici = Math.max(0, i - n + 1);
            int cFinal = Math.min(dim, n + i);
            for (int j = cInici; j < cFinal; j++) {
                if (!visitado[i][j] && fastBoard[i][j] == color) {
                    int head = 0, tail = 0;
                    qR[tail] = i; qC[tail] = j; tail++;
                    visitado[i][j] = true;
                    int tamano = 0;
                    
                    while(head < tail) {
                        int r = qR[head]; int c = qC[head]; head++;
                        tamano++;
                        for (int k = 0; k < 6; k++) {
                            int nr = r + DR[k]; int nc = c + DC[k];
                             if (nr >= 0 && nr < dim) {
                                int jStart = Math.max((nr - n) + 1, 0);
                                int jEnd = Math.min(n + nr, dim);
                                if (nc >= jStart && nc < jEnd) {
                                    if (!visitado[nr][nc] && fastBoard[nr][nc] == color) {
                                        visitado[nr][nc] = true;
                                        qR[tail] = nr; qC[tail] = nc; tail++;
                                    }
                                }
                             }
                        }
                    }
                    scoreTotal += (tamano * tamano);
                }
            }
        }
        return scoreTotal;
    }
}