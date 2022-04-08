import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    private final String ipv4;
    private final int tcpPort;
    private final int udpPort;
    private int gameRegister;
    private int heigthMaze;
    private int widthMaze;
    private int numberOfghost;
    private String multicastAddr;
    private String multicassPort;

    private Client(String ipv4, int tcpPort, int udpPort){
        this.ipv4 = ipv4;
        this.tcpPort = tcpPort;
        this.udpPort = udpPort;
    }

    public int getUdpPort() {
        return udpPort;
    }

    public void setGameRegister(int gameRegister) {
        this.gameRegister = gameRegister;
    }

    public void launchGame(){
        try{
            System.out.println("ipv4 : " + ipv4 + " , tcpPort : " + tcpPort + " udpPort : " + udpPort);
            Socket tcpSocket = new Socket(ipv4, tcpPort);
            DatagramSocket udpSocket = new DatagramSocket(udpPort);
            Scanner sc = new Scanner(System.in);
            BufferedReader br = new BufferedReader(new InputStreamReader(tcpSocket.getInputStream()));
            PrintWriter pw = new PrintWriter(new OutputStreamWriter(tcpSocket.getOutputStream()));

            while(true){

                String message = br.readLine();
                int nbGames = Integer.parseInt(message.substring(6, Utils.findEndIndex(message, 6, '*')));
                System.out.println("nbGames : " + nbGames);

                for(int i = 0; i < nbGames; i++){
                    message = br.readLine();
                    String gameId = message.substring(7, Utils.findEndIndex(message, 7, ' '));
                    String nbPlayer = message.substring(9, Utils.findEndIndex(message, 9, '*'));
                    System.out.println("Game id : " + gameId + " and number of player inside the game : " + nbPlayer);
                }

                InteractionIntroductionPhase iip = new InteractionIntroductionPhase(this);
                String question = "Would you create a new game or join an existant game ? ";
                question += "Or would you know some information about game ? (register/new/size/list/game/quit)";

                iip.putQuestionOnIntroductionPhase(tcpSocket, br, pw, question,
                        new String[]{"register","new","size","list","game","quit"}, sc);

                System.out.println("I'am at the end of the register phase");

                question = "Would you start the game ? ";
                question += "Or would you know some information about game ? (start/unregister/size/list/game/quit)";
                int state = iip.putQuestionOnIntroductionPhase(tcpSocket, br, pw, question,
                        new String[]{"start","unregister","size","list","game","quit"}, sc);


                if(state == 2){
                    continue;
                }

                System.out.println("Enter in main phase of the game");

            }

        }catch(Exception e){
            System.out.println("Connection refused : wrong ipv4 address or port");
        }

    }

    public static void main(String[] args){
        if(Utils.commandsArgsFormatIsCorrect(args)){
            Client c = new Client(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]));
            c.launchGame();
        }else{
            System.out.println("Wrong argument: you must give an ip and ipv4 address");
        }
    }
}
