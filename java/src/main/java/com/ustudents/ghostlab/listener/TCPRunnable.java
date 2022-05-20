package com.ustudents.ghostlab.listener;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.communication.Reader;

import java.io.IOException;
import java.io.InputStream;

public class TCPRunnable implements Runnable{

    private final Client client;
    private final InputStream inputStream;
    private volatile boolean exit;


    public TCPRunnable(Client client) throws IOException{
        this.client = client;
        this.inputStream = client.getSocket().getInputStream();
        exit = false;
    }

    /**
     * Allow to stop the thread.
     */
    public void wantExit(){
        this.exit = true;
    }

    /**Method run of TCPRunnable. */
    @Override
    public void run() {
        Reader reader = new Reader(client);
        try {
            while(!exit && !client.getSocket().isClosed()){
                reader.read(inputStream);
            }
            
        } catch (IOException e) {
            client.addContentTologs("warning:", "Exception", 1);
            e.printStackTrace();
        }
    }
}
