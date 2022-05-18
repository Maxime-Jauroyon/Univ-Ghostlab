package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class MainScene extends Scene{

    public MainScene(Client client){
        super(client);
    }

    /*public void mainSceneOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Create Game")){
            client.setLastPressedButton(SceneData.BUTTON_CREATEGAME);
            if(client.getUsername() == null){
                client.setCurrentScene(SceneData.SCENE_USERCHOICE);
            }else{
                client.getSender().send("NEWPL " + client.getUsername() + " " + client.getUdpPort() + "***");
            }
        }
        
        ImGui.sameLine();
        
        if(ImGui.button("Join Game")){
            client.setLastPressedButton(SceneData.BUTTON_JOINGAME);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();
        
        if(ImGui.button("Maze info")){
            client.setLastPressedButton(SceneData.BUTTON_MAZEINFO);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();

        if(ImGui.button("List player")){
            client.setLastPressedButton(SceneData.BUTTON_LISTPLAYER);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();

        if(ImGui.button("Game Info")){
            client.getSender().send("GAME?***");
        }

        ImGui.end();
    }*/

    public void mainSceneOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Creates Game")){
            client.setLastPressedButton(SceneData.BUTTON_CREATEGAME);
            if(client.getUsername() == null){
                client.setCurrentScene(SceneData.SCENE_USERCHOICE);
            }else{
                client.getSender().send("NEWPL " + client.getUsername() + " " + client.getUdpPort() + "***");
            }
        }
        
        if(ImGui.collapsingHeader("Available Games")){
            if(ImGui.button("Reload Games Data")){
                //client.getTcpSender().setState(1);
                client.getSender().send("GAME?***");
                client.getSender().send("LIST? " + 0  + "***");
            } 
            
            /*for(int i = 0; i < client.getGamesId().size(); i++){
                int gameId = client.getGamesId().get(i);
                if(ImGui.collapsingHeader("Game " + gameId)){
                    if(ImGui.button("Join")){
                        if(client.getUsername() == null){
                            client.setGameRegister(gameId);
                            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
                        }else{
                            client.getSender().send("REGIS " + client.getUsername() + " " + client.getUdpPort()
                            + " " + gameId + "***");
                        }
                    }
                    
                    if(ImGui.collapsingHeader("Players") && 
                        client.getPlayersUsernamePerGames().containsKey(gameId)){
                        List<String> playersUsername = client.getPlayersUsernamePerGames().get(gameId);
                        for(String playerUsername: playersUsername){
                            ImGui.text("- " + playerUsername);
                        }
                    }
                }    
            }*/
            
            
        }

        ImGui.end();
    }

    public void display() throws IOException{ 
        mainSceneOption();  
        mainContainer();
    }
    
}
