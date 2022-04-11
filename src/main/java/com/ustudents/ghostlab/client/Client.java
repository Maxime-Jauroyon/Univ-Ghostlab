package client;

import interaction.InteractionInGamePhase;
import interaction.InteractionIntroductionPhase;
import runnable.TCPRunnable;
import runnable.UDPMulticastRunnable;

import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.security.spec.ECField;
import java.util.Scanner;

public class Client {
    private final Socket socket;
    private DatagramSocket datagramSocketSender;
    private DatagramSocket datagramSocketReceived;
    private int gameRegistered;
    private int heigthMaze;
    private int widthMaze;
    private int numberOfghost;
    private String multicastAddr;
    private String multicastPort;
    private String idPlayer;
    private final int[] startedPos;
    private final int[] currentPos;
    private int score;



    private Client(String ipv4, int tcpPort, int udpPort) throws IOException {
        socket = new Socket(ipv4, tcpPort);
        datagramSocketSender = new DatagramSocket();
        datagramSocketReceived = new DatagramSocket(udpPort);
        startedPos = new int[2];
        currentPos = new int[2];
        score = 0;
    }

    public Socket getSocket() {
        return socket;
    }

    public int getUdpPort() {
        return datagramSocketReceived.getLocalPort();
    }

    public void setGameRegister(int gameRegister) {
        this.gameRegistered = gameRegister;
    }

    public void setHeigthMaze(int heigthMaze) {
        this.heigthMaze = heigthMaze;
    }

    public void setWidthMaze(int widthMaze) {
        this.widthMaze = widthMaze;
    }

    public void setNumberOfghost(int numberOfghost) {
        this.numberOfghost = numberOfghost;
    }

    public void setMulticastAddr(String multicastAddr) {
        this.multicastAddr = multicastAddr;
    }

    public void setMulticassPort(String multicassPort) {
        this.multicastPort = multicassPort;
    }

    public void setIdPlayer(String idPlayer) {
        this.idPlayer = idPlayer;
    }

    public void setStartedPos(int h, int w){
        System.out.println("Your position on the maze are : (" +  h + "," + w + ")");
        startedPos[0] = h;
        startedPos[1] = w;
    }

    public void setCurrentPos(int h, int w){
        System.out.println("Your position on the maze are : (" +  h + "," + w + ")");
        currentPos[0] = h;
        currentPos[1] = w;
    }

    public void setScore(int score) {
        System.out.println("Your score are : " + score);
        this.score = score;
    }

    public void launchGame() throws IOException, InterruptedException {
        Scanner sc = new Scanner(System.in);
        BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter pw = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));

        while(true){

            InteractionIntroductionPhase iip = new InteractionIntroductionPhase(this);
            iip.getQuestionInIntroductionPhase(br, "");

            String question = "Would you create a new game or join an existant game ? ";
            question += "Or would you know some information about game ? (register/new/size/list/game/quit)";

            int state = iip.putQuestionOnIntroductionPhase(br, pw, question,
                    new String[]{"register","new","size","list","game","quit"}, sc);

            if(state == 3){
                return;
            }

            System.out.println("I'am at the end of the register phase");

            question = "Would you start the game ? ";
            question += "Or would you know some information about game ? (start/unregister/size/list/game/quit)";
            state = iip.putQuestionOnIntroductionPhase(br, pw, question,
                    new String[]{"start","unregister","size","list","game","quit"}, sc);

            if(state == 3){
                return;
            }else if(state == 2){
                continue;
            }

            System.out.println("Enter in main phase of the game");
            iip.getQuestionInIntroductionPhase(br, "");

            Thread udpMulticastThread = new Thread(new UDPMulticastRunnable(socket));
            Thread tcpThread = new Thread(new TCPRunnable(this, br, pw, sc));


            tcpThread.start();
            udpMulticastThread.start();
            tcpThread.join();
            udpMulticastThread.join();


        }
    }

    public static void main(String[] args){
        try{
            if(Utils.commandsArgsFormatIsCorrect(args)){
                Client c = new Client(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]));
                c.launchGame();
            }else{
                System.out.println("Wrong argument: you must give an ip and ipv4 address");
            }

        }catch (Exception e){
            e.printStackTrace();
            System.out.println("Connection refused : wrong ipv4 address or port");
        }

    }
}
