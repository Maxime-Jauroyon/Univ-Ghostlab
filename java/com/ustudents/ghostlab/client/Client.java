package client;

import interaction.InteractionInGamePhase;
import interaction.InteractionIntroductionPhase;
import runnable.TCPRunnable;
import runnable.UDPMulticastRunnable;
import runnable.UDPRunnable;

import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.security.spec.ECField;
import java.util.Scanner;

import static java.lang.System.exit;

public class Client {
    private final Socket socket;
    private final DatagramSocket datagramSocket;
    private String username;
    private int gameRegistered;
    private int heigthMaze;
    private int widthMaze;
    private int numberOfghost;
    private String multicastAddr;
    private int multicastPort;
    private String idPlayer;
    private final int[] startedPos;
    private final int[] currentPos;
    private int score;



    private Client(String ipv4, int tcpPort, String username, int udpPort) throws IOException {
        socket = new Socket(ipv4, tcpPort);
        this.username = username;
        datagramSocket = new DatagramSocket(udpPort);
        startedPos = new int[2];
        currentPos = new int[2];
        score = 0;
    }

    public Socket getSocket() {
        return socket;
    }

    public int getUdpPort() {
        return datagramSocket.getLocalPort();
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
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

    public void setMulticastPort(int multicastPort) {
        this.multicastPort = multicastPort;
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

            question = "Would you start the game ? ";
            question += "Or would you know some information about game ? (start/unregister/size/list/game/quit)";
            state = iip.putQuestionOnIntroductionPhase(br, pw, question,
                    new String[]{"start","unregister","size","list","game","quit"}, sc);

            if(state == 3){
                return;
            }else if(state == 2){
                continue;
            }

            iip.getQuestionInIntroductionPhase(br, "");

            Thread tcpThread = new Thread(new TCPRunnable(this, br, pw, sc));
            Thread udpMulticastThread = new Thread(new UDPMulticastRunnable(socket, multicastAddr, multicastPort));
            Thread udpTread = new Thread(new UDPRunnable(socket, datagramSocket));

            tcpThread.start();
            udpMulticastThread.start();
            udpTread.start();
            tcpThread.join();
            udpMulticastThread.join();
            udpTread.join();

        }
    }

    private static void help(){
        System.out.println("usage: client [options]");
        System.out.println();
        System.out.println("ghostlab is an online matchmaking based game where you take" +
                " upon yourself to become the best ghost hunter!");
        System.out.println();
        System.out.println("options:");
        System.out.println("\t-i, --ip <server ip> defines the ip to connect to" +
                " (127.0.0.1 by default).");
        System.out.println("\t-p, --port <server tcp port>         defines the port to connect to " +
                "(4785 by default).");
        System.out.println("\t-n, --name <player name>             defines the name to use when connected " +
                "to a match (will be asked later if not provided).");
        System.out.println("\t-u, --udp-port,  <client udp port>   defines the udp port to use to communicate " +
                "with other players (5541 used by default).");
        System.out.println("\t-h, --help                           displays this help message and exits.");
        System.out.println("\t-v, --version                        displays the program's version and exits.\n");

    }

    public static void main(String[] args){
        try{
            String ipv4Addr = "127.0.0.1";
            int tcpPort = 4785;
            String username = null;
            int udpPort = 5541;


            for(int i = 0; i < args.length; i++){
                String[] options = (i < args.length-1)? new String[]{args[i], args[i+1]}: new String[0];


                if((args[i].equals("--ip") || args[i].equals("-i"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 0)){
                        System.out.println("Error: You must give correct ipv4 address to use " + args[i]
                                + " options");
                        exit(0);
                    }
                    ipv4Addr = args[i+1];
                    i+=1;
                }else if((args[i].equals("--port") || args[i].equals("-p"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 1)){
                        System.out.println("Error: You must give 4 numbers to use " + args[i] + " options");
                        exit(0);
                    }
                    tcpPort = Integer.parseInt(args[i+1]);
                    i+=1;
                }else if((args[i].equals("--name") || args[i].equals("-n"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 2)){
                        System.out.println("Error: You must give 8 alphanumerics characters to use " + args[i]
                                + " options");
                        exit(0);
                    }
                    username = args[i+1];
                    i+=1;
                }else if((args[i].equals("--udp-port") || args[i].equals("-u"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 1)){
                        System.out.println("Error: You must give 4 numbers to use " + args[i] + " options");
                        exit(0);
                    }
                    udpPort = Integer.parseInt(args[i+1]);
                    i+=1;
                }else if(args[i].equals("--help") || args[i].equals("-h")){
                    help();
                    exit(0);
                }else if(args[i].equals("--version") || args[i].equals("-v")){
                    System.out.println("version: 1.0.0");
                    exit(0);
                }else{
                    System.out.println("option not yet implemented : " + args[i] + " " + args[i].length());
                    exit(0);
                }
            }

            Client c = new Client(ipv4Addr, tcpPort, username, udpPort);
            c.launchGame();

        }catch (Exception e){
            e.printStackTrace();
            System.out.println("Connection refused : wrong ipv4 address or port");
        }

    }
}


