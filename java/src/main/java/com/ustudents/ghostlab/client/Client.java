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
import java.util.SortedMap;
import java.util.TreeMap;

import static java.lang.System.exit;

@Command(name = "client", version = "1.0.0", description = "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!")
@WindowInfo(title = "Ghostlab Client", width = 1280, height = 720)
public class Client extends Application {
    private Socket socket;
    private DatagramSocket datagramSocket;
    private TCPRunnable tcpRunnable;
    private MulticastRunnable multicastRunnable;
    private Sender sender;
    private String ipv4_addr;
    private int tcpPort;
    private String username;
    private int udpPort;
    private int gameRegistered;
    private GameModel gameModel;
    private List<Integer> requestGamesId = new ArrayList<>();
    private SortedMap<Integer, List<String>> requestPlayersUsernamePerGames = new TreeMap<>();
    private SortedMap<Integer, List<Integer>> requestMazeSizePerGame = new TreeMap<>();

    /**
     * To launch the game with flag in argument which initialize the datagram socket
     * at the first time where functions was called.
     * @param flag an option
     */
    public void launch(int flag) throws IOException {
        addContentTologs("client: info:", "connection to server established.",1);
        gameRegistered = -1;
        gameModel = new GameModel();
        socket = new Socket(ipv4_addr, tcpPort);
        sender = new Sender(this, socket.getOutputStream());
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
    }

    /**
     * When the client receives the message [Welco...], he starts the multicast cast
     * of the registered game.
     * @param multicastAddr the multicast address was given by the server.
     * @param multicastPort the multicast port was given by the server.
     */
    public void launchMulticastThread(String multicastAddr, int multicastPort){
        multicastRunnable = new MulticastRunnable(this, multicastAddr, multicastPort);
        Thread multicastThread = new Thread(multicastRunnable);
        multicastThread.start();
    }

    /**
     * @return Return the client socket.
     */
    public Socket getSocket() {
        return socket;
    }

    /**
     * @return Return the udp port of the client.
     */
    public int getUdpPort() {
        return datagramSocket.getLocalPort();
    }

    /**
     * @return Return the object which 
     * is used to send message to the server.
     */
    public Sender getSender(){
        return sender;
    }

    /**
     * @return Return the username.
     */
    public String getUsername() {
        return username;
    }

    /**
     * @return Game id where the is registered.
     */
    public int getGameRegister(){
        return gameRegistered;
    }

    /**
     * @return Return the game model.
     */
    public GameModel getGameModel(){
        return gameModel;
    }

    /**
     * @return Return the list of games id.
     */
    public List<Integer> getRequestGamesId(){
        return requestGamesId;
    }

    /**
     * @return Return the list of players per games.
     */
    public SortedMap<Integer, List<String>> getRequestPlayersUsernamePerGames(){
        return requestPlayersUsernamePerGames;
    }

    /**
     * @return Return the list of maze size per games.
     */
    public SortedMap<Integer, List<Integer>> getRequestMazeSizePerGame(){
        return requestMazeSizePerGame;
    }

    /**
     * Set the ipv4 address for the socket TCP.
     * @param ipv4_addr An ipv4 address.
     */
    public void setIPv4Addr(String ipv4_addr) {
        this.ipv4_addr = ipv4_addr;
    }

    /**
     * Set the ipv4 address for the socket TCP.
     * @param ipv4_addr An ipv4 address.
     */
    public void settcpPort(int tcpPort) {
        this.tcpPort = tcpPort;
    }

    /**
     * Set the username.
     * @param username An username.
     */
    public void setUsername(String username) {
        this.username = username;
    }

    /**
     * Set the udp port.
     * @param udpPort An udp port.
     */
    public void setudpPort(int udpPort) {
        this.udpPort = udpPort;
    }

    /**
     * Set the game id where the player is registered.
     * @param gameRegister A game id.
     */
    public void setGameRegister(int gameRegister) {
        this.gameRegistered = gameRegister;
    }

    /**
     * Add game id in the liste of games id.
     * @param gameId A game id.
     */
    public void addInRequestGamesId(int gameId){
        requestGamesId.add(gameId);
    }

    /**
     * Add an username in a specific game.
     * @param gameId A game id.
     * @param username An username.
     */
    public void addInRequestPlayersUsernamePerGames(int gameId, String username){
        if(!requestPlayersUsernamePerGames.containsKey(gameId))
            requestPlayersUsernamePerGames.put(gameId, new ArrayList<>());
        requestPlayersUsernamePerGames.get(gameId).add(username);
    }

