package com.ustudents.ghostlab.runnable;

import com.ustudents.ghostlab.client.Utils;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.Socket;

public class UDPMulticastRunnable implements Runnable{

    private final Socket socket;
    private final String multicastAddr;
    private final int multicastPort;

    public UDPMulticastRunnable(Socket socket, String multicastAddr, int multicastPort){
        this.socket = socket;
        this.multicastAddr = multicastAddr;
        this.multicastPort = multicastPort;
    }

    @Override
    public void run() {
        try {
            MulticastSocket mso = new MulticastSocket(multicastPort);
            mso.joinGroup(InetAddress.getByName(multicastAddr));
            byte[]data=new byte[200];
            DatagramPacket paquet=new DatagramPacket(data,data.length);
            while(!socket.isClosed()){
                mso.receive(paquet);
                String receivedMessage = new String(paquet.getData(),0,paquet.getLength());
                String[] list = receivedMessage.split(" ");

                if(list[0].equals("GHOST")) {
                    String posX = list[1];
                    String posY = list[2].substring(0, list[2].length() - 3);
                    System.out.println("[Ghost] : has moved in : (" + posX + "," + posY + ")");
                }else if(list[0].equals("SCORE")) {
                    String username = list[1];
                    String score = list[2];
                    String posX = list[3];
                    String posY = list[4].substring(0, list[4].length() - 3);
                    System.out.println("[" + username + "] : has kept a ghost in : (" + posX + "," + posY + ") "
                            + " with " + score + " points");
                }else if(list[0].equals("ENDGA")){
                    String username = list[1];
                    String score = list[2].substring(0, list[2].length() - 3);
                    System.out.println("[Winner] : " + username + " with " + score + " points");
                }else if(list[0].equals("MESSA")){
                    //Utils.printAllMessage(receivedMessage, "Global Message From");
                }
            }

            mso.leaveGroup(InetAddress.getByName("231.1.2.4"));

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
