package com.ustudents.ghostlab.communication;

import java.io.BufferedReader;
import java.io.IOException;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.listener.TCPRunnable;
import com.ustudents.ghostlab.other.Utils;
import com.ustudents.ghostlab.scene.SceneData;

public class Reader {

    private final Client client;
    private final TCPRunnable tcpRunnable;

    public Reader(Client client, TCPRunnable tcpRunnable){
        this.client = client;
        this.tcpRunnable = tcpRunnable;
    }

    public void readThreeEndSeparator(BufferedReader br) throws IOException{
        for(int i = 0; i < 3; i++){
            br.read();
        }
    }

    private void readGAMES(BufferedReader br) throws IOException{
        int nbGame = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:" ,
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
        client.addContentTologs("client: received from server:",
         "OGAME " + gameId + " " + nbPlayer + "***", 0);
    
    }
    
    private void readREGOK(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
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
        client.addContentTologs("client: received from server:",
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
        client.addContentTologs("client: received from server:",
         "SIZE! " + gameId + " " + mazeHeight + " " + mazeWidth + "***", 0);
        client.backToPreviousScene(); 
    }
    
    private void readLIST(BufferedReader br) throws IOException{
        int gameId = Utils.readOctets(br, 1);
        int nbPlayer = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:" ,
         "LIST! " + gameId + " " + nbPlayer + "***", 0);

        for(int i = 0; i < nbPlayer; i++){
            read(br);
        }
        client.backToPreviousScene();
    }

    private void readPLAYR(BufferedReader br) throws IOException{
        String username = Utils.readOctetToMakeString(br, 8);
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
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
        client.addContentTologs("client: received from server:",
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
        client.addContentTologs("client: received from server:",
         "POSIT " + username + " " + posX + " " + posY + 
         "***", 0);
        client.setCurrentScene(SceneData.SCENE_INGAME);
    }
    
    private void readMOVE(BufferedReader br, int flag) throws IOException{
        String posX = Utils.readOctetToMakeString(br, 3);
        String posY = Utils.readOctetToMakeString(br, 3);
        if(flag == 0){
            client.addContentTologs("client: received from server:",
            "MOVE! " + posX + " " + posY + "***", 0);
        }else{
            String playerScore = Utils.readOctetToMakeString(br, 4);
            client.addContentTologs("client: received from server:",
            "MOVEF " + posX + " " + posY + " " + playerScore +"***", 0);
        }
        readThreeEndSeparator(br);
        client.getGameModel().updateMaze(new int[]{Integer.parseInt(posX), Integer.parseInt(posY)});
        client.setCurrentScene(SceneData.SCENE_INGAME);

    }

    private void readGOBYE(BufferedReader br) throws IOException{
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
            "GOBYE***", 0);
        client.getSocket().close();
        client.launch(1);
        client.setCurrentScene(SceneData.SCENE_MAIN);
        tcpRunnable.wantExit();
    }
    
    private void readGLIST(BufferedReader br) throws IOException{
        int nbPlayer = Utils.readOctets(br, 1);
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:" ,
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
        client.addContentTologs("client: received from server:",
         "GPLYR " + username + " " + posX + " " + posY + " " +
         playerScore +  "***", 0);
    
    }

    public void read(BufferedReader br) throws IOException {
        String read = "";
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
        }else if(read.equals("MALL!")){
            
        }else if(read.equals("SEND!")){
            
        }else if(read.length() > 0){
            client.addContentTologs("client: error:", "unknow message", 1);
        }
    }
}
