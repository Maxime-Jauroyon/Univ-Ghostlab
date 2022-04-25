package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class MainScene extends Scene{

    public MainScene(Client client){
        super(client);
    }

    public void mainSceneOption() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Create Game")){
            client.setLastPressedButton(SceneData.BUTTON_CREATEGAME);
            if(client.getUsername() == null){
                client.setScene(SceneData.SCENE_USERNAMECHOICE);
            }else{
                client.SendRequest("NEWPL " + client.getUsername() + " " + client.getUdpPort() + "***");
            }
        }
        
        ImGui.sameLine();
        
        if(ImGui.button("Join Game")){
            client.setLastPressedButton(SceneData.BUTTON_JOINGAME);
            client.setScene(SceneData.SCENE_USERNAMECHOICE);
        }

        ImGui.end();
    }

    public void display() throws IOException{ 
        mainSceneOption();  
        mainContainer();
    }
    
}
