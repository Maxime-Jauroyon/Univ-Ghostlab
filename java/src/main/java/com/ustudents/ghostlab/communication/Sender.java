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

    /*public byte[] convertListToArrayOfByte(List<Byte> byteList){
        byte[] byteArray = new byte[byteList.size()];
        for (int index = 0; index < byteList.size(); index++) {
            byteArray[index] = byteList.get(index);
        }
        return byteArray;
    }

    public List<Byte> fillListWithString(List<Byte> byteList, String content){
        byte[] byteArray = new byte[content.length()];
        for(int i = 0; i < byteArray.length; i++){
            byteList.add(byteArray[i]);
        }
        return byteList;
    }*/

    public void sendNEWPL(String request) throws IOException{
        outputStream.write(request.getBytes());
        outputStream.flush();

    }

    public void sendREGIS(String[] request) throws IOException{
        for(int i = 0; i < 3; i++){
            request[i] += " ";
            outputStream.write(request[i].getBytes());
        }
        outputStream.write((byte) Integer.parseInt(request[3].substring(0, request[3].length()-3)));
        outputStream.write("***".getBytes());
        outputStream.flush();
    } 
    
    public void sendSIZEAndLIST(String[] request) throws IOException{
        request[0] += " ";
        outputStream.write(request[0].getBytes());
        outputStream.write((byte) Integer.parseInt(request[1].substring(0, request[1].length()-3)));
        outputStream.write("***".getBytes());
    } 
    
    public void sendString(String[] request) throws IOException{
        for(int i = 0; i < 2; i++){
            request[i] += " ";
            outputStream.write(request[i].getBytes());
        }
        
        outputStream.write(request[1].substring(0, request[1].length()-3).getBytes());
    }

    public void send(String request) throws IOException{
        client.addContentTologs("client: info:", request, 0);
        String[] list = request.split(" ");
        String requestName = list[0];
        if(requestName.contains("NEWPL") || requestName.contains("START") ||
           requestName.contains("UNREG") || requestName.contains("GAME?") ||
           requestName.contains("GLIS?") || requestName.contains("IQUIT") ||
           requestName.contains("MOV") || requestName.contains("MALL?") || 
           requestName.contains("SEND?")){
            sendNEWPL(request);
        }else if(requestName.contains("REGIS")){
            sendREGIS(list);
        }else if(requestName.contains("SIZE?") || requestName.contains("LIST?")){
            sendSIZEAndLIST(list);
        }
    }

    public void sendServerInfo() throws IOException{
        client.getRequestGamesId().clear();
        client.getRequestPlayersUsernamePerGames().clear();
        client.getRequestMazeSizePerGame().clear();
        send("GAME?***");
        for(int gameId: client.getRequestGamesId())
            send("LIST? " + gameId  + "***");
    }
    
    
    
}