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
    }

    public void display() throws IOException{ 
        mainSceneOption();  
        mainContainer();
    }
    
}
