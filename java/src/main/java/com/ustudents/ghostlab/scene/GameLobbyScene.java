package com.ustudents.ghostlab.scene;

import com.ustudents.ghostlab.client.Client;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class GameLobbyScene extends Scene{

    public GameLobbyScene(Client client){
        super(client);
    }

    public void mainSceneOption(){
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);


        if(ImGui.button("Create Game")){
            if(client.getUsername() == null){
                client.setScene(SceneData.SCENE_USERNAMECHOICE);
            }else{
                client.setScene(SceneData.SCENE_GAMELOBBY);
            }
        } 

        ImGui.end();
    }

    public void display(){
        header(); 
        mainSceneOption();  
        footer();
    }
    
}
