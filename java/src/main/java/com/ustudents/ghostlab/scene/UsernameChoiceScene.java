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

    private void createGameSection() throws IOException{
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
                client.SendRequest("NEWPL " + username + " " + client.getUdpPort() + "***");
            }else{
                client.addContentTologs("client: warning:", username, 1);
                client.addContentTologs("client: warning:",
                 "your username does'nt respect the format (8 characters)", 1);
            }

        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.setScene(SceneData.SCENE_MAIN);
        }
    }

    private void joinGameSection() throws IOException{
        if(client.getUsername() == null){
            ImGui.text("Enter a username:");
            ImGui.sameLine();
            ImGui.inputText("##UsernameChoice", client.getUsernameChoiceContent());
        }
        
        ImGui.text("Enter a game id:");
        ImGui.sameLine();
        ImGui.inputText("##GameChoice", client.getGameChoiceContent());

        if (ImGui.button("Use it")) {

            String username = client.getUsernameChoiceContent().get();
            String gameId = client.getGameChoiceContent().get();
            username = (client.getUsername() == null)? username: client.getUsername();
            client.getUsernameChoiceContent().clear();
            if(Utils.answerIsCorrectInput(username, 0)){
                client.setUsername(username);
                client.addContentTologs("client:", "your username are : " + username, 1);
                if(gameId.length() > 0 && Utils.answerIsCorrectInput(gameId, 1)){
                    client.setGameRegister(gameId);
                    client.addContentTologs("client:", "choose game : " + gameId, 1);
                    client.SendRequest("REGIS " + username + " " + client.getUdpPort()
                     + " " + Utils.tranformdigitTochar(gameId) + "***");
                }else{
                    client.addContentTologs("client: warning:", gameId, 1);
                    client.addContentTologs("client: warning:",
                 "your game choice does'nt respect the format (between 0 to 255)", 1);
                }
                
            }else{
                client.addContentTologs("client: warning:", username, 1);
                client.addContentTologs("client: warning:",
                 "your username does'nt respect the format (8 characters)", 1);
            }

        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.getGameChoiceContent().clear();
            client.setScene(SceneData.SCENE_MAIN);
        }
    }

    public void usernameChoice() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove |
        ImGuiWindowFlags.NoCollapse);

        if(client.getLastPressedButton() == SceneData.BUTTON_CREATEGAME){
            createGameSection();
        }else if(client.getLastPressedButton() == SceneData.BUTTON_JOINGAME){
            joinGameSection();
        }
        ImGui.end();
    }

    public void display() throws IOException{
        usernameChoice();
        mainContainer();
    }
    
}
