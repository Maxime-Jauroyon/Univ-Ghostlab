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
                return 1;
            }else if(state == 2){
                return 2;
            }
        }
    }

    public int getQuestionOnIntroductionPhase(BufferedReader br, String clientChoice) throws IOException {
        String message = br.readLine();
        String[] list = message.split(" ");
        if(message.startsWith("REGOK")) {
            client.setGameRegister(Integer.parseInt(list[1].substring(0, list[1].length()-3)));
            return 1;
        }else if(message.startsWith("WELCO")){
            client.setGameRegister(Integer.parseInt(list[1]));
            client.setHeigthMaze(Integer.parseInt(list[2]));
            client.setWidthMaze(Integer.parseInt(list[3]));
            client.setNumberOfghost(Integer.parseInt(list[4]));
            client.setMulticastAddr(list[5]);
            client.setMulticastAddr(list[6].substring(0, list[6].length()-3));
            return 1;
        }else if(message.startsWith("LIST!") || message.startsWith("GAMES")) {
            String gameId = "";
            boolean startWithList = message.startsWith("LIST!");
            if (startWithList) {
                gameId = list[1];
            }

            String getCorrectNumberOf = (startWithList)? list[2].substring(0, list[2].length()-3):
                    list[1].substring(0, list[1].length()-3);
            int numberOf = Integer.parseInt(getCorrectNumberOf);

            for (int i = 0; i < numberOf; i++) {
                message = br.readLine();
                list = message.split(" ");
                if (startWithList) {
                    String playerId = list[1].substring(0, list[1].length()-3);
                    System.out.println("Game id : " + gameId + " and player id inside the game : " + playerId);
                } else {
                    gameId = list[1];
                    String nbPlayer = list[2].substring(0, list[2].length()-3);
                    System.out.println("Game id : " + gameId + " and number of player inside the game : "
                            + nbPlayer);
                }
            }

        }else if(message.startsWith("SIZE!")) {
            String gameId = list[1];
            String height = list[2];
            String width = list[3].substring(0, list[3].length()-3);
            System.out.println("Game id : " + gameId + " , height : " + height +
                    " et width : " + width + " of the maze");

        }else if(message.startsWith("UNROK")){
            String gameId = list[1].substring(0, list[1].length()-3);
            System.out.println("Unregister of game : " + gameId);
            return 2;

            //TODO: make function to explain all existant error in program
        }else{
            System.out.println("Error : Wrong using of " + clientChoice);
        }

        return 1;
    }
}
