package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.client.Utils;

import org.lwjgl.glfw.GLFW;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class UsernameChoiceScene extends Scene {
    
    

    public UsernameChoiceScene(Client client){
        super(client);
    }

    public void usernameChoice() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove |
        ImGuiWindowFlags.NoCollapse);


        ImGui.text("Enter a username:");
        ImGui.sameLine();
        ImGui.inputText("##UsernameChoice", client.getUsernameChoiceContent());
        ImGui.sameLine();

        if (ImGui.button("Use it")) {

            String username = client.getUsernameChoiceContent().get();
            client.getUsernameChoiceContent().clear();
            if(Utils.answerIsCorrectInput(username, 0)){
                client.setUsername(username);
                client.addContentTologs("client:", "your username are : " + username, 1);
                //client.setScene(SceneData.GAMELOBBY);
                client.SendRequest("NEWPL " + client.getUsername() + " " + client.getUdpPort() + "***");
            }else{
                client.addContentTologs("client: warning:", username, 1);
                client.addContentTologs("client: warning:",
                 "your username does'nt respect the format (8 characters)", 1);
            }

        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.setScene(SceneData.SCENE_MAIN);
        }

        ImGui.end();
    }

    public void display() throws IOException{
        header();
        usernameChoice();
        footer();
    }
    
}
