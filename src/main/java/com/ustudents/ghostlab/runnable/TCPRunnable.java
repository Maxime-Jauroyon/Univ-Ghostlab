package runnable;

import client.Client;
import interaction.InteractionInGamePhase;

import java.io.BufferedReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class TCPRunnable implements Runnable{

    private final Client client;
    private final BufferedReader br;
    private final PrintWriter pw;
    private final Scanner sc;


    public TCPRunnable(Client client,
                       BufferedReader br, PrintWriter pw,
                       Scanner sc){

        this.client = client;
        this.br = br;
        this.pw = pw;
        this.sc = sc;
    }

    @Override
    public void run() {
        try{
            String question = "What to you want to do ? (move/list/messall/messto/quit)";
            InteractionInGamePhase iigp = new InteractionInGamePhase(client);
            iigp.putQuestionOnGamePhase(br, pw, question, new String[]{"move","list","messall","messto","quit"}, sc);

        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
