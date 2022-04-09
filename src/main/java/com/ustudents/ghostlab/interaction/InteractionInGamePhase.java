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

    public String putQuestionOnMoveChoice(String question, String[] correctAnswers, Scanner sc){
        String choice = Utils.gameStepChoice(question, correctAnswers, sc);
        /*if(choice.equals("left")){

        }else if(choice.equals("right")){

        }else if(choice.equals("top")){

        }else if(choice.equals("bottom")){

        }*/

        return "";
    }

    public int putQuestionOnGamePhase(BufferedReader br, PrintWriter pw,
                                              String question, String[] correctAnswers, Scanner sc) throws IOException {

        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("move")){
                String moveState = putQuestionOnMoveChoice("Where do you want to go ? (left/right/top/bottom)",
                        new String[]{"left","right","top","bottom"}, sc);
            }else if(choice.equals("quit")){
                clientSender = "IQUIT***\n";
            }

            pw.print(clientSender);
            pw.flush();

            int state = getQuestionInGamePhase(br, choice);
            if(state == 1){
                return 1;
            }else if(state == 2){
                return 2;
            }
        }

    }

    public int getQuestionInGamePhase(BufferedReader br, String clientChoice) throws IOException {
        System.out.println("test");
        String message = br.readLine();
        System.out.println(message);
        String[] list = message.split(" ");
        if(list[0].equals("MOVE!")){

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
