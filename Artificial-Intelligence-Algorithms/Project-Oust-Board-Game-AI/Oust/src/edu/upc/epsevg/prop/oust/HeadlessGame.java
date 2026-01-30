package edu.upc.epsevg.prop.oust;

import edu.upc.epsevg.prop.oust.players.HumanPlayer;
import edu.upc.epsevg.prop.oust.players.RandomPlayer;
import edu.upc.epsevg.prop.oust.players.PerezQuerol.PlayerMiniMax;
import edu.upc.epsevg.prop.oust.players.PerezQuerol.PlayerMiniMaxIDS;

import java.awt.Point;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.lang.reflect.Field;

public class HeadlessGame {

    private IPlayer players[];
    private GameStatus status;
    private int gameCount;
    private int timeout;
    private int size;
    
    private IPlayer originalP1; 
    private IPlayer originalP2;

    // --- CLASE AUXILIAR NUEVA PARA GUARDAR EL RESULTADO Y LOS MOVIMIENTOS ---
    private class MatchResult {
        PlayerType winner;
        int totalMoves;
        
        public MatchResult(PlayerType w, int m) {
            this.winner = w;
            this.totalMoves = m;
        }
    }

    public static void main(String[] args) { 
        
        int boardSize = 7;
        int timeout = 3; 
        
        // =========================================================
        // TORNEO 1: PROFUNDIDAD FIJA (3) vs MVP
        // =========================================================
        System.out.println("\n\n#################################################");
        System.out.println("###  FASE 1: MINIMAX FIJO (DEPTH 3) vs MVP    ###");
        System.out.println("#################################################");
        
        IPlayer fixedPlayer = new PlayerMiniMaxIDS(); 
        IPlayer mvp1 = new MOustValuablePlayer(); 
        
        HeadlessGame torneoFijo = new HeadlessGame(fixedPlayer, mvp1, boardSize, timeout, 50); // 10 partidas
        GameResult resFijo = torneoFijo.start();
        System.out.println(resFijo);

        
        // =========================================================
        // TORNEO 2: IDS (TIEMPO) vs MVP
        // =========================================================
        System.out.println("\n\n#################################################");
        System.out.println("###  FASE 2: MINIMAX IDS vs MVP               ###");
        System.out.println("#################################################");
        
        IPlayer idsPlayer = new PlayerMiniMaxIDS(); 
        IPlayer mvp2 = new MOustValuablePlayer(); 
        
        HeadlessGame torneoIDS = new HeadlessGame(idsPlayer, mvp2, boardSize, timeout, 50); // 10 partidas
        GameResult resIDS = torneoIDS.start();
        System.out.println(resIDS);
    }

    public HeadlessGame(IPlayer p1, IPlayer p2, int size, int timeout, int gameCount) {
        this.size = size;
        this.players = new IPlayer[2];
        this.originalP1 = p1;
        this.originalP2 = p2;
        players[0] = p1;
        players[1] = p2;
        this.gameCount = gameCount;
        this.timeout = timeout;
    }

    public GameResult start() {
        GameResult gr = new GameResult();
        int mitad = gameCount / 2;

        for (int i = 0; i < gameCount; i++) {
            
            if (i < mitad) {
                players[0] = originalP1;
                players[1] = originalP2;
                System.out.println("\n=== PARTIDA " + (i + 1) + "/" + gameCount + " (EMPIEZA: " + players[0].getName() + ") ===");
            } else {
                players[0] = originalP2;
                players[1] = originalP1;
                System.out.println("\n=== PARTIDA " + (i + 1) + "/" + gameCount + " (EMPIEZA: " + players[0].getName() + ") ===");
            }

            // Aquí ahora pasamos el objeto MatchResult completo
            gr.update(play());
        }
        return gr;
    }

    private class Result {
        public boolean ok;
    }

