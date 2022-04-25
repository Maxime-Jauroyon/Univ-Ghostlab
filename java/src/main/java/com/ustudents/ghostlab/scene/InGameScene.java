package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class InGameScene extends Scene{
    
    public InGameScene(Client client){
        super(client);
    }

    public void inGameOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Move")){
            
        }

        ImGui.sameLine();
        
        if(ImGui.button("Player list")){
            
        }

        ImGui.sameLine();
        
        if(ImGui.button("Send Everyone Message")){
            
        }

        ImGui.sameLine();

        if(ImGui.button("Send Private Message")){
            
        }

        ImGui.end();
    
    }    

    public void display() throws IOException{ 
        mainContainer();
    }
}
