package com.ustudents.ghostlab.scene;

import java.io.IOException;
import java.util.List;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class MainScene extends Scene{

    public MainScene(Client client){
        super(client);
    }

    /**
     * To display the option of the main scene.
     */
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
                client.getSender().sendServerInfo();
            } 
            
            for(int gameId: client.getRequestGamesId()){
                if(ImGui.collapsingHeader("Game " + gameId)){
                    if(ImGui.button("Join")){
                        client.setGameRegister(gameId);
                        if(client.getUsername() == null){
                            client.setLastPressedButton(SceneData.BUTTON_JOINGAME);
                            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
                        }else{
                            client.getSender().send("REGIS " + client.getUsername() + " " + client.getUdpPort()
                            + " " + gameId + "***");
                        }
                    }

                    if(ImGui.collapsingHeader("Players") && 
                        client.getRequestPlayersUsernamePerGames().containsKey(gameId)){
                        List<String> playersUsername = client.getRequestPlayersUsernamePerGames().get(gameId);
                        for(String playerUsername: playersUsername){
                            ImGui.text("- " + playerUsername);
                        }
                        
                    }

                    if(ImGui.collapsingHeader("Maze") && 
                    client.getRequestPlayersUsernamePerGames().containsKey(gameId)){
                        List<Integer> mazes = client.getRequestMazeSizePerGame().get(gameId);
                        ImGui.text("- Size : " + mazes.get(0) + "x" + mazes.get(1));
                    }
                    
                }    
            }
            
            
        }

        ImGui.end();
    }

    /**
     * To display the main scene.
     */
    public void display() throws IOException{ 
        mainSceneOption();  
        mainContainer();
    }
    
}