    /**
     * Add dimension of maze in a specific game.
     * @param gameId A game id.
     * @param mazeHeight Height of maze.
     * @param mazeWidth Width of maze.
     */
    public void addInRequestMazeSizePerGame(int gameId, int mazeHeight, int mazeWidth){
        requestMazeSizePerGame.put(gameId, new ArrayList<>(List.of(mazeHeight, mazeWidth)));
    }

    /**
     * Clear date of games (in client side).
     */
    public void clearData(){
        getRequestGamesId().clear();
        getRequestPlayersUsernamePerGames().clear();
        getRequestMazeSizePerGame().clear();
    }

    /**
     * Allow to restart the client.
     */
    public void resetClient() throws IOException{
        socket.close();
        tcpRunnable.wantExit();
        setCurrentScene(SceneData.SCENE_MAIN);
        launch(1);
        sender.sendServerInfo();
        setUsername(null);
    }
    
    private List<Integer> scene = new ArrayList<>();
    private final List<String> logs = new ArrayList<>();
    private final List<String> serverAnswers = new ArrayList<>();

    private ImBoolean consoleShowInfo = new ImBoolean(true);
    private ImBoolean consoleShowWarning = new ImBoolean(true);
    private ImBoolean consoleShowError = new ImBoolean(true);
    private ImBoolean consoleShouldScroll = new ImBoolean();
    private ImBoolean blockClientRequest = new ImBoolean();
    private ImString consoleCommand = new ImString();
    private ImString usernameChoiceContent = new ImString(200);
    private ImString gameChoiceContent = new ImString(200);
    private int lastPressedButton = -1;
    
    /**
     * @return Get the console info option.
     */
    public ImBoolean getConsoleShowInfo(){
        return consoleShowInfo;
    }

    /**
     * @return Get the console warning option.
     */
    public ImBoolean getConsoleShowWarning(){
        return consoleShowWarning;
    }
    
    /**
     * @return Get the console error option.
     */
    public ImBoolean getConsoleShowError(){
        return consoleShowError;
    }

    /**
     * @return Get the auto scroll option.
     */
    public ImBoolean getConsoleShouldScroll(){
        return consoleShouldScroll;
    }

    /**
     * @return Get the block client request option.
     */
    public ImBoolean getBlockClientRequest(){
        return blockClientRequest;
    }

    /**
     * @return Get the console command input.
     */
    public ImString getConsoleCommand(){
        return consoleCommand;
    }

    /**
     * @return Get the user choice input.
     */
    public ImString getUsernameChoiceContent(){
        return usernameChoiceContent;
    }

    /**
     * @return Get the user choice input.
     */
    public ImString getGameChoiceContent(){
        return gameChoiceContent;
    }

    /**
     * @return Get last pressed button.
     */
    public int getLastPressedButton(){
        return lastPressedButton;
    }

    /**
     * Set last pressed button.
     * @param button Button.
     */
    public void setLastPressedButton(int button){
        lastPressedButton = button;
    }

    /**
     * @return Get logs.
     */
    public List<String> getLogs(){
        return logs;
    }

    /**
     * @return Get last server answer.
     */
    public String getLastServerAnswer(){
        return serverAnswers.get(serverAnswers.size() - 1);
    }

    /**
     * Add content to logs
     * @param separator Specifie the message type.
     * @param content Message.
     * @param flag 0 to add player command into the log.
     */
    public void addContentTologs(String separator, String content, int flag){
        consoleShouldScroll.set(true);
        if(flag == 0){
            serverAnswers.add(content);
        }
        content = separator + " " + content;
        logs.add(content);
    }

    /**
     * To back to the previous scene.
     */
    public void backToPreviousScene(){
        scene.remove(scene.size()-1);
    }
    
    /**
     * @return The current scene.
     */
    public int getCurrentScene(){
        return scene.get(scene.size()-1);
    }
    
    /**
     * Set current scene.
     * @param scene A scene.
     */
    public void setCurrentScene(int scene){
        this.scene.add(scene);
    }

    /**
     * @return Get visible log.
     */
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
    
    /**
     * Add help command in logs.
     */
    public void helpcommand(){
        addContentTologs("client: info:", "commands:", 1);
        addContentTologs("client: info:", "\tq, e, quit, exit   terminates the program.", 1);
        addContentTologs("client: info:", "\th, help            displays this help message.", 1);
        addContentTologs("client: info:", "\tv, version         display the program's version.", 1);

    }

    /**
     * Initialize display of the game.
     */
    @Override
    protected void initialize() {
        addContentTologs("client: info:", "you can now enter commands.",1);
        helpcommand();
        scene.add(SceneData.SCENE_MAIN);
    }

    /**
     * Allow to update the display.
     */
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

    /**
     * To destroy the user interface.
     */
    @Override
    protected void destroy() {
        exit(0);
    }
}
