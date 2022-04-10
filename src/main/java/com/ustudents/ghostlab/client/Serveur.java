package client;

import client.Utils;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Serveur {

    private static void callGameMessage(PrintWriter pw){
        pw.print("GAMES 1***\n");
        pw.flush();
        pw.print("OGAMES 0 2***\n");
        pw.flush();
    }

    private static void callListMessage(PrintWriter pw){
        pw.print("LIST! 0 2***\n");
        pw.flush();
        pw.print("PLAYR leo***\n");
        pw.flush();
        pw.print("PLAYR kevin***\n");
        pw.flush();
    }

    private static void callSizeMessage(PrintWriter pw){
        pw.print("SIZE! 0 2 2***\n");
        pw.flush();
    }

    private static void callUnregister(PrintWriter pw){
        pw.print("UNROK 0***\n");
        pw.flush();
    }

    private static void callNewGame(PrintWriter pw){
        pw.print("REGOK 0***\n");
        pw.flush();
    }

    private static void callWelcome(PrintWriter pw){
        pw.print("WELCO 0 20 20 3 224.0.1.4 6677***\n");
        pw.flush();
    }

    private static void callPOSIT(PrintWriter pw){
        pw.print("POSIT leotom22 0 0***\n");
        pw.flush();
    }

    private static void callIQUIT(PrintWriter pw){
        pw.print("GOBYE***\n");
        pw.flush();
    }

    private static void callRIMOV(PrintWriter pw){
        pw.print("MOVE! 003 001***\n");
        pw.flush();
    }

    private static void callGLIS(PrintWriter pw){
        pw.print("GLIS! 2***\n");
        pw.flush();
        pw.print("[GPLYR leotom22 002 003 0010***\n");
        pw.flush();
        pw.print("[GPLYR leotom23 003 004 0011***\n");
        pw.flush();
    }

    private static void someStartCommands(Socket socket, BufferedReader br, PrintWriter pw) throws IOException {
        while(true){
            String clientMessage = br.readLine();
            if(clientMessage.startsWith("REGIS") || clientMessage.startsWith("NEWPL")){
                pw.print("REGOK 0***\n");
            }else if(clientMessage.startsWith("UNREG")){
                pw.print("UNROK 0***\n");
            }else if(clientMessage.startsWith("SIZE?")){
                String idGame = clientMessage.substring(6, Utils.findEndIndex(clientMessage, 6, '*'));
                pw.print("SIZE! " + idGame + " 10 10***\n");
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
            if(clientMessage.startsWith("UNREG") || clientMessage.startsWith("LIST?")){
                callGameMessage(pw);
            }
        }
    }

    private static void someGameCommands(Socket socket, BufferedReader br, PrintWriter pw) throws IOException {
        while (true) {
            String clientMessage = br.readLine();
            System.out.println(clientMessage);
            if(clientMessage.startsWith("IQUIT")){
                callIQUIT(pw);
                socket.close();
                return;
            }else if(clientMessage.contains("MOV")){
                System.out.println(clientMessage);
                callRIMOV(pw);
            }else if(clientMessage.startsWith("GLIS?")){
                callGLIS(pw);
            }
        }
    }

    public static void main(String[] args){
        try{
            ServerSocket serverSocket = new ServerSocket(5557);

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
