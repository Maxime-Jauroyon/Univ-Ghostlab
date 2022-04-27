package com.ustudents.ghostlab.other;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.util.List;

import com.ustudents.ghostlab.client.Client;

public class Utils {

    /*public static int findEndIndex(String message, int startIndex, char endChar){
        while(message.charAt(startIndex) != endChar){
            startIndex++;
        }
        return startIndex;
    }*/

    public static boolean answerIsCorrectInput(String answer, int flag){
        if((flag == 0 && answer.length() != 8) || (flag == 1 && ((answer.length() > 3 || answer.length() < 1) 
            || (Integer.parseInt(answer) > 255 || Integer.parseInt(answer) < 0))) 
            || (flag == 2 && (answer.length() > 5 || answer.length() < 1)))
            return false;

        for(char c: answer.toCharArray()){
            if((flag == 0 && (c < 33 || c > 126)) || (flag == 1 && (c < 48 || c > 57)))
                return false;
        }

        return true;
    }

    public static int readOctets(BufferedReader br, int num) throws IOException{
        br.read();
        int res = 0;
        for(int i = 0; i < num; i++){
            res += br.read();
        }
        
        return res;
    }

    public static String readOctetToMakeString(BufferedReader br, int num) throws IOException{
        br.read();
        String res = "";
        for(int i = 0; i < num; i++){
            res += (char) br.read();
        }
        
        return res;
    }

    /*public static char tranformdigitTochar(String string){
        return (char) Integer.parseInt(string);
    }*/

    private static boolean checkServerIp(String[] args){
        try {
            return Inet4Address.getByName(args[1]).getHostAddress().equals(args[1]);
        }
        catch (UnknownHostException ex) {
            return false;
        }
    }

    public static boolean commandsArgsFormatAreCorrect(String[] args, int flag){
        if(args.length%2 != 0)
            return false;
        else if(flag == 0){
            return checkServerIp(args);
        }else if(flag == 1){
            return Utils.answerIsCorrectInput(args[1], 2);
        }else if(flag == 2){
            return Utils.answerIsCorrectInput(args[1], 0);
        }
        return false;
    }
}
