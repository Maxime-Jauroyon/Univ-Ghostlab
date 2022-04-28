package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.InputStream;
import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.listener.TCPRunnable;
import com.ustudents.ghostlab.scene.SceneData;

public class Reader {

    private final Client client;
    private final TCPRunnable tcpRunnable;

    public Reader(Client client, TCPRunnable tcpRunnable){
        this.client = client;
        this.tcpRunnable = tcpRunnable;
    }

    /*public void readThreeEndSeparator(BufferedReader br) throws IOException{
        for(int i = 0; i < 3; i++){
            br.read();
        }
    }

    private void readGAMES(BufferedReader br) throws IOException{
        int nbGame = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:" ,
         "GAMES " + nbGame + "***", 0);

        for(int i = 0; i < nbGame; i++){
            read(br);
        }
    }

    private void readOGAMES(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        int nbPlayer = br.read();
        nbPlayer = br.read();
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "OGAME " + gameId + " " + nbPlayer + "***", 0);
    
    }
    
    private void readREGOK(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "REGOK " + gameId + "***", 0);
        client.setCurrentScene(SceneData.SCENE_GAMELOBBY);
        
    }
    
    private void readREGNO(BufferedReader br) throws IOException{
        readThreeEndSeparator(br);
        client.addContentTologs("client: error: received from server:",
         "REGNO***", 0);
        client.setUsername(null);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        
    }
    
    private void readUNROK(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "UNROK " + gameId + "***", 0);
        client.getSocket().close();
        client.launch(1);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        tcpRunnable.wantExit();
        
    }

    private void readDUNNO(BufferedReader br) throws IOException{
        readThreeEndSeparator(br);
        client.addContentTologs("client: error: received from server:",
         "DUNNO***", 0);
    }
    
    private void readSIZE(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        int mazeHeight = Utils.readOctets(br, 2);
        int mazeWidth = Utils.readOctets(br, 2);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "SIZE! " + gameId + " " + mazeHeight + " " + mazeWidth + "***", 0);
        client.backToPreviousScene(); 
    }
    
    private void readLIST(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        int nbPlayer = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:" ,
         "LIST! " + gameId + " " + nbPlayer + "***", 0);

        for(int i = 0; i < nbPlayer; i++){
            read(br);
        }
        client.backToPreviousScene();
    }

    private void readPLAYR(BufferedReader br) throws IOException{
        String username = Utils.readOctetToMakeString(br, 8);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "PLAYR " + username + "***", 0);
    
    }

    private void readWELCO(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        int mazeHeight = Utils.readOctets(br, 2);
        int mazeWidth = Utils.readOctets(br, 2);
        int ghost = Utils.readOctets(br, 1);
        String ipMulticast = Utils.readOctetToMakeString(br, 15);
        String portMulticast = Utils.readOctetToMakeString(br, 4);
        readThreeEndSeparator(br);
        client.getGameModel().setMaze(mazeHeight, mazeWidth);
        client.addContentTologs("client: info: received from server:",
         "WELCO " + gameId + " " + mazeHeight + " " + mazeWidth + 
         " " + ghost + " " + ipMulticast + " " + portMulticast + 
         "***", 0);

    }
    
    private void readPOSIT(BufferedReader br) throws IOException{
        String username = Utils.readOctetToMakeString(br, 8);
        String posX = Utils.readOctetToMakeString(br, 3);
        String posY = Utils.readOctetToMakeString(br, 3);
        readThreeEndSeparator(br);
        client.getGameModel().setNewPos(Integer.parseInt(posX), Integer.parseInt(posY));
        client.addContentTologs("client: info: received from server:",
         "POSIT " + username + " " + posX + " " + posY + 
         "***", 0);
        client.setCurrentScene(SceneData.SCENE_INGAME);
    }
    
    private void readMOVE(BufferedReader br, int flag) throws IOException{
        String posX = Utils.readOctetToMakeString(br, 3);
        String posY = Utils.readOctetToMakeString(br, 3);
        if(flag == 0){
            client.addContentTologs("client: info: received from server:",
            "MOVE! " + posX + " " + posY + "***", 0);
        }else{
            String playerScore = Utils.readOctetToMakeString(br, 4);
            client.addContentTologs("client: info: received from server:",
            "MOVEF " + posX + " " + posY + " " + playerScore +"***", 0);
        }
        readThreeEndSeparator(br);
        client.getGameModel().updateMaze(new int[]{Integer.parseInt(posX), Integer.parseInt(posY)});
        client.setCurrentScene(SceneData.SCENE_INGAME);

    }

    private void readGOBYE(BufferedReader br) throws IOException{
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
            "GOBYE***", 0);
        client.getSocket().close();
        client.launch(1);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        tcpRunnable.wantExit();
    }
    
    private void readGLIST(BufferedReader br) throws IOException{
        int nbPlayer = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:" ,
         "GLIS! " + nbPlayer + "***", 0);

        for(int i = 0; i < nbPlayer; i++){
            read(br);
        }
    }

    private void readGPLYR(BufferedReader br) throws IOException{
        String username = Utils.readOctetToMakeString(br, 8);
        String posX = Utils.readOctetToMakeString(br, 3);
        String posY = Utils.readOctetToMakeString(br, 3);
        String playerScore = Utils.readOctetToMakeString(br, 4);
        readThreeEndSeparator(br);
        client.addContentTologs("client: info: received from server:",
         "GPLYR " + username + " " + posX + " " + posY + " " +
         playerScore +  "***", 0);
    
    }

    private void readMESS(BufferedReader br, int flag) throws IOException{
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
        readThreeEndSeparator(br);
        client.backToPreviousScene();
    }*/

