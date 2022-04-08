import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class InteractionIntroductionPhase {
    private final Client client;

    public InteractionIntroductionPhase(Client client){
        this.client = client;
    }

    public int putQuestionOnIntroductionPhase(Socket tcpSocket, BufferedReader br, PrintWriter pw,
                                               String question, String[] correctAnswers, Scanner sc) throws IOException {
        while(true){
            String choice = Utils.gameStepChoice(question, correctAnswers, sc);
            String clientSender = "";

            if(choice.equals("register")){
                String username = Utils.getInput("Now choose your username : ", sc, 0);
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "REGIS " + username + " " + client.getUdpPort() + " " + gameId + "***\n";
            }else if(choice.equals("new")){
                String username = Utils.getInput("Now choose your username : ", sc, 0);
                clientSender = "NEWPL " + username + " " + client.getUdpPort() + "***\n";
            }else if(choice.equals("unregister")){
                clientSender = "UNREG***\n";
            }else if(choice.equals("size")){
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "SIZE? " + gameId + "***\n";
            }else if(choice.equals("list")){
                String gameId = Utils.getInput("Now choose your game id number : ", sc, 1);
                clientSender = "LIST? " + gameId + "***\n";
            }else if(choice.equals("game")){
                clientSender = "GAME?***\n";
            }else if(choice.equals("start")){
                clientSender = "START***\n";
            }else if(choice.equals("quit")){
                System.out.println("Connection closing");
                tcpSocket.close();
                return 0;
            }

            pw.print(clientSender);
            pw.flush();

            int state = getQuestionOnIntroductionPhase(br, choice);
            if(state == 1){
                return 0;
            }else if(state == 2){
                return 1;
            }
        }
    }

    public int getQuestionOnIntroductionPhase(BufferedReader br, String clientChoice) throws IOException {
        String message = br.readLine();
        if(message.startsWith("REGOK")) {
            client.setGameRegister(Integer.parseInt(message.substring(6, Utils.findEndIndex(message, 6, '*'))));
            return 1;
        }else if(message.startsWith("WELCO")){

            return 1;
        }else if(message.startsWith("LIST!") || message.startsWith("GAMES")) {
            String gameId = "";
            boolean startWithList = message.startsWith("LIST!");
            if (startWithList) {
                gameId = message.substring(6, Utils.findEndIndex(message, 6, ' '));
            }
            int startIndex = ((message).startsWith("LIST!")) ? 8 : 6;
            int numberOf = Integer.parseInt(message.substring(startIndex, Utils.findEndIndex(message, startIndex, '*')));
            for (int i = 0; i < numberOf; i++) {
                message = br.readLine();
                if (startWithList) {
                    String playerId = message.substring(6, Utils.findEndIndex(message, 6, '*'));
                    System.out.println("Game id : " + gameId + " and player id inside the game : " + playerId);
                } else {
                    gameId = message.substring(7, Utils.findEndIndex(message, 7, ' '));
                    String nbPlayer = message.substring(9, Utils.findEndIndex(message, 9, '*'));
                    System.out.println("Game id : " + gameId + " and number of player inside the game : "
                            + nbPlayer);
                }
            }

        }else if(message.startsWith("SIZE!")) {
            String gameId = message.substring(6, Utils.findEndIndex(message, 6, ' '));
            String height = message.substring(8, Utils.findEndIndex(message, 8, ' '));
            String width = message.substring(10, Utils.findEndIndex(message, 10, '*'));
            System.out.println("Game id : " + gameId + " , height : " + height +
                    " et width : " + width + " of the maze");

        }else if(message.startsWith("UNROK")){
            String gameId = message.substring(6, Utils.findEndIndex(message, 6, '*'));
            System.out.println("Unregister of game : " + gameId);
            return 2;

            //TODO: make function to explain all existant error in program
        }else{
            System.out.println("Error : Wrong using of " + clientChoice);
        }

        return 0;
    }
}
