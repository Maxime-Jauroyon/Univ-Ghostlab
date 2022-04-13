package client;

import java.io.*;
import java.net.*;

public class Serveur {

    private static void callGameMessage(PrintWriter pw){
        pw.print("GAMES 1***");
        pw.flush();
        pw.print("OGAMES 0 2***");
        pw.flush();
    }

    private static void callListMessage(PrintWriter pw){
        pw.print("LIST! 0 2***");
        pw.flush();
        pw.print("PLAYR leo***");
        pw.flush();
        pw.print("PLAYR kevin***");
        pw.flush();
    }

    private static void callSizeMessage(PrintWriter pw){
        pw.print("SIZE! 0 2 2***");
        pw.flush();
    }

    private static void callUnregister(PrintWriter pw){
        pw.print("UNROK 0***");
        pw.flush();
    }

    private static void callNewGame(PrintWriter pw){
        pw.print("REGOK 0***");
        pw.flush();
    }

    private static void callWelcome(PrintWriter pw){
        pw.print("WELCO 0 20 20 3 231.1.2.4### 7759***");
        pw.flush();
    }

    private static void callPOSIT(PrintWriter pw){
        pw.print("POSIT leotom22 0 0***");
        pw.flush();
    }

    private static void callIQUIT(PrintWriter pw){
        pw.print("GOBYE***");
        pw.flush();
    }

    private static void callRIMOV(PrintWriter pw){
        pw.print("MOVE! 003 001***");
        pw.flush();
    }

    private static void callGLIS(PrintWriter pw){
        pw.print("GLIS! 2***");
        pw.flush();
        pw.print("[GPLYR leotom22 002 003 0010***");
        pw.flush();
        pw.print("[GPLYR leotom23 003 004 0011***");
        pw.flush();
    }

    private static void callMALL(PrintWriter pw, String toSend) throws IOException {
        pw.print("MALL!***");
        pw.flush();
        String message = "MESSA " + "leotom22" + " " + toSend + "+++";
        byte[] data = message.getBytes();
        DatagramSocket dso= new DatagramSocket();
        InetSocketAddress ia=new InetSocketAddress("231.1.2.4",7759);
        DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
        dso.send(paquet);
    }

    private static void callSEND(PrintWriter pw, String username, String toSend) throws IOException {
        pw.print("SEND!***");
        pw.flush();
        String message = "MESSP " + username + " " + toSend + "+++";
        byte[] data = message.getBytes();
        DatagramSocket dso= new DatagramSocket();
        InetSocketAddress ia=new InetSocketAddress("127.0.0.1",5541);
        DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
        dso.send(paquet);
    }


    private static void someStartCommands(Socket socket, BufferedReader br, PrintWriter pw) throws IOException {
        while(true){
            String clientMessage = Utils.read(br);
            if(clientMessage.startsWith("REGIS") || clientMessage.startsWith("NEWPL")){
                pw.print("REGOK 0***");
                pw.flush();
            }else if(clientMessage.startsWith("UNREG")){
                pw.print("UNROK 0***");
            }else if(clientMessage.startsWith("SIZE?")){
                String idGame = clientMessage.substring(6, Utils.findEndIndex(clientMessage, 6, '*'));
                pw.print("SIZE! " + idGame + " 10 10***");
            }else if(clientMessage.startsWith("LIST?")){
                callListMessage(pw);
            }else if(clientMessage.startsWith("GAME?")){
                callGameMessage(pw);
            }else if(clientMessage.startsWith("START")){
                callWelcome(pw);
                return;
            }else if(clientMessage.startsWith("IQUIT")){
                callIQUIT(pw);
                socket.close();
                return;
            }

            pw.flush();
            if(clientMessage.startsWith("UNREG")){
                callGameMessage(pw);
            }
        }
    }

    private static void someGameCommands(Socket socket, BufferedReader br, PrintWriter pw) throws IOException {
        while (true) {
            String clientMessage = Utils.read(br);
            String[] list = clientMessage.split(" ");
            if(clientMessage.startsWith("IQUIT")){
                callIQUIT(pw);
                socket.close();
                return;
            }else if(clientMessage.contains("MOV")){
                callRIMOV(pw);
            }else if(clientMessage.startsWith("GLIS?")){
                callGLIS(pw);
            }else if(clientMessage.startsWith("MALL?")){
                callMALL(pw, clientMessage.substring(6, clientMessage.length()-3));
            }else if(clientMessage.startsWith("SEND?")){
                callSEND(pw, list[1], clientMessage.substring(15, clientMessage.length()-3));
            }
        }
    }

    public static void main(String[] args){
        try{
            ServerSocket serverSocket = new ServerSocket(4785);

            while(true){
                Socket socket = serverSocket.accept();
                BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                PrintWriter pw = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
                callGameMessage(pw); //game command
                someStartCommands(socket, br, pw);
                callPOSIT(pw);
                someGameCommands(socket, br, pw);
            }



        }catch(Exception e){
            e.printStackTrace();
        }
    }
}
