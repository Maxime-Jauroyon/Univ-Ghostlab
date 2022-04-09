package interaction;
import client.Client;
import client.Utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class InteractionInGamePhase extends InteractionPerPhase {

    public InteractionInGamePhase(Client client) {
        super(client);
    }

    public String putQuestionOnMoveChoice(String firstQuestion, String secondQuestion,
                                          String[] correctAnswers, Scanner sc){

        String choice = Utils.gameStepChoice(firstQuestion, correctAnswers, sc);
        String nbMove = Utils.getInput(secondQuestion, sc, 1);
        if(nbMove.length() > 3)
            nbMove = nbMove.substring(0,3);
        else{
            for(int i = 3 - nbMove.length(); i > 0; i--){
                nbMove = "0" + nbMove;
            }
        }


        if(choice.equals("left")){
            return "LEMOV " + nbMove + "***\n";
        }else if(choice.equals("right")){
            return "RIMOV " + nbMove + "***\n";
        }else if(choice.equals("up")){
            return "UPMOV " + nbMove + "***\n";
        }else{
            return "DOMOV " + nbMove + "***\n";
        }
    }

    public int putQuestionOnGamePhase(BufferedReader br, PrintWriter pw,
                                              String question, String[] correctAnswers, Scanner sc) throws IOException {

        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("move")){
                clientSender = putQuestionOnMoveChoice("Where do you want to go ? " +
                                "(left/right/up/down)", "How much case would you move ? ",
                                 new String[]{"left","right","up","down"}, sc);

            }else if(choice.equals("list")){
                clientSender = "GLIS?***\n";
            } else if(choice.equals("quit")){
                clientSender = "IQUIT***\n";
            }

            pw.print(clientSender);
            pw.flush();

            int state = getQuestionInGamePhase(br);
            if(state == 1){
                return 1;
            }else if(state == 2){
                return 2;
            }
        }

    }

    public void getListOfPlayersInGame(BufferedReader br, int numberOfPlayer) throws IOException {
        for(int i = 0; i < numberOfPlayer; i++){
            String message = br.readLine();
            String[] list = message.split(" ");
            int xpos = Integer.parseInt(list[2]);
            int ypos = Integer.parseInt(list[3]);
            int score = Integer.parseInt(list[4].substring(0, list[4].length()-3));
            System.out.println("Player " + (i+1) + " : " + list[1] + ", her position on the maze : (" +
                    xpos + "," + ypos + ") and his score : " + score);
        }
    }

    public int getQuestionInGamePhase(BufferedReader br) throws IOException {
        String message = br.readLine();
        String[] list = message.split(" ");

        if(list[0].equals("MOVE!")) {
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2].substring(0, list[2].length()-3)));
        }else if(list[0].equals("MOVEF")) {
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2]));
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2]));
            client.setScore(Integer.parseInt(list[3].substring(0, list[3].length() - 3)));

        }else if(list[0].equals("GLIS!")){
            getListOfPlayersInGame(br, Integer.parseInt(list[1].substring(0, list[1].length()-3)));
        }else if(list[0].equals("GOBYE***")){
            System.out.println("Connection closing");
            if(!client.getSocket().isClosed()){
                client.getSocket().close();
            }
            return 1;
        }
        return 0;
    }
}
