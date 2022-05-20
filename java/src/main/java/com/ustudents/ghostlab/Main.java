package com.ustudents.ghostlab;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.other.Utils;

import static java.lang.System.exit;

/** This is main class of the program. */
public class Main {
    /** Start of the program. */
    public static void main(String[] args) {
        try{
            String ipv4Addr = "127.0.0.1";
            int tcpPort = 4785;
            String username = null;
            int udpPort = 5541;

            for(int i = 0; i < args.length; i++){
                if(i == args.length-1 && !args[i].equals("--help") && !args[i].equals("-h")){
                    System.out.println("Error : You must give an argument with an option");
                    help();
                    exit(0);
                }
                String[] options = (i < args.length-1)? new String[]{args[i], args[i+1]}: new String[0];


                if((args[i].equals("--ip") || args[i].equals("-i"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 0)){
                        System.out.println("Error: You must give correct ipv4 address to use " + args[i]
                                + " options");
                        exit(0);
                    }
                    ipv4Addr = args[i+1];
                    i+=1;
                }else if((args[i].equals("--port") || args[i].equals("-p"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 1)){
                        System.out.println("Error: You must give 4 numbers to use " + args[i] + " options");
                        exit(0);
                    }
                    tcpPort = Integer.parseInt(args[i+1]);
                    i+=1;
                }else if((args[i].equals("--name") || args[i].equals("-n"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 2)){
                        System.out.println("Error: You must give 8 alphanumerics characters to use " + args[i]
                                + " options");
                        exit(0);
                    }
                    username = args[i+1];
                    i+=1;
                }else if((args[i].equals("--udp-port") || args[i].equals("-u"))){
                    if(!Utils.commandsArgsFormatAreCorrect(options, 1)){
                        System.out.println("Error: You must give 4 numbers to use " + args[i] + " options");
                        exit(0);
                    }
                    udpPort = Integer.parseInt(args[i+1]);
                    i+=1;
                }else if(args[i].equals("--help") || args[i].equals("-h")){
                    help();
                    exit(0);
                }else if(args[i].equals("--version") || args[i].equals("-v")){
                    System.out.println("version: 1.0.0");
                    exit(0);
                }else{
                    System.out.println("option not yet implemented : " + args[i] + " " + args[i].length());
                    exit(0);
                }
            }

            Client c = new Client();
            c.setIPv4Addr(ipv4Addr);
            c.settcpPort(tcpPort);
            c.setUsername(username);
            c.setudpPort(udpPort);
            c.launch(0);
            c.run(args);

        }catch (Exception e){
            e.printStackTrace();
            System.out.println("Connection refused : wrong ipv4 address or port");
        }
    }

    /**
     * Display help commands.
     */
    private static void help(){
        System.out.println("usage: client [options]");
        System.out.println();
        System.out.println("ghostlab is an online matchmaking based game where you take" +
                " upon yourself to become the best ghost hunter!");
        System.out.println();
        System.out.println("options:");
        System.out.println("\t-i, --ip <server ip> defines the ip to connect to" +
                " (127.0.0.1 by default).");
        System.out.println("\t-p, --port <server tcp port>         defines the port to connect to " +
                "(4785 by default).");
        System.out.println("\t-n, --name <player name>             defines the name to use when connected " +
                "to a match (will be asked later if not provided).");
        System.out.println("\t-u, --udp-port,  <client udp port>   defines the udp port to use to communicate " +
                "with other players (5541 used by default).");
        System.out.println("\t-h, --help                           displays this help message and exits.");
        System.out.println("\t-v, --version                        displays the program's version and exits.\n");

    }
}
