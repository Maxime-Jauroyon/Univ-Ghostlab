package com.ustudents.ghostlab.client;

import com.ustudents.application.Application;
import com.ustudents.application.window.WindowInfo;
import com.ustudents.common.command.Command;
import com.ustudents.ghostlab.communication.Sender;
import com.ustudents.ghostlab.listener.TCPRunnable;
import com.ustudents.ghostlab.listener.UDPRunnable;
import com.ustudents.ghostlab.scene.GameLobbyScene;
import com.ustudents.ghostlab.scene.MainScene;
import com.ustudents.ghostlab.scene.SceneData;
import com.ustudents.ghostlab.scene.UsernameChoiceScene;

import imgui.type.ImBoolean;
import imgui.type.ImString;

import java.io.*;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import static java.lang.System.exit;

@Command(name = "client", version = "1.0.0", description = "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!")
@WindowInfo(title = "Ghostlab Client", width = 1280, height = 720)
public class Client extends Application {
    private final Socket socket;
    private final DatagramSocket datagramSocket;
    private final TCPRunnable tcpRunnable;
    private final PrintWriter pw;
    private String username;
    private String gameRegistered;
    /*private final Thread tcpThread;
    private final Thread udpThread;*/
    //private String username;
    /*private int gameRegistered;
    private int heigthMaze;
    private int widthMaze;
    private int numberOfghost;
    private String multicastAddr;
    private int multicastPort;
    private String idPlayer;
    private int[] startedPos;
    private int[] currentPos;
    private int score;*/

    public Client(String ipv4, int tcpPort, String username, int udpPort) throws IOException {
        addContentTologs("client:", "connection to server established.",1);
        socket = new Socket(ipv4, tcpPort);
        datagramSocket = new DatagramSocket(udpPort);
        tcpRunnable = new TCPRunnable(this);
        Thread tcpThread = new Thread(tcpRunnable);
        Thread udpThread = new Thread(new UDPRunnable(this, datagramSocket));
        tcpThread.start();
        udpThread.start();
        addContentTologs("client:", "udp listener thread started.",1);
        addContentTologs("client:", "tcp listener thread started.",1);
        this.pw = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
        this.username = username;
        gameRegistered = "-1";
        /*startedPos = new int[2];
        currentPos = new int[2];
        score = 0;*/
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

    public void SendRequest(String action) throws IOException{
        Sender.send(pw, action);
    }

    public String getGameRegister(){
        return gameRegistered;
    }

    public void setGameRegister(String gameRegister) {
        this.gameRegistered = gameRegister;
    }

    /*public void setHeigthMaze(int heigthMaze) {
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
    }*/

    /*public void launchGame() throws IOException, InterruptedException {
        Scanner sc = new Scanner(System.in);
        BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter pw = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
        addContentTologs("client:", "leo is here !");
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
    }*/

    // -----------------------------------------------------------------------------------------------------------------
    private int currentScene;
    private final List<String> logs = new ArrayList<>();
    private final List<String> serverAnswers = new ArrayList<>();

    private ImBoolean consoleShowInfo = new ImBoolean(true);
    private ImBoolean consoleShowWarning = new ImBoolean(true);
    private ImBoolean consoleShowError = new ImBoolean(true);
    private ImString consoleCommand = new ImString();
    private ImString usernameChoiceContent = new ImString();
    private ImString gameChoiceContent = new ImString();
    private int lastPressedButton = -1;
    
    public ImBoolean getConsoleShowInfo(){
        return consoleShowInfo;
    }

    public ImBoolean getConsoleShowWarning(){
        return consoleShowWarning;
    }
    
    public ImBoolean getConsoleShowError(){
        return consoleShowError;
    }

    public ImString getConsoleCommand(){
        return consoleCommand;
    }

    public ImString getUsernameChoiceContent(){
        return usernameChoiceContent;
    }

    public ImString getGameChoiceContent(){
        return gameChoiceContent;
    }

    public int getLastPressedButton(){
        return lastPressedButton;
    }

    public void setLastPressedButton(int button){
        lastPressedButton = button;
    }

    public List<String> getLogs(){
        return logs;
    }

    public String getLastServerAnswer(){
        return serverAnswers.get(serverAnswers.size() - 1);
    }

    public void addContentTologs(String separator, String content, int flag){
        if(flag == 0){
            serverAnswers.add(content);
        }
        content = separator + " " + content;
        logs.add(content);
    }
    
    public void setScene(int scene){
        currentScene = scene;
    }
    
    public void helpcommand(){
        addContentTologs("client:", "commands:", 1);
        addContentTologs("client:", "\tq, e, quit, exit   terminates the program.", 1);
        addContentTologs("client:", "\th, help            displays this help message.", 1);
        addContentTologs("client:", "\tv, version         display the program's version.", 1);

    }

    @Override
    protected void initialize() {
        addContentTologs("client:", "you can now enter commands.",1);
        helpcommand();
    }

    @Override
    protected void renderImGui() {
        try{
            if(currentScene == SceneData.SCENE_MAIN){
                new MainScene(this).display();
            }else if(currentScene == SceneData.SCENE_USERNAMECHOICE){
                new UsernameChoiceScene(this).display();
            }else if(currentScene == SceneData.SCENE_GAMELOBBY){
                new GameLobbyScene(this).display();
            }
            
        }catch(IOException e){
            e.printStackTrace();
        }
        
    }

    @Override
    protected void destroy() {
        exit(0);
    }
}
