package com.ustudents.ghostlab.interaction;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.client.Utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

import static java.lang.System.exit;
import static java.lang.Thread.sleep;

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
            return "LEMOV " + nbMove + "***";
        }else if(choice.equals("right")){
            return "RIMOV " + nbMove + "***";
        }else if(choice.equals("up")){
            return "UPMOV " + nbMove + "***";
        }else{
            return "DOMOV " + nbMove + "***";
        }
    }

    public void putQuestionOnGamePhase(BufferedReader br, PrintWriter pw,
                                       String question, String[] correctAnswers,
                                       Scanner sc) throws IOException {

        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("move")){
                clientSender = putQuestionOnMoveChoice("Where do you want to go ? " +
                                "(left/right/up/down)", "How much case would you move ? ",
                                 new String[]{"left","right","up","down"}, sc);

            }else if(choice.equals("list")) {
                clientSender = "GLIS?***";
            }else if(choice.equals("messall")) {
                clientSender = "MALL? " + Utils.messageToSend() + "***";
            }else if(choice.equals("messto")){
                String playerId = Utils.getInput("Who is the player ?", sc, 0);
                clientSender = "SEND? " + playerId + " " + Utils.messageToSend() + "***";
            } else if(choice.equals("quit")){
                clientSender = "IQUIT***";
            }

            pw.print(clientSender);
            pw.flush();

            getQuestionInGamePhase(br);
        }

    }

    public void getListOfPlayersInGame(BufferedReader br, int numberOfPlayer) throws IOException {
        for(int i = 0; i < numberOfPlayer; i++){
            String message = Utils.read(br);
            String[] list = message.split(" ");
            int xpos = Integer.parseInt(list[2]);
            int ypos = Integer.parseInt(list[3]);
            int score = Integer.parseInt(list[4].substring(0, list[4].length()-3));
            System.out.println("[" + list[1] + "] : his actual position : (" +
                    xpos + "," + ypos + ") with " + score + " points");
        }
    }

    public void getQuestionInGamePhase(BufferedReader br) throws IOException {
        String message = Utils.read(br);
        String[] list = message.split(" ");
        if(list[0].equals("MOVE!")) {
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2].substring(0, list[2].length()-3)));
        }else if(list[0].equals("MOVEF")) {
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2]));
            client.setCurrentPos(Integer.parseInt(list[1]), Integer.parseInt(list[2]));
            client.setScore(Integer.parseInt(list[3].substring(0, list[3].length() - 3)));

        }else if(list[0].equals("GLIS!")) {
            getListOfPlayersInGame(br, Integer.parseInt(list[1].substring(0, list[1].length() - 3)));
        }else if(list[0].equals("MALL!***") || list[0].equals("SEND!***")) {
            System.out.println("Your message has been sended");
        }else if(list[0].equals("GOBYE***")){
            System.out.println("Connection closing");
            if(!client.getSocket().isClosed()){
                client.getSocket().close();
                exit(0);
            }
        }
    }
}
