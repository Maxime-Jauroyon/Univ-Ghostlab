package com.ustudents.ghostlab.scene;

import java.io.IOException;
import java.util.List;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class GameLobbyScene extends Scene{

    public GameLobbyScene(Client client){
        super(client);
    }

    /**
     * To display the option of the game lobby scene.
     */
    public void gameLobbyOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client - Game " + client.getGameRegister(), ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);
        
        if(!client.getBlockClientRequest().get()){
            if(ImGui.button("Start")){
                client.getBlockClientRequest().set(true);
                client.getSender().send("START***");
            }
            ImGui.sameLine();
            if(ImGui.button("Unregister"))
                client.getSender().send("UNREG***");
        }
        
        if(ImGui.collapsingHeader("Available Games")){
            if(!client.getBlockClientRequest().get() && ImGui.button("Reload Games Data")){
                client.getSender().sendServerInfo();
            } 
            
            for(int gameId: client.getRequestGamesId()){
                String yourgame = (client.getGameRegister() == gameId)? "(your registered game)": "";
                if(ImGui.collapsingHeader("Game " + gameId + " " + yourgame)){
                    if(ImGui.collapsingHeader("Players") 
                        && client.getRequestPlayersUsernamePerGames().containsKey(gameId)){
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
     * To display the game lobby scene.
     */
    public void display() throws IOException{
        gameLobbyOption();  
        mainContainer();
    }
    
}
