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

    /**Method run of MulticastRunnable. */
    @Override
    public void run() {
        try {
            MulticastSocket multicastSocket = new MulticastSocket(multicastPort);
            multicastSocket.joinGroup(InetAddress.getByName(multicastAddr));
            byte[]data=new byte[220];
            DatagramPacket paquet=new DatagramPacket(data,data.length);
            while(!client.getSocket().isClosed()){
                multicastSocket.receive(paquet);
                String receivedMessage = new String(paquet.getData(),0,paquet.getLength());
                client.addContentTologs("client: info: received from server:", receivedMessage, 0);
                String[] list = receivedMessage.split(" ");

                if(list[0].equals("GHOST")) {
                    String posX = list[1];
                    String posY = list[2].substring(0, list[2].length() - 3);
                    client.addContentTologs("client: info:", "ghost has been heard in " +
                     posX + " and " + posY, 0);
                }else if(list[0].equals("SCORE")) {
                    list[4] = list[4].substring(0, list[4].length()-3);
                    client.getGameModel().updatePlayerInfo(list[1], list[3], list[4], list[2]);
                    client.addContentTologs("client: info:", "the score of " + list[1] + " has been updated " 
                    + list[2] + " and has been heard in " + list[3] + " and " + list[4], 0);
                }else if(list[0].equals("ENDGA")){
                    String username = list[1];
                    String score = list[2].substring(0, list[2].length() - 3);
                    client.addContentTologs("client: info:", username + " won the game with " + score + " points", 1);
                    client.getSender().send("IQUIT***");
                }else if(list[0].equals("MESSA")){
                    String message = receivedMessage.substring(15, receivedMessage.length()-3); 
                    System.out.println("Size of message : " + message.length());
                    client.addContentTologs("client: info: received from " + list[1] + ":", message, 1);
                }
            }

            multicastSocket.leaveGroup(InetAddress.getByName("231.1.2.4"));

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
