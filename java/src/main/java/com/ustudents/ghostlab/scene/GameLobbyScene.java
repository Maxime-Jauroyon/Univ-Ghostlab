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

    public void gameLobbyOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);

        if(ImGui.button("Start")){
            client.getSender().send("START***");
        }

        ImGui.sameLine();
        
        if(ImGui.button("Unregister")){
            client.getSender().send("UNREG***");
        }
        
        if(ImGui.collapsingHeader("Available Games")){
            if(ImGui.button("Reload Games Data")){
                client.getSender().sendServerInfo();
            } 
            
            for(int gameId: client.getRequestGamesId()){
                if(ImGui.collapsingHeader("Game " + gameId)){
                    if(ImGui.collapsingHeader("Players") && 
                        client.getRequestPlayersUsernamePerGames().containsKey(gameId)){
                        List<String> playersUsername = client.getRequestPlayersUsernamePerGames().get(gameId);
                        for(String playerUsername: playersUsername){
                            ImGui.text("- " + playerUsername);
                        }
                    }
                }    
            }
            
            
        }

        ImGui.end();
    }

    public void display() throws IOException{
        gameLobbyOption();  
        mainContainer();
    }
    
}
