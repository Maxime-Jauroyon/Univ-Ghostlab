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

    @Override
    public void run() {
        try {
            while(!client.getSocket().isClosed()) {
                byte[] data = new byte[200];
                DatagramPacket packet = new DatagramPacket(data, data.length);
                datagramSocket.receive(packet);
                String receivedMessage = new String(packet.getData(), 0, packet.getLength());
                //Utils.printAllMessage(receivedMessage, "Private Message From");
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
