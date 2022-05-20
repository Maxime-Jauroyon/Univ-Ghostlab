package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.InputStream;
import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.scene.SceneData;

public class Reader {

    private final Client client;
    private int currentGameID;

    public Reader(Client client){
        this.client = client;
    }
    
    /**
     * Read server answer about games request.
     * @param inputStream InputStream.
     */
    private void readGAMES(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int nbGames = bytes[1];
        
        client.addContentTologs("client: info: received from server:", 
        "GAMES " + nbGames + "***", 0);
    }

    /**
     * Read server answer about games request.
     * @param inputStream InputStream.
     */
    private void readOGAMES(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[7];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int nbPlayer = bytes[3];

        client.addContentTologs("client: info: received from server:",
         "OGAME " + gameId + " " + nbPlayer + "***", 0);
        client.addInRequestGamesId(gameId);
        client.getSender().send("LIST? " + gameId  + "***");
        client.getSender().send("SIZE? " + gameId  + "***");
    }

    /**
     * Read server answer about register request.
     * @param inputStream InputStream.
     */
    private void readREGOK(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int gameId = bytes[1];

        client.addContentTologs("client: info: received from server:",
         "REGOK " + gameId + "***", 0);
        client.setGameRegister(gameId);
        client.getSender().sendServerInfo();
        client.setCurrentScene(SceneData.SCENE_GAMELOBBY);
    }
    
