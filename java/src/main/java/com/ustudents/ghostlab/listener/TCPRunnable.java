package com.ustudents.ghostlab.listener;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.communication.Reader;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class TCPRunnable implements Runnable{

    private final Client client;
    //private final BufferedReader br;
    private final InputStream inputStream;
    private volatile boolean exit;


    public TCPRunnable(Client client) throws IOException{
        this.client = client;
        //this.br = new BufferedReader(new InputStreamReader(client.getSocket().getInputStream()));
        this.inputStream = client.getSocket().getInputStream();
        exit = false;
    }

    public void wantExit(){
        this.exit = true;
    }

    @Override
    public void run() {
        Reader reader = new Reader(client, this);
        try {
            while(!exit && !client.getSocket().isClosed()){
                //reader.read(br);
                reader.read(inputStream);
            }
            
        } catch (IOException e) {
            client.addContentTologs("warning:", "Exception", 1);
            e.printStackTrace();
        }
    }
}
