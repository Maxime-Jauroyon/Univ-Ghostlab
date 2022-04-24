package com.ustudents.ghostlab.runnable;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.communication.Reader;
import com.ustudents.ghostlab.communication.Sender;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

public class TCPRunnable implements Runnable{

    private final Client client;
    private final BufferedReader br;
    private final PrintWriter pw;
    private String username;


    public TCPRunnable(Client client, String username) throws IOException{
        this.client = client;
        this.br = new BufferedReader(new InputStreamReader(client.getSocket().getInputStream()));
        this.pw = new PrintWriter(new OutputStreamWriter(client.getSocket().getOutputStream()));
        this.username = username;
    }

    public String getUsername(){
        return username;
    }

    public void setUsername(String username){
        this.username = username;
    }

    @Override
    public void run() {
        Reader reader = new Reader(client);
        try {
            reader.read(br);
        } catch (IOException e) {
            e.printStackTrace();
        }


        /*try{
            String question = "What to you want to do ? (move/list/messall/messto/quit)";
            InteractionInGamePhase iigp = new InteractionInGamePhase(client);
            iigp.putQuestionOnGamePhase(br, pw, question, new String[]{"move","list","messall","messto","quit"}, sc);

        }catch (Exception e){
            e.printStackTrace();
        }*/
    }
}
