package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.PrintWriter;

public class Sender {

    public static void send(PrintWriter pw, String action) throws IOException{
        pw.print(action);
        pw.flush();
    }    
    
}
