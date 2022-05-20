package com.ustudents.ghostlab.other;

import java.net.Inet4Address;
import java.net.UnknownHostException;

public class Utils {

    /**
     * To make space.
     * @param n Number of space.
     * @return N space char
     */
    public static String makeSpace(int n){
        String res = "";
        for(int i = 0; i < n; i++){
            res += " ";
        }
        return res;
    }

    /**
     * Check if answer contains char between [^0-9]
     * @param answer String to check.
     * @return True if contains char between [^0-9], else false.
     */
    public static boolean containsChar(String answer){
        for(int i = 0; i < answer.length(); i++){
            char temp = answer.charAt(i);
            if(temp < 48 || temp > 57)
                return false;
        }
        return true;
    }

    /**
     * Check the answer is correct in function of flag.
     * @param answer String to check.
     * @param flag Method of check choosed.
     * @return True if the format is validate, else false.
     */
    public static boolean answerIsCorrectInput(String answer, int flag){
        if((flag == 0 && answer.length() != 8) || (flag == 1 && 
             (!containsChar(answer) || (answer.length() > 3 || answer.length() < 1) 
            || (Integer.parseInt(answer) > 255 || Integer.parseInt(answer) < 0))) 
            || (flag == 2 && (answer.length() != 4))
            || (flag == 3 && (!containsChar(answer) || Integer.parseInt(answer) < 0 
            || Integer.parseInt(answer) > 999)))
            return false;

        for(char c: answer.toCharArray()){
            if((flag == 0 && (c < 33 || c > 126)) || (flag == 1 && (c < 48 || c > 57)))
                return false;
        }

        return true;
    }

    /**
     * Check if message if correct.
     * @param answer String to check.
     * @return True if the format is validate, else false.
     */
    public static boolean messageIsCorrect(String answer){
        return answer.length() > 0 && answer.length() < 201; 
    }
    
    /**
     * Check if the server ipv4 address is correct.
     * @param args All arguments given to the program.
     * @return True if ipv4 address is correct, else false.
     */
    private static boolean checkServerIp(String[] args){
        try {
            return Inet4Address.getByName(args[1]).getHostAddress().equals(args[1]);
        }
        catch (UnknownHostException ex) {
            return false;
        }
    }

    /**
     * Verify if the command in argument is correct
     * @param args All arguments given to the program.
     * @param flag To choose the specific argument to check.
     * @return true if the command in argument is correct, else false.
     */
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
