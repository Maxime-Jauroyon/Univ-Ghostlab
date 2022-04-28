package com.ustudents.ghostlab.client;

import com.ustudents.application.Application;
import com.ustudents.application.window.WindowInfo;
import com.ustudents.common.command.Command;
import com.ustudents.ghostlab.communication.Sender;
import com.ustudents.ghostlab.listener.TCPRunnable;
import com.ustudents.ghostlab.listener.MulticastRunnable;
import com.ustudents.ghostlab.listener.UDPRunnable;
import com.ustudents.ghostlab.other.GameModel;
import com.ustudents.ghostlab.scene.GameLobbyScene;
import com.ustudents.ghostlab.scene.InGameScene;
import com.ustudents.ghostlab.scene.MainScene;
import com.ustudents.ghostlab.scene.SceneData;
import com.ustudents.ghostlab.scene.UserChoiceScene;

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
    private Socket socket;
    private DatagramSocket datagramSocket;
    private TCPRunnable tcpRunnable;
    private MulticastRunnable multicastRunnable;
    //private PrintWriter pw;
    private Sender sender;
    private String ipv4_addr;
    private int tcpPort;
    private String username;
    private int udpPort;
    private String gameRegistered;
    private GameModel gameModel;
    /*private int mazeHeight;
    private int mazeWidth;
    private int[][] playerWay;*/
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

    /*public Client(String ipv4, int tcpPort, String username, int udpPort) throws IOException {
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
        gameModel = new GameModel();
    }*/

    public void launch(int flag) throws IOException {
        addContentTologs("client: info:", "connection to server established.",1);
        socket = new Socket(ipv4_addr, tcpPort);
        if(flag == 0){
            datagramSocket = new DatagramSocket(udpPort);
        }
        tcpRunnable = new TCPRunnable(this);
        Thread tcpThread = new Thread(tcpRunnable);
        Thread udpThread = new Thread(new UDPRunnable(this, datagramSocket));
        tcpThread.start();
        udpThread.start();
        addContentTologs("client: info:", "udp listener thread started.",1);
        addContentTologs("client: info:", "tcp listener thread started.",1);
        //sender = new Sender(new PrintWriter(new OutputStreamWriter(socket.getOutputStream())));
        sender = new Sender(this, socket.getOutputStream());
        gameRegistered = "-1";
        gameModel = new GameModel();
    }

    /*public void close(BufferedReader br) throws IOException{
        datagramSocket.close();
        datagramSocket = new DatagramSocket(udpPort);
    }*/

    public void launchMulticastThread(String multicastAddr, int multicastPort){
        multicastRunnable = new MulticastRunnable(this, multicastAddr, multicastPort);
        Thread multicastThread = new Thread(multicastRunnable);
        multicastThread.start();
    }

    public Socket getSocket() {
        return socket;
    }

    public int getUdpPort() {
        return datagramSocket.getLocalPort();
    }

    public Sender getSender(){
        return sender;
    }

    public String getUsername() {
        return username;
    }

    public GameModel getGameModel(){
        return gameModel;
    }

    public void setIPv4Addr(String ipv4_addr) {
        this.ipv4_addr = ipv4_addr;
    }

    public void settcpPort(int tcpPort) {
        this.tcpPort = tcpPort;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setudpPort(int udpPort) {
        this.udpPort = udpPort;
    }

    /*public void sendRequest(String action) throws IOException{
        Sender.send(pw, action);
    }*/

    public String getGameRegister(){
        return gameRegistered;
    }

    public void setGameRegister(String gameRegister) {
        this.gameRegistered = gameRegister;
    }


    /*public void setNumberOfghost(int numberOfghost) {
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
    
    private List<Integer> scene = new ArrayList<>();
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

    public void backToPreviousScene(){
        scene.remove(scene.size()-1);
    }

    public int getCurrentScene(){
        return scene.get(scene.size()-1);
    }
    
    public void setCurrentScene(int scene){
        this.scene.add(scene);
    }

    public List<String> getVisibleLog(){
        List<String> visibleLogs = new ArrayList<>();
        for(int i = 0; i < logs.size(); i++){
            String log = logs.get(i);
            if((log.startsWith("client: info:") && consoleShowInfo.get()) ||
                (log.startsWith("client: warning:") && consoleShowWarning.get()) ||
                (log.startsWith("client: error:") && consoleShowError.get())
                || log.startsWith("$"))
                visibleLogs.add(log);
        }
        return visibleLogs;
        
    }
    
    public void helpcommand(){
        addContentTologs("client: info:", "commands:", 1);
        addContentTologs("client: info:", "\tq, e, quit, exit   terminates the program.", 1);
        addContentTologs("client: info:", "\th, help            displays this help message.", 1);
        addContentTologs("client: info:", "\tv, version         display the program's version.", 1);

    }

    @Override
    protected void initialize() {
        addContentTologs("client: info:", "you can now enter commands.",1);
        helpcommand();
        scene.add(SceneData.SCENE_MAIN);
    }

    @Override
    protected void renderImGui() {
        try{
            if(getCurrentScene() == SceneData.SCENE_MAIN){
                new MainScene(this).display();
            }else if(getCurrentScene() == SceneData.SCENE_USERCHOICE){
                new UserChoiceScene(this).display();
            }else if(getCurrentScene() == SceneData.SCENE_GAMELOBBY){
                new GameLobbyScene(this).display();
            }else if(getCurrentScene() == SceneData.SCENE_INGAME){
                new InGameScene(this).display();
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