    /*private byte[] cleanRead(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[218];
        int readChar = inputStream.read(bytes);
        byte[] cleanBytes = new byte[readChar];
        for(int i = 0; i < cleanBytes.length; i++){
            cleanBytes[i] = bytes[i];
        }
        return cleanBytes;
    }*/

    /*public void readThreeEndSeparator(InputStream inputStream) throws IOException{
        for(int i = 0; i < 3; i++){
            inputStream.read();
        }
    }*/

    /*private int convertByteToInt(byte[] bytes, int begin, int end){
        ByteBuffer byteBuffer = ByteBuffer.wrap(bytes, begin, end);
        return (int)byteBuffer.get();
    }*/
 
    private void readGAMES(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int nbGames = bytes[1];
        
        client.addContentTologs("client: info: received from server:", 
        "GAMES " + nbGames + "***", 0);
    }

    private void readOGAMES(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[7];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int nbPlayer = bytes[3];

        client.addContentTologs("client: info: received from server:",
         "OGAME " + gameId + " " + nbPlayer + "***", 0);
    
    }

    private void readREGOK(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int gameId = bytes[1];

        client.addContentTologs("client: info: received from server:",
         "REGOK " + gameId + "***", 0);
        client.setCurrentScene(SceneData.SCENE_GAMELOBBY);
        
    }
    
