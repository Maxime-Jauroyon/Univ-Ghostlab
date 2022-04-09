package client;

import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Utils {
    public static int findEndIndex(String message, int startIndex, char endChar){
        while(message.charAt(startIndex) != endChar){
            startIndex++;
        }
        return startIndex;
    }

    private static boolean answerIsCorrect(String[] correctAnswers, String userAnswer){
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
    }

    public static boolean answerIsCorrectInput(String answer, int flag){
        if((flag == 0 && answer.length() != 8) || (flag == 1 && (answer.length() > 3 || answer.length() < 1))
                || (flag == 2 && (answer.length() > 5 || answer.length() < 1)))
            return false;

        for(char c: answer.toCharArray()){
            if((flag == 0 && (c < 33 || c > 126)) || (flag == 1 && (c < 48 || c > 57)))
                return false;
        }
        return true;
    }

    public static String getInput(String question, Scanner sc, int flag){
        while(true){
            System.out.print(question);
            String temp = sc.next();
            if(answerIsCorrectInput(temp, flag)){
                return temp;
            }
        }
    }

    public static boolean commandsArgsFormatIsCorrect(String[] args){
        if(args.length != 3 || !Utils.answerIsCorrectInput(args[1], 2)
                || !Utils.answerIsCorrectInput(args[2], 2))
            return false;

        try {
            return Inet4Address.getByName(args[0]).getHostAddress().equals(args[0]);
        }
        catch (UnknownHostException ex) {
            return false;
        }
    }
}
