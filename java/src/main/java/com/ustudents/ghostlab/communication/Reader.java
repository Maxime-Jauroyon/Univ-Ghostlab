package com.ustudents.ghostlab.communication;

import java.io.BufferedReader;
import java.io.IOException;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.scene.SceneData;

public class Reader {

    private final Client client;

    public Reader(Client client){
        this.client = client;
    }

    public void readThreeEndSeparator(BufferedReader br) throws IOException{
        for(int i = 0; i < 3; i++){
            br.read();
        }
    }

    private void readGAMES(BufferedReader br) throws IOException{
        int nbGame = br.read();
        nbGame = br.read();
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:" ,
         "GAMES " + nbGame + "***", 0);

        for(int i = 0; i < nbGame; i++){
            read(br);
        }
    }

    private void readOGAMES(BufferedReader br) throws IOException{
        int idGame = br.read();
        idGame = br.read();
        int nbPlayer = br.read();
        idGame = br.read();
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
         "OGAME " + idGame + " " + nbPlayer + "***", 0);
    
    }
    
    private void readREGOK(BufferedReader br) throws IOException{
        int idGame = br.read();
        idGame = br.read();
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
         "REGOK " + idGame + "***", 0);
        client.setScene(SceneData.SCENE_GAMELOBBY);
        
    }
    
    private void readREGNO(BufferedReader br) throws IOException{
        readThreeEndSeparator(br);
        client.addContentTologs("client: error: received from server:",
         "REGNO***", 0);
        client.setUsername(null);
        client.setScene(SceneData.SCENE_MAIN);
        
    }
    
    private void readUNROK(BufferedReader br) throws IOException{
        int idGame = br.read();
        idGame = br.read();
        readThreeEndSeparator(br);
        client.addContentTologs("client: received from server:",
         "UNROK " + idGame + "***", 0);
        client.setGameRegister("-1"); 
        client.setScene(SceneData.SCENE_MAIN);
        
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
            
        }else if(read.equals("SIZE!")){
            
        }else if(read.equals("LIST!")){
            
        }else if(read.equals("WELCO")){
            
        }else if(read.equals("POSIT")){
            
        }else if(read.equals("MOVE")){
            
        }else if(read.equals("GOBYE")){
            
        }else if(read.equals("GLIS")){
            
        }else if(read.equals("MALL!")){
            
        }else if(read.equals("SEND!")){
            
        }else if(read.length() > 0){
            client.addContentTologs("client: error:", "unknow message", 1);
        }
    }
}
