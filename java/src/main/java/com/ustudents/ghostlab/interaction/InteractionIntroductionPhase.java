package com.ustudents.ghostlab.interaction;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.client.Utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class InteractionIntroductionPhase extends InteractionPerPhase {

    public InteractionIntroductionPhase(Client client){
        super(client);
    }

    public int putQuestionOnIntroductionPhase(BufferedReader br, PrintWriter pw,
                                               String question, String[] correctAnswers, Scanner sc) throws IOException {
        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("register")){
                /*if(client.getUsername() == null)
                    client.setUsername(Utils.getInput("Now choose your username : ", sc, 0));
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "REGIS " + client.getUsername() + " " + client.getUdpPort() + " " + gameId + "***";
            */}else if(choice.equals("new")){
                /*if(client.getUsername() == null)
                    client.setUsername(Utils.getInput("Now choose your username : ", sc, 0));
                clientSender = "NEWPL " + client.getUsername() + " " + client.getUdpPort() + "***";*/
            }else if(choice.equals("unregister")){
                clientSender = "UNREG***";
            }else if(choice.equals("size")){
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "SIZE? " + gameId + "***";
            }else if(choice.equals("list")){
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "LIST? " + gameId + "***";
            }else if(choice.equals("game")){
                clientSender = "GAME?***";
            }else if(choice.equals("start")){
                clientSender = "START***";
            }else if(choice.equals("quit")){
                clientSender = "IQUIT***";
            }

            pw.print(clientSender);
            pw.flush();

            int state = getQuestionInIntroductionPhase(br, choice);
            if(state == 1){
                return 1;
            }else if(state == 2){
                return 2;
            }else if(state == 3){
                return 3;
            }
        }
    }

    public int getQuestionInIntroductionPhase(BufferedReader br, String clientChoice) throws IOException {
        String message = Utils.read(br);
        client.addContentTologs("$",message);
        String[] list = message.split(" ");
        if(list[0].equals("REGOK")) {
            //client.setGameRegister(Integer.parseInt(list[1].substring(0, list[1].length()-3)));
            return 1;
        }else if(list[0].equals("WELCO")){
            /*client.setGameRegister(Integer.parseInt(list[1]));
            client.setHeigthMaze(Integer.parseInt(list[2]));
            client.setWidthMaze(Integer.parseInt(list[3]));
            client.setNumberOfghost(Integer.parseInt(list[4]));
            client.setMulticastAddr(list[5].substring(0, list[5].length()-3));
            client.setMulticastPort(Integer.parseInt(list[6].substring(0, list[6].length()-3)));*/
            return 1;
        }else if(list[0].equals("POSIT")){
            /*client.setIdPlayer(list[1]);
            client.setStartedPos(Integer.parseInt(list[2]), Integer.parseInt(list[3].substring(0, list[3].length()-3)));
        */}else if(list[0].equals("LIST!") || list[0].equals("GAMES")) {
            String gameId = "";
            boolean startWithList = message.startsWith("LIST!");
            if (startWithList) {
                gameId = list[1];
            }
            String getCorrectNumberOf = (startWithList)? list[2].substring(0, list[2].length()-3):
                    list[1].substring(0, list[1].length()-3);
            client.addContentTologs("$",message);
            client.addContentTologs("$", getCorrectNumberOf);
            int numberOf = Integer.parseInt(getCorrectNumberOf);

            for (int i = 0; i < numberOf; i++) {
                message = Utils.read(br);
                list = message.split(" ");
                if (startWithList) {
                    String playerId = list[1].substring(0, list[1].length()-3);
                    client.addContentTologs("client:", "[Game " + gameId + "] : " + playerId + " is currently in the game");
                    //System.out.println("[Game " + gameId + "] : " + playerId + " is currently in the game");
                } else {
                    gameId = list[1];
                    String nbPlayer = list[2].substring(0, list[2].length()-3);
                    client.addContentTologs("client:", "[Game " + gameId + "] : He has " + nbPlayer + " player(s) in the waiting room");
                    //System.out.println("[Game " + gameId + "] : He has " + nbPlayer + " player(s) in the waiting room");
                }
            }

        }else if(list[0].equals("SIZE!")) {
            String gameId = list[1];
            String height = list[2];
            String width = list[3].substring(0, list[3].length()-3);
            System.out.println("[Game " + gameId + "] : " + height + " of height" +
                    " by " + width + " of width");

        }else if(list[0].equals("UNROK")){
            String gameId = list[1].substring(0, list[1].length()-3);
            System.out.println("[Game " + gameId + "] : Your are unregister");
            return 2;

            //TODO: make function to explain all existant error in program
        }else if(list[0].equals("GOBYE***")){
            System.out.println("Connection closing");
            if(!client.getSocket().isClosed()){
                client.getSocket().close();
            }
            return 3;
        }else{
            System.out.println("Error : Wrong using of " + clientChoice);
        }

        return 0;
    }
}
