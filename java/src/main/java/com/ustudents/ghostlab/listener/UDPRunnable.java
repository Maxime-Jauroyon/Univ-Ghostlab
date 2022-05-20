package com.ustudents.ghostlab.listener;

import com.ustudents.ghostlab.client.Client;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UDPRunnable implements Runnable{
    private final Client client;
    private final DatagramSocket datagramSocket;

    public UDPRunnable(Client client, DatagramSocket datagramSocket){
        this.client = client;
        this.datagramSocket = datagramSocket;
    }

    /**Method run of UDPRunnable. */
    @Override
    public void run() {
        try {
            while(!client.getSocket().isClosed()) {
                byte[] data = new byte[200];
                DatagramPacket packet = new DatagramPacket(data, data.length);
                datagramSocket.receive(packet);
                String receivedMessage = new String(packet.getData(), 0, packet.getLength());
                String list[] = receivedMessage.split(" ");
                String message = receivedMessage.substring(15, receivedMessage.length()-3); 
                client.addContentTologs("client: info: received from server:", receivedMessage, 0);
                client.addContentTologs("client: info: received from " + list[1] + ":", message, 1);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
