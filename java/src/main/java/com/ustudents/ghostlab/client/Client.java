package com.ustudents.ghostlab.client;

import com.ustudents.application.Application;
import com.ustudents.application.window.WindowInfo;
import com.ustudents.common.command.Command;
import com.ustudents.ghostlab.interaction.InteractionIntroductionPhase;
import com.ustudents.ghostlab.runnable.TCPRunnable;
import com.ustudents.ghostlab.runnable.UDPMulticastRunnable;
import com.ustudents.ghostlab.runnable.UDPRunnable;
import imgui.ImGui;

import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.Scanner;

@Command(name = "client", version = "1.0.0", description = "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!")
@WindowInfo(title = "Ghostlab Client", width = 1280, height = 720)
public class Client extends Application {
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

    public Client(String ipv4, int tcpPort, String username, int udpPort) throws IOException {
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

    @Override
    protected void initialize() {

    }

    @Override
    protected void renderImGui() {
        ImGui.text("Hello, World!");
        ImGui.showDemoWindow();
    }

    @Override
    protected void destroy() {

    }
}
