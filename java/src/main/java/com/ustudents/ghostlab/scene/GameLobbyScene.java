package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class GameLobbyScene extends Scene{

    public GameLobbyScene(Client client){
        super(client);
    }

    public void mainSceneOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Start")){
            
        }

        ImGui.sameLine();
        
        if(ImGui.button("Unregister")){
            client.SendRequest("UNREG***");
        }

        ImGui.end();
    }

    public void display() throws IOException{
        mainSceneOption();  
        mainContainer();
    }
    
}
