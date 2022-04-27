package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

public class Sender {
    private final OutputStream outputStream;

    public Sender(OutputStream outputStream){
        this.outputStream = outputStream;
    }

    public byte[] convertListToArrayOfByte(List<Byte> byteList){
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
    }

    public void sendNEWPL(String request) throws IOException{
        /*byteList = fillListWithString(byteList, request[0] + " ");
        byteList = fillListWithString(byteList, request[1] + " ");
        byteList.add((byte)Integer.parseInt(request[2].substring(0, 4)));
        byteList = fillListWithString(byteList, "***");
        byte[] byteArray = convertListToArrayOfByte(byteList);
        outputStream.write(byteArray);
        outputStream.flush();*/

        /*for(int i = 0; i < request.length; i++){
            if(i != request.length-1){
                request[i] += " ";
                
            }

            outputStream.write(request[i].getBytes());
            
        }*/


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

    public void send(String request) throws IOException{
        String[] list = request.split(" ");
        String requestName = list[0];
        if(requestName.contains("NEWPL") || requestName.contains("START") ||
           requestName.contains("UNREG") || requestName.contains("GAME?") ||
           requestName.contains("GLIS?") || requestName.contains("IQUIT")){
            sendNEWPL(request);
        }else if(requestName.contains("REGIS")){
            sendREGIS(list);
        }else if(requestName.contains("SIZE?") || requestName.contains("LIST?")){
            sendSIZEAndLIST(list);
        }else if(requestName.contains("MOV")){
            
        }else if(requestName.contains("MALL?")){
            
        }else if(requestName.contains("SEND?")){
            
        }
    }
    
    
    
}