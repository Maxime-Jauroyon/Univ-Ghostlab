package com.ustudents.ghostlab.communication;

import java.io.BufferedReader;
import java.io.IOException;

import com.ustudents.ghostlab.client.Client;

public class Reader {

    private final Client client;

    public Reader(Client client){
        this.client = client;
    }

    private void readGAMES(BufferedReader br) throws IOException{
        int nbGame = br.read();
        nbGame = br.read();
        client.addContentTologs("client: received from server:" ,
         "GAMES " + nbGame + "***");

        for(int i = 0; i < nbGame; i++){
            read(br);
        }
    }

    private void readOGAMES(BufferedReader br) throws IOException{
        int idGame = br.read();
        idGame = br.read();
        int nbPlayer = br.read();
        idGame = br.read();
        client.addContentTologs("client: received from server:",
         "OGAME " + idGame + " " + nbPlayer + "***");
    
    }   

    public void read(BufferedReader br) throws IOException {
        String read = "";
        for(int i = 0; i < 5; i++){
            read += (char) br.read();
        }
        
        client.addContentTologs("$", read);

        if(read.equals("GAMES")){
            readGAMES(br);
        }else if(read.equals("OGAME")){
            readOGAMES(br);
        }else if(read.equals("REGOK")){

        }else if(read.equals("REGNO")){

        }else if(read.equals("UNROK")){

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
            
        }else{
            client.addContentTologs("client: error:", "unknow message");
        }
    }
}
