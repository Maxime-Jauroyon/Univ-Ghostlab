package com.ustudents.ghostlab.communication;

import java.io.IOException;
import java.io.PrintWriter;

public class Sender {

    public static void sender(PrintWriter pw, String action) throws IOException{
        pw.print(action);
        pw.flush();
    }    
    
}
