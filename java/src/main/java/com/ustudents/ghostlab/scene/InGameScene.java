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

        char[][] maze = client.getGameModel().getMaze();
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                ImGui.text(" ");
                ImGui.sameLine();
                ImGui.text(String.valueOf(maze[i][j]));
                ImGui.sameLine();
                ImGui.text(" ");
                if(j != maze[i].length-1){
                    ImGui.sameLine();
                }
            }
        }

        if(ImGui.button("Move")){
            client.setLastPressedButton(SceneData.BUTTON_MAZEMOVE);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();
        
        if(ImGui.button("Player list")){
            client.getSender().send("GLIS?***");
        }

        ImGui.sameLine();
        
        if(ImGui.button("Send Everyone Message")){
            client.setLastPressedButton(SceneData.BUTTON_ALLMESSAGE);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();

        if(ImGui.button("Send Private Message")){
            client.setLastPressedButton(SceneData.BUTTON_PRIVATEMESSAGE);
            client.setCurrentScene(SceneData.SCENE_USERCHOICE);
        }

        ImGui.sameLine();

        if(ImGui.button("Quit Game")){
            client.getSender().send("IQUIT***");
        }

        ImGui.end();
    
    }    

    public void display() throws IOException{ 
        inGameOption();
        mainContainer();
        
    }
}