    // --- MODIFICADO: Ahora devuelve MatchResult en lugar de PlayerType ---
    private MatchResult play() {
        this.status = new GameStatus(size);
        int moveCounter = 0; // CONTADOR DE MOVIMIENTOS

        while (!this.status.isGameOver()) {
            moveCounter++; // Aumentamos en cada turno

            final Semaphore semaphore = new Semaphore(1);
            semaphore.tryAcquire();

            final Result r = new Result();
            final PlayerType cp = status.getCurrentPlayer();
            
            Thread t1 = new Thread(() -> {
                PlayerMove m = null;
                try {
                    m = players[cp == PlayerType.PLAYER1 ? 0 : 1].move(new GameStatus(status));
                } catch(Exception ex) {
                    System.out.println("Excepció descontrolada al player:"+cp.name());
                    ex.printStackTrace();
                }
                
                if (m != null) {
                    try {
                        // IMPRESIÓN DE STATS (Igual que antes)
                        IPlayer currentPlayerObj = players[cp == PlayerType.PLAYER1 ? 0 : 1];
                        String nombre = currentPlayerObj.getName();
                        if (nombre.contains("PerezQuerol") || nombre.contains("PEREZQUEROL")) {
                            System.out.println(">> [YO] Movimiento: " + m.getPoints());
                            imprimirStatsDeFuerza(m);
                        } 

                        for(Point p : m.getPoints()){
                            if(cp != status.getCurrentPlayer()) throw new Exception("Invalid move sequence");
                            status.placeStone(p);
                        }
                        if( !status.isGameOver() && cp == status.getCurrentPlayer()) throw new Exception("Invalid move sequence");
                    
                    } catch(Exception ex){
                        System.out.println("Excepció descontrolada al player:"+cp.name());
                        ex.printStackTrace();
                        status.forceLoser(cp);    
                    }                    
                    
                } else {
                    status.forceLoser(cp);
                }
                r.ok = true;
                semaphore.release();
            });

            Thread t2 = new Thread(() -> {
                try {
                    Thread.sleep(HeadlessGame.this.timeout * 1000);
                } catch (InterruptedException ex) {
                }
                if (!r.ok) {
                    players[cp == PlayerType.PLAYER1 ? 0 : 1].timeout();
                }
            });

            t1.start();
            t2.start();
            long WAIT_EXTRA_TIME = 2000;
            try {
                if (!semaphore.tryAcquire(1, timeout * 1000 + WAIT_EXTRA_TIME, TimeUnit.MILLISECONDS)) {
                    System.out.println("Espera il·legal ! Player trampós:"+cp.name());
                    semaphore.acquire();
                }

            } catch (InterruptedException ex) {
                Logger.getLogger(HeadlessGame.class.getName()).log(Level.SEVERE, null, ex);
            }
            gc();
        }
        
        PlayerType winner = status.GetWinner();
        String winnerName = (winner == PlayerType.PLAYER1) ? players[0].getName() : (winner == PlayerType.PLAYER2 ? players[1].getName() : "Empate");
        System.out.println("\n|||| GANADOR PARTIDA: " + winnerName + " (Movimientos: " + moveCounter + ") ||||");
        
        // Devolvemos quién ganó Y cuántos turnos duró
        return new MatchResult(winner, moveCounter);
    }

    private void imprimirStatsDeFuerza(PlayerMove m) {
        try {
            Class<?> clase = m.getClass();
            Field[] campos = clase.getDeclaredFields(); 
            
            System.out.print("   [STATS] ");
            boolean datosEncontrados = false;

            for (Field f : campos) {
                f.setAccessible(true); 
                String nombre = f.getName();
                Object valor = f.get(m);

                if (!nombre.equals("path") && !nombre.equals("points")) {
                      System.out.print(nombre + ": " + valor + " | ");
                      datosEncontrados = true;
                }
            }
            if (!datosEncontrados) System.out.print(" (No se pudieron leer campos internos)");
            System.out.println(); 

        } catch (Exception e) {
            System.out.println("   [ERROR LEYENDO STATS]: " + e.getMessage());
        }
    }

    private class GameResult {

        // --- CAMBIO: Ahora guardamos MatchResult en lugar de solo PlayerType ---
        java.util.List<MatchResult> results;

        public GameResult() {
            results = new ArrayList<MatchResult>();
        }

        public void update(MatchResult res) {
            results.add(res);
        }

        @Override
        public String toString() {
            String res = "\n ================================================================="+
                         "\n ================        RESULTADOS FINAL       =================="+
                         "\n =================================================================\n";
            
            int winsPerez = 0;
            int winsRival = 0;
            int ties = 0;
            
            // Variables para estadísticas de movimientos
            long totalMoves = 0;
            long totalMovesWhenPerezLoses = 0;
            int lossesPerez = 0;

            int mitad = gameCount / 2;

            for (int i = 0; i < results.size(); i++) {
                MatchResult r = results.get(i);
                PlayerType winner = r.winner;
                int moves = r.totalMoves;
                
                totalMoves += moves;
                
                // Determinamos quién es PEREZ en esta partida concreta
                IPlayer p1_in_game = (i < mitad) ? originalP1 : originalP2;
                boolean perezIsP1 = (p1_in_game == originalP1);

                if (winner == null) {
                    ties++;
                } else {
                    if (winner == PlayerType.PLAYER1) {
                        if (perezIsP1) winsPerez++;
                        else { 
                            winsRival++;
                            // Perez perdió aquí
                            lossesPerez++;
                            totalMovesWhenPerezLoses += moves;
                        }
                    } 
                    else { // Ganó PLAYER2
                        if (perezIsP1) {
                            winsRival++;
                            // Perez perdió aquí (era P1 y ganó P2)
                            lossesPerez++;
                            totalMovesWhenPerezLoses += moves;
                        }
                        else winsPerez++;
                    }
                }
            }

            double avgMoves = results.isEmpty() ? 0 : (double) totalMoves / results.size();
            double avgResistance = lossesPerez == 0 ? 0 : (double) totalMovesWhenPerezLoses / lossesPerez;

            res += "TOTAL PARTIDAS: " + results.size() + "\n";
            res += "------------------------------------------------\n";
            res += "YO (" + originalP1.getName() + "): \t WINS: " + winsPerez + "\n";
            res += "RIVAL (" + originalP2.getName() + "): \t WINS: " + winsRival + "\n";
            res += "EMPATES: \t\t\t " + ties + "\n";
            res += "------------------------------------------------\n";
            res += "PROMEDIO MOVIMIENTOS GLOBAL: \t " + String.format("%.2f", avgMoves) + "\n";
            res += "RESISTENCIA (Avg Moves al perder): \t " + String.format("%.2f", avgResistance) + "\n";
            
            return res;
        }
    }

    public static void gc() {
        Object obj = new Object();
        WeakReference ref = new WeakReference<Object>(obj);
        obj = null;
        while (ref.get() != null) {
            System.gc();
        }
    }
}