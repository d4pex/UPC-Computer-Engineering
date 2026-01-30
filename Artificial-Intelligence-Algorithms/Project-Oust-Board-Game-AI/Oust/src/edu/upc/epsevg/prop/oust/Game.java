package edu.upc.epsevg.prop.oust;

import edu.upc.epsevg.prop.oust.players.HumanPlayer;
import edu.upc.epsevg.prop.oust.players.RandomPlayer;
import edu.upc.epsevg.prop.oust.players.PerezQuerol.PlayerMiniMax;
import edu.upc.epsevg.prop.oust.players.PerezQuerol.PlayerMiniMaxIDS;

import javax.swing.SwingUtilities;

/**
 * Oust: el joc de taula.
 * @author Bernat
 */
public class Game {
    /**
     * @param args no usats
     */
    public static void main(String[] args) { 
         
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                
                //---------------------------------------------
                // Un parell de tontets jugant 
                //---------------------------------------------
                //IPlayer player1 = new RandomPlayer("Asterix");
                //IPlayer player2 = new RandomPlayer("Obelix");

                //---------------------------------------------
                // Deixem el tontet en mans d'una mala persona
                //---------------------------------------------
                //IPlayer player1 = new RandomPlayer("Asterix");
                //IPlayer player2 = new MalaOustiaPlayer();
                
                //---------------------------------------------
                // (PlayerMiniMax OR PlayerMiniMaxIDS) vs (HUMÀ OR RANDOM OR MALAOUSTIA OR MVP)
                //---------------------------------------------
                //IPlayer player1 = new PlayerMiniMax(3);
                IPlayer player2 = new PlayerMiniMaxIDS();
                //IPlayer player2 = new HumanPlayer("");
                //IPlayer player2 = new RandomPlayer("");
                //IPlayer player2 = new MalaOustiaPlayer();
                IPlayer player1 = new MOustValuablePlayer();
                
                
                //---------------------------------------------
                // Customitzeu els paràmetres
                //---------------------------------------------
                int midaCostat = 7;
                int timeoutEnSegons = 5;
                boolean pauseEnAutomatic = false;
                
                new Board(player1 , player2, midaCostat /*mida*/,  timeoutEnSegons/*s timeout*/, pauseEnAutomatic);
             }
        });
    }
}


