package com.ustudents.ghostlab.scene;

import java.io.IOException;

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
            client.sendRequest("START***");
        }

        ImGui.sameLine();
        
        if(ImGui.button("Unregister")){
            client.sendRequest("UNREG***");
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
            client.sendRequest("GAME?***");
        }

        ImGui.end();
    }

    public void display() throws IOException{
        gameLobbyOption();  
        mainContainer();
    }
    
}
