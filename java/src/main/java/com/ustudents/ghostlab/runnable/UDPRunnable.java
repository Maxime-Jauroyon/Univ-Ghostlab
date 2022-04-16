package com.ustudents.ghostlab.runnable;

import com.ustudents.ghostlab.client.Utils;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Socket;

public class UDPRunnable implements Runnable{
    private final Socket socket;
    private final DatagramSocket datagramSocket;

    public UDPRunnable(Socket socket, DatagramSocket datagramSocket){
        this.socket = socket;
        this.datagramSocket = datagramSocket;
    }

    @Override
    public void run() {
        try {
            while(!socket.isClosed()) {
                byte[] data = new byte[200];
                DatagramPacket packet = new DatagramPacket(data, data.length);
                datagramSocket.receive(packet);
                String receivedMessage = new String(packet.getData(), 0, packet.getLength());
                Utils.printAllMessage(receivedMessage, "Private Message From");
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
