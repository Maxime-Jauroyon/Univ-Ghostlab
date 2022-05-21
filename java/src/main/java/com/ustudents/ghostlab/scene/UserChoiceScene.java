package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.other.Utils;

import org.lwjgl.glfw.GLFW;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class UserChoiceScene extends Scene {
    
    

    public UserChoiceScene(Client client){
        super(client);
    }  
    
    /**
     * Display join or create game popup.
     * @param type Type of popup.
     */
    public void joinOrCreateGamePopupDraw(String type) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {
            ImGui.text("Choose a name:");
            ImGui.sameLine();
            ImGui.inputText("###PlayerName", client.getUsernameChoiceContent());
            ImGui.sameLine();

            if(ImGui.button("Send")){
                String username = client.getUsernameChoiceContent().get();
                client.getUsernameChoiceContent().clear();
                if(Utils.answerIsCorrectInput(username, 0)){
                    client.setUsername(username);
                    client.addContentTologs("client: info:", "your username are : " + 
                    username, 1);
                    if(type.equals("CreateGame")){
                        client.getSender().send("NEWPL " + username + " " + client.getUdpPort() + "***");
                    }else{
                        client.getSender().send("REGIS " + username + " " + client.getUdpPort() + " " + client.getGameRegister() + "***");
                    }
                }else{
                    client.addContentTologs("client: warning:", username, 1);
                    client.addContentTologs("client: warning:",
                    "your username does'nt respect the format (8 characters)", 1);
                    client.setCurrentScene(SceneData.SCENE_MAIN);
                }
            }

            ImGui.sameLine();

            if (ImGui.button("Back")) {
                client.getUsernameChoiceContent().clear();
                client.backToPreviousScene();
            }
        }
        ImGui.endPopup();
    }

    /**
     * Display send message in game popup.
     * @param type Type of popup.
     * @param flag 0 to send private message, else everyone message.
     */
    public void sendMessageInGamePopupDraw(String type, int flag) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {

            ImGui.text("Your Message:");
            ImGui.sameLine();
            ImGui.inputText("##Message", client.getGameChoiceContent());
            ImGui.sameLine();
            String username = "";
            String message = "";

            if(flag == 0){
                if(ImGui.button("Send")){
                    username = client.getUsernameChoiceContent().get();
                    message = client.getGameChoiceContent().get();
                    client.getUsernameChoiceContent().clear();
                    client.getGameChoiceContent().clear();
                    if(Utils.answerIsCorrectInput(username, 0)){
                        if(Utils.messageIsCorrect(message)){
                            client.getSender().send("SEND? " + username + " " + message+ "***");
                        }else{
                            client.addContentTologs("client: warning:", message, 1);
                            client.addContentTologs("client: warning:",
                        "Wrong format of message (between 0 to 200 characters)", 1);
                        }
                    }else{
                        client.addContentTologs("client: warning:", username, 1);
                        client.addContentTologs("client: warning:",
                        "your username does'nt respect the format (8 characters)", 1);
                        client.backToPreviousScene();
                    }
                }
            }else{
                if(ImGui.button("Send")){
                    message = client.getGameChoiceContent().get();
                    client.getGameChoiceContent().clear();
                    if(Utils.messageIsCorrect(message)){
                        client.getSender().send("MALL? " + message + "***");
                    }else{
                        client.addContentTologs("client: warning:", message, 1);
                        client.addContentTologs("client: warning:",
                    "Wrong format of message (between 0 to 200 characters)", 1);
                        client.backToPreviousScene();
                    }
                } 
            }

            ImGui.sameLine();

            if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
                client.getUsernameChoiceContent().clear();
                client.getGameChoiceContent().clear();
                client.backToPreviousScene();
            }
        }
        ImGui.endPopup();
    }

    /**
     * Display send message in game popup.
     * @param type Type of popup.
     * @param move Type of move request.
     * @param toward Direction of move.
     */
    public void nbMoveInGamePopupDraw(String type, String move, int toward) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {
            ImGui.text("Number of move:");
            ImGui.sameLine();
            ImGui.inputText("##MazeMoveChoice", client.getUsernameChoiceContent());
            ImGui.sameLine();
            String mazeMoveChoice = client.getUsernameChoiceContent().get();

            if(ImGui.button("Send")){
                if(Utils.answerIsCorrectInput(mazeMoveChoice, 3)){
                    client.addContentTologs("client: info: taille :", String.valueOf(mazeMoveChoice.length()), 1);
                    int size = mazeMoveChoice.length();
                    for(int i = 0; i < 3 - size; i++){
                        mazeMoveChoice = "0" + mazeMoveChoice;
                    }
                    client.addContentTologs("client: info:", mazeMoveChoice, 1);
                    client.getGameModel().setWantedPos(toward, Integer.parseInt(mazeMoveChoice));
                    client.getSender().send(move + " " + mazeMoveChoice + "***");
                }else{
                    client.addContentTologs("client: warning:", "move : " + move +
                     ", number of move : " + mazeMoveChoice, 1);
                    client.addContentTologs("client: warning:",
                     "Wrong choice of move, you must choose an value between 0 to 999", 1);
                    client.backToPreviousScene(); 
                }
                client.getUsernameChoiceContent().clear();
            }

            ImGui.sameLine();

            if (ImGui.button("Back")) {
                client.getUsernameChoiceContent().clear();
                client.backToPreviousScene();
            }
        }
        ImGui.endPopup();
    }

    /**To display the popup. */
    public void userChoice() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove |
        ImGuiWindowFlags.NoCollapse);

        if(client.getLastPressedButton() == SceneData.BUTTON_CREATEGAME){
            joinOrCreateGamePopupDraw("CreateGame");
        }else if(client.getLastPressedButton() == SceneData.BUTTON_JOINGAME){
            joinOrCreateGamePopupDraw("JoinGame");
        }else if(client.getLastPressedButton() == SceneData.BUTTON_PRIVATEMESSAGE){
            sendMessageInGamePopupDraw("Send Private Message", 0);
        }else if(client.getLastPressedButton() == SceneData.BUTTON_ALLMESSAGE){
            sendMessageInGamePopupDraw("Send Everyone Message", 1);
        }else if(client.getLastPressedButton() == SceneData.BUTTON_MAZEMOVE_TOWARD_UP){
            nbMoveInGamePopupDraw("Number of move : ", "UPMOV", client.getLastPressedButton());
        }else if(client.getLastPressedButton() == SceneData.BUTTON_MAZEMOVE_TOWARD_LEFT){
            nbMoveInGamePopupDraw("Number of move : ", "LEMOV", client.getLastPressedButton());
        }else if(client.getLastPressedButton() == SceneData.BUTTON_MAZEMOVE_TOWARD_RIGHT){
            nbMoveInGamePopupDraw("Number of move : ", "RIMOV", client.getLastPressedButton());
        }else if(client.getLastPressedButton() == SceneData.BUTTON_MAZEMOVE_TOWARD_DOWN){
            nbMoveInGamePopupDraw("Number of move : ", "DOMOV", client.getLastPressedButton());
        }
        ImGui.end();
    }

    /**
     * Display user choice.
     * @throws IOException
     */
    public void display() throws IOException{
        userChoice();
        mainContainer();
    }
    
}
