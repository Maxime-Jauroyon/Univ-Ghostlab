package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.OutputStream;

import com.ustudents.ghostlab.client.Client;

public class Sender {
    private final Client client;
    private final OutputStream outputStream;

    public Sender(Client client, OutputStream outputStream){
        this.outputStream = outputStream;
        this.client = client;
    }

    /**
     * Send request to the server.
     * @param request Request.
     */
    public void sendString(String request) throws IOException{
        outputStream.write(request.getBytes());
        outputStream.flush();

    }

    /**
     * Send register request to the server.
     * @param request Request.
     */
    public void sendREGIS(String[] request) throws IOException{
        outputStream.write(("REGIS " + request[1] + " " + request[2] + " ").getBytes());
        outputStream.write((byte) client.getGameRegister());
        outputStream.write("***".getBytes());
        outputStream.flush();
    } 
    
    /**
     * Send size and list request to the server.
     * @param request Request.
     */
    public void sendSIZEAndLIST(String[] request) throws IOException{
        request[0] += " ";
        outputStream.write(request[0].getBytes());
        outputStream.write((byte) Integer.parseInt(request[1].substring(0, request[1].length()-3)));
        outputStream.write("***".getBytes());
        outputStream.flush();
    }

    /**
     * Send request to the server.
     * @param request Request.
     */
    public void send(String request) throws IOException{
        client.addContentTologs("client: info: sent to the server:", request, 0);
        String[] list = request.split(" ");
        String requestName = list[0];
        if(requestName.contains("NEWPL") || requestName.contains("START") || 
           requestName.contains("UNREG") || requestName.contains("GAME?") || 
           requestName.contains("GLIS?") || requestName.contains("IQUIT") || 
           requestName.contains("MOV") || requestName.contains("MALL?") || 
           requestName.contains("SEND?")){
            sendString(request);
        }else if(requestName.contains("REGIS")){
            sendREGIS(list);
        }else if(requestName.contains("SIZE?") || requestName.contains("LIST?")){
            sendSIZEAndLIST(list);
        }
    }

    /**
     * Send request to the server to get games infos.
     * @param request Request.
     */
    public void sendServerInfo() throws IOException{
        client.clearData();
        send("GAME?***");
    }
    
    
    
}