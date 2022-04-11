package runnable;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.Socket;

public class UDPMulticastRunnable implements Runnable{

    private final Socket socket;

    public UDPMulticastRunnable(Socket socket){
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            MulticastSocket mso = new MulticastSocket(7759);
            mso.joinGroup(InetAddress.getByName("231.1.2.4"));
            byte[]data=new byte[100];
            DatagramPacket paquet=new DatagramPacket(data,data.length);
            while(!socket.isClosed()){
                mso.receive(paquet);
                String st=new String(paquet.getData(),0,paquet.getLength());
                System.out.println("[Global message] : "  +  st);
            }

            mso.leaveGroup(InetAddress.getByName("231.1.2.4"));

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