    /**
     * Read server answer about register request.
     * @param inputStream InputStream.
     */
    private void readREGNO(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: error: received from server:",
         "REGNO***", 0);
        client.setGameRegister(-1);
        client.getSender().sendServerInfo();
        client.setUsername(null);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        
    }

    /**
     * Read server answer about register request.
     * @param inputStream InputStream.
     */
    private void readUNROK(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int gameId = bytes[1];

        client.addContentTologs("client: info: received from server:",
         "UNROK " + gameId + "***", 0);
        client.resetClient();
    }

    /**
     * Read server answer about request.
     * @param inputStream InputStream.
     */
    private void readDUNNO(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: error: received from server:",
         "DUNNO***", 0);
    }

    /**
     * Read server answer about size request.
     * @param inputStream InputStream.
     */
    private void readSIZE(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[11];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int mazeHeight = bytes[3];
        int mazeWidth = bytes[6];
        
        client.addContentTologs("client: info: received from server:",
         "SIZE! " + gameId + " " + mazeHeight + " " + mazeWidth + "***", 0);
        client.addInRequestMazeSizePerGame(gameId, mazeHeight, mazeWidth); 
    }
    
    /**
     * Read server answer about list request.
     * @param inputStream InputStream.
     */
    private void readLIST(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[7];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int nbPlayer = bytes[3];

        client.addContentTologs("client: info: received from server:" ,
         "LIST! " + gameId + " " + nbPlayer + "***", 0);
        currentGameID = gameId;
    }

    /**
     * Read server answer about list request.
     * @param inputStream InputStream.
     */
    private void readPLAYR(InputStream inputStream, int gameId) throws IOException{
        byte[] bytes = new byte[12];
        int readBytes = inputStream.read(bytes);
        String username = new String(bytes, 1, readBytes-4);

        client.addContentTologs("client: info: received from server:",
         "PLAYR " + username + "***", 0);
        if(gameId == client.getGameRegister() && username.equals(client.getUsername()))
            username += " (you)";
        client.addInRequestPlayersUsernamePerGames(gameId, username); 
    
    }

    /**
     * Convert byte array to string.
     * @param bytes Byte array.
     * @param begin Start index.
     * @param end End index.
     * @return String extract from the byte array.
     */
    private String convertByteArrayToString(byte[] bytes, int begin, int end){
        String res = "";
        for(int i = begin; i < end; i++){
            res += (char) bytes[i];
        }
        return res;
    }

    /**
     * Read server answer about start request.
     * @param inputStream InputStream.
     */
    private void readWELCO(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[34];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int mazeHeight = bytes[3];
        int mazeWidth = bytes[6];
        int ghost = bytes[9];
        String multicastAddr = convertByteArrayToString(bytes, 11, 26);
        client.addContentTologs("$", multicastAddr, 1);
        String multicastPort = convertByteArrayToString(bytes, 27, 31);
        client.addContentTologs("$", multicastPort, 1);

        client.launchMulticastThread(multicastAddr.split("#")[0], Integer.parseInt(multicastPort));
        client.getGameModel().setMaze(mazeHeight, mazeWidth);
        client.addContentTologs("client: info: received from server:",
         "WELCO " + gameId + " " + mazeHeight + " " + mazeWidth + 
         " " + ghost + " " + multicastAddr + " " + multicastPort + 
         "***", 0);

    }
    
    /**
     * Read server answer about start request.
     * @param inputStream InputStream.
     */
    private void readPOSIT(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[20];
        inputStream.read(bytes);
        String username = convertByteArrayToString(bytes, 1, 9);
        String posX = convertByteArrayToString(bytes, 10, 13);
        String posY = convertByteArrayToString(bytes, 14, 17);

        client.getGameModel().setNewPos(Integer.parseInt(posX), Integer.parseInt(posY));
        client.addContentTologs("client: info: received from server:",
         "POSIT " + username + " " + posX + " " + posY + 
         "***", 0);
        client.getBlockClientRequest().set(false);
        client.setCurrentScene(SceneData.SCENE_INGAME);
        client.getSender().send("GLIS?***");
    }

    /**
     * Read server answer about move request.
     * @param inputStream InputStream.
     */
    private void readMOVE(InputStream inputStream, int flag) throws IOException{
        byte[] bytes = new byte[16];
        inputStream.read(bytes);
        String posX = convertByteArrayToString(bytes, 1, 4);
        String posY = convertByteArrayToString(bytes, 5, 8);

        if(flag == 0){
            client.addContentTologs("client: info: received from server:",
            "MOVE! " + posX + " " + posY + "***", 0);
        }else{
            String playerScore = convertByteArrayToString(bytes, 9, 13);
            client.addContentTologs("client: info: received from server:",
            "MOVEF " + posX + " " + posY + " " + playerScore +"***", 0);
        }
        
        client.getGameModel().updateMaze(new int[]{Integer.parseInt(posX), Integer.parseInt(posY)});
        client.setCurrentScene(SceneData.SCENE_INGAME);

    }

    /**
     * Read server answer about glist request.
     * @param inputStream InputStream.
     */
    private void readGLIST(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int nbPlayer = bytes[1];
        
        client.getGameModel().players().clear();
        client.addContentTologs("client: info: received from server:" ,
         "GLIS! " + nbPlayer + "***", 0);

    }

    /**
     * Read server answer about glist request.
     * @param inputStream InputStream.
     */
    private void readGPLYR(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[25];
        inputStream.read(bytes);
        String username = convertByteArrayToString(bytes, 1, 9);
        String posX = convertByteArrayToString(bytes, 10, 13);
        String posY = convertByteArrayToString(bytes, 14, 17);
        String score = convertByteArrayToString(bytes, 18, 22);

        client.addContentTologs("client: info: received from server:",
         "GPLYR " + username + " " + posX + " " + posY + " " +
         score +  "***", 0);
         if(username.equals(client.getUsername()))
            username += " (you)";
         client.getGameModel().addInPlayers(username, posX, posY, score);
    
    }

    /**
     * Read server answer about mess request.
     * @param inputStream InputStream.
     */
    private void readMESS(InputStream inputStream, int flag) throws IOException{
        byte[] bytes = new byte[8];
        inputStream.read(bytes);
        if(flag == 0){
            client.addContentTologs("client: info: received from server:",
            "SEND!***", 0);
        }else if(flag == 1){
            client.addContentTologs("client: warning: received from server:",
            "NSEND***", 0);
        }else{
            client.addContentTologs("client: info: received from server:",
            "MALL!***", 0);
            
        }

        if(flag != 1)
            client.backToPreviousScene();
    }

    /**
     * Read server answer about godbye request.
     * @param inputStream InputStream.
     */
    private void readGOBYE(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: info: received from server:",
            "GOBYE***", 0);
        client.resetClient();
        
    }

    /**
     * Read server answer.
     * @param inputStream InputStream.
     */
    public void read(InputStream inputStream) throws IOException {
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        String read = new String(bytes);
        client.addContentTologs("$", read, 1);

        if(read.equals("GAMES")){
            readGAMES(inputStream);
        }else if(read.equals("OGAME")){
            readOGAMES(inputStream);
        }else if(read.equals("REGOK")){
            readREGOK(inputStream);
        }else if(read.equals("REGNO")){
            readREGNO(inputStream);
        }else if(read.equals("UNROK")){
            readUNROK(inputStream);
        }else if(read.equals("DUNNO")){
            readDUNNO(inputStream);
        }else if(read.equals("SIZE!")){
            readSIZE(inputStream);
        }else if(read.equals("LIST!")){
            readLIST(inputStream);
        }else if(read.equals("PLAYR")){
            readPLAYR(inputStream, currentGameID);
        }else if(read.equals("WELCO")){
            readWELCO(inputStream);
        }else if(read.equals("POSIT")){
            readPOSIT(inputStream);
        }else if(read.equals("MOVE!")){
            readMOVE(inputStream, 0);
        }else if(read.equals("MOVEF")){
            readMOVE(inputStream, 1);
        }else if(read.equals("GOBYE")){
            readGOBYE(inputStream);
        }else if(read.equals("GLIS!")){
            readGLIST(inputStream);
        }else if(read.equals("GPLYR")){
            readGPLYR(inputStream);
        }else if(read.equals("SEND!")){
            readMESS(inputStream, 0);
        }else if(read.equals("NSEND")){
            readMESS(inputStream, 1);
        }else if(read.equals("MALL!")){
            readMESS(inputStream, 2);    
        }else if(read.length() > 0){
            client.addContentTologs("client: error:", "unknow message", 1);
        }

        client.getUsernameChoiceContent().clear();
        client.getConsoleCommand().clear();
        client.getGameChoiceContent().clear();
    }
}
