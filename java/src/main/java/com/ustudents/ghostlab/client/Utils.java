package com.ustudents.ghostlab.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.Inet4Address;
import java.net.UnknownHostException;

public class Utils {

    public static int findEndIndex(String message, int startIndex, char endChar){
        while(message.charAt(startIndex) != endChar){
            startIndex++;
        }
        return startIndex;
    }

    /*private static boolean answerIsCorrect(String[] correctAnswers, String userAnswer){
        for (String correctAnswer : correctAnswers) {
            if (userAnswer.equals(correctAnswer))
                return true;
        }
        return false;
    }

    private static void printCorrectAnswer(String[] correctAnswers){
        for (String correctAnswer : correctAnswers) {
            System.out.print(correctAnswer + ", ");
        }
        System.out.println();
    }

    public static String gameStepChoice(String question, String[] correctAnswers, Scanner sc){
        String userAnswer;
        while(true){
            System.out.println(question);
            userAnswer = sc.next();
            if(answerIsCorrect(correctAnswers, userAnswer)){
                return userAnswer;
            }else{
                System.out.print("Wrong answer you must choose between : ");
                printCorrectAnswer(correctAnswers);
            }
        }
    }*/

    public static boolean answerIsCorrectInput(String answer, int flag){
        if((flag == 0 && answer.length() != 8) || (flag == 1 && (answer.length() > 3 || answer.length() < 1) 
            && (Integer.parseInt(answer) > 255 || Integer.parseInt(answer) < 0)) 
            || (flag == 2 && (answer.length() > 5 || answer.length() < 1)))
            return false;

        for(char c: answer.toCharArray()){
            if((flag == 0 && (c < 33 || c > 126)) || (flag == 1 && (c < 48 || c > 57)))
                return false;
        }

        return true;
    }

    /*public static String getInput(String question, Scanner sc, int flag){
        while(true){
            System.out.println(question);
            Scanner sc1 = new Scanner(System.in);
            String temp = sc1.nextLine();
            if(answerIsCorrectInput(temp, flag)){
                return temp;
            }
        }
    }

    public static String messageToSend(){
        Scanner temp = new Scanner(System.in);
        System.out.println("Your message : ");
        String message = temp.nextLine();
        return (message.length() > 200)? message.substring(0, 200): message;
    }

    public static String test(Scanner sc){
        String res = sc.next();
        while(sc.hasNext()){
            res += sc.next();
        }
        return res;
    }

    public static void printAllMessage(String receivedMessage, String type){
        String[] list = receivedMessage.split(" ");
        System.out.print("[" + type +  " : " + list[1] + "] : ");
        for(int i = 2; i < list.length; i++){
            if(i == list.length-1){
                System.out.print(list[i].substring(0, list[i].length()-3) + " ");
            }else{
                System.out.print(list[i] + " ");
            }
        }
        System.out.println();
    }*/

    /*private static boolean checkThreeNextChar(String read){
        if(read.length() < 3)
            return false;

        for(int i = 3; i > 0; i--){
            if(read.charAt(read.length()-i) != '*'){
                return false;
            }
        }
        return true;
    }

    public static String read(BufferedReader br) throws IOException {
        String read = "";
        while(!checkThreeNextChar(read)){
            read += String.valueOf(br.read());
        }
        return read;
    }*/

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

    public static char tranformdigitTochar(String string){
        return (char) Integer.parseInt(string);
    }

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