    private void readREGNO(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: error: received from server:",
         "REGNO***", 0);
        client.setUsername(null);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        
    }

    private void readUNROK(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        int gameId = bytes[1];

        client.addContentTologs("client: info: received from server:",
         "UNROK " + gameId + "***", 0);
        client.getSocket().close();
        client.launch(1);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        tcpRunnable.wantExit();
        
    }

    private void readDUNNO(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: error: received from server:",
         "DUNNO***", 0);
    }

    private void readSIZE(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[11];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int mazeHeight = bytes[3];
        int mazeWidth = bytes[6];
        
        client.addContentTologs("client: info: received from server:",
         "SIZE! " + gameId + " " + mazeHeight + " " + mazeWidth + "***", 0);
        client.backToPreviousScene(); 
    }
    
    private void readLIST(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[7];
        inputStream.read(bytes);
        int gameId = bytes[1];
        int nbPlayer = bytes[3];

        client.addContentTologs("client: info: received from server:" ,
         "LIST! " + gameId + " " + nbPlayer + "***", 0);
        client.backToPreviousScene();
    }

    private void readPLAYR(InputStream inputStream) throws IOException{
        byte[] bytes = new byte[12];
        int readBytes = inputStream.read(bytes);
        String username = new String(bytes, 1, readBytes-3);

        client.addContentTologs("client: info: received from server:",
         "PLAYR " + username + "***", 0);
    
    }

    private String convertByteArrayToString(byte[] bytes, int begin, int end){
        String res = "";
        for(int i = begin; i < end; i++){
            res += (char) bytes[i];
        }
        return res;
    }

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

        client.launchMulticastThread(multicastAddr, Integer.parseInt(multicastPort));
        client.getGameModel().setMaze(mazeHeight, mazeWidth);
        client.addContentTologs("client: info: received from server:",
         "WELCO " + gameId + " " + mazeHeight + " " + mazeWidth + 
         " " + ghost + " " + multicastAddr + " " + multicastPort + 
         "***", 0);

    }
    
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
        client.setCurrentScene(SceneData.SCENE_INGAME);
    }

    private void readMOVE(InputStream inputStream, int flag) throws IOException{
        byte[] bytes = new byte[11];
        inputStream.read(bytes);
        String posX = convertByteArrayToString(bytes, 1, 4);
        String posY = convertByteArrayToString(bytes, 5, 8);

        if(flag == 0){
            client.addContentTologs("client: info: received from server:",
            "MOVE! " + posX + " " + posY + "***", 0);
        }else{
            String playerScore = convertByteArrayToString(bytes, 9, 12);
            client.addContentTologs("client: info: received from server:",
            "MOVEF " + posX + " " + posY + " " + playerScore +"***", 0);
        }
        
        client.getGameModel().updateMaze(new int[]{Integer.parseInt(posX), Integer.parseInt(posY)});
        client.setCurrentScene(SceneData.SCENE_INGAME);

    }

    private void readGOBYE(InputStream inputStream) throws IOException{
        inputStream.read(new byte[3]);
        client.addContentTologs("client: info: received from server:",
            "GOBYE***", 0);
        client.getSocket().close();
        client.launch(1);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        tcpRunnable.wantExit();
    }

    public void read(InputStream inputStream) throws IOException {
        byte[] bytes = new byte[5];
        inputStream.read(bytes);
        String read = new String(bytes);
        //client.addContentTologs("client: info: received from server:", read, 0);

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
            readPLAYR(inputStream);
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
            
        }else if(read.equals("GPLYR")){
            
        }else if(read.equals("SEND!")){
            
        }else if(read.equals("NSEND")){
            
        }else if(read.equals("MALL!")){
                
        }else if(read.length() > 0){
            client.addContentTologs("client: error:", "unknow message", 1);
        }

        client.getUsernameChoiceContent().clear();
        client.getConsoleCommand().clear();
        client.getGameChoiceContent().clear();

        /*String read = "";
        for(int i = 0; i < 5; i++){
            read += (char) br.read();
        }
        
        client.addContentTologs("$", read, 1);

        if(read.equals("GAMES")){
            readGAMES(br);
        }else if(read.equals("OGAME")){
            readOGAMES(br);
        }else if(read.equals("REGOK")){
            readREGOK(br);
        }else if(read.equals("REGNO")){
            readREGNO(br);
        }else if(read.equals("UNROK")){
            readUNROK(br);
        }else if(read.equals("DUNNO")){
            readDUNNO(br);
        }else if(read.equals("SIZE!")){
            readSIZE(br);
        }else if(read.equals("LIST!")){
            readLIST(br);
        }else if(read.equals("PLAYR")){
            readPLAYR(br);
        }else if(read.equals("WELCO")){
            readWELCO(br);
        }else if(read.equals("POSIT")){
            readPOSIT(br);
        }else if(read.equals("MOVE!")){
            readMOVE(br, 0);
        }else if(read.equals("MOVEF")){
            readMOVE(br, 1);
        }else if(read.equals("GOBYE")){
            readGOBYE(br);
        }else if(read.equals("GLIS!")){
            readGLIST(br);
        }else if(read.equals("GPLYR")){
            readGPLYR(br);
        }else if(read.equals("SEND!")){
            readMESS(br, 0);
        }else if(read.equals("NSEND")){
            readMESS(br, 1);
        }else if(read.equals("MALL!")){
            readMESS(br, 2);    
        }else if(read.length() > 0){
            client.addContentTologs("client: error:", "unknow message", 1);
        }

        client.getUsernameChoiceContent().clear();
        client.getConsoleCommand().clear();
        client.getGameChoiceContent().clear();*/
    }
}
