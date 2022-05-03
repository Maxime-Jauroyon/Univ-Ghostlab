package com.ustudents.ghostlab.listener;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

import com.ustudents.ghostlab.client.Client;

public class MulticastRunnable implements Runnable{

    private final Client client;
    private final String multicastAddr;
    private final int multicastPort;

    public MulticastRunnable(Client client, String multicastAddr, int multicastPort){
        this.client = client;
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
            while(!client.getSocket().isClosed()){
                mso.receive(paquet);
                String receivedMessage = new String(paquet.getData(),0,paquet.getLength());
                client.addContentTologs("client: info: received from server:", receivedMessage, 0);
                String[] list = receivedMessage.split(" ");

                if(list[0].equals("GHOST")) {
                    String posX = list[1];
                    String posY = list[2].substring(0, list[2].length() - 3);
                    client.addContentTologs("client: info:", "ghost has been heard in " +
                     posX + " and " + posY, 0);
                }else if(list[0].equals("SCORE")) {
                    String score = list[2];
                    client.addContentTologs("client: info:", "your score has been updated" +
                     score, 0);
                    client.getGameModel().setScore(Integer.parseInt(score));
                }else if(list[0].equals("ENDGA")){
                    String username = list[1];
                    String score = list[2].substring(0, list[2].length() - 3);
                    client.addContentTologs("client: info:", username + " won the game with " + score + " points", 1);
                    client.getSender().send("IQUIT***");
                }else if(list[0].equals("MESSA")){
                    String message = receivedMessage.substring(15, receivedMessage.length()-3); 
                    client.addContentTologs("client: info: received from " + list[1] + ":", message, 1);
                }
            }

            mso.leaveGroup(InetAddress.getByName("231.1.2.4"));

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
