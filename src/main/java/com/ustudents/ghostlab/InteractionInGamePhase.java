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

    public int putQuestionOnGamePhase(Socket tcpSocket, BufferedReader br, PrintWriter pw,
                                              String question, String[] correctAnswers, Scanner sc) throws IOException {

        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("move")){
                String moveState = putQuestionOnMoveChoice("Where do you want to go ? (left/right/top/bottom)",
                        new String[]{"left","right","top","bottom"}, sc);
            }/*else if(choice.equals("list")){

            }*/
        }

        return 0;
    }

    public int getQuestionOnIntroductionPhase(BufferedReader br, String clientChoice) throws IOException {

        return 0;
    }
}
