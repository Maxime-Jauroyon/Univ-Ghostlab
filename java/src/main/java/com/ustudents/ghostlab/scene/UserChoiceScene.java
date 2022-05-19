package com.ustudents.ghostlab.scene;

import java.io.IOException;

import com.ustudents.ghostlab.client.Client;
import com.ustudents.ghostlab.other.GameModel;
import com.ustudents.ghostlab.other.Utils;

import org.lwjgl.glfw.GLFW;

import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public class UserChoiceScene extends Scene {
    
    

    public UserChoiceScene(Client client){
        super(client);
    }

    /*private void createGameSection() throws IOException{
        ImGui.text("Enter a username:");
        ImGui.sameLine();
        ImGui.inputText("##UsernameChoice", client.getUsernameChoiceContent());
        ImGui.sameLine();

        if (ImGui.button("Use it")) {

            String username = client.getUsernameChoiceContent().get();
            if(Utils.answerIsCorrectInput(username, 0)){
                client.setUsername(username);
                client.addContentTologs("client: info:", "your username are : " + username, 1);
                client.getSender().send("NEWPL " + username + " " + client.getUdpPort() + "***");
            }else{
                client.addContentTologs("client: warning:", username, 1);
                client.addContentTologs("client: warning:",
                 "your username does'nt respect the format (8 characters)", 1);
            }

        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.backToPreviousScene();
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
            client.getGameChoiceContent().clear();
            if(Utils.answerIsCorrectInput(username, 0)){
                client.setUsername(username);
                client.addContentTologs("client: info:", "your username are : " + username, 1);
                if(gameId.length() > 0 && Utils.answerIsCorrectInput(gameId, 1)){
                    client.setGameRegister(gameId);
                    client.addContentTologs("client: info:", "choosed game : " + gameId, 1);
                    client.getSender().send("REGIS " + username + " " + client.getUdpPort()
                     + " " + gameId + "***");
                }else{
                    client.setUsername(null);
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
            client.backToPreviousScene();
        }
    }

    private void specificGameInfoSection(int flag) throws IOException{
        ImGui.text("Enter a game id:");
        ImGui.sameLine();
        ImGui.inputText("##GameChoice", client.getGameChoiceContent());

        if (ImGui.button("Use it")) {
            String gameId = client.getGameChoiceContent().get();
            client.getGameChoiceContent().clear();
            if(gameId.length() > 0 && Utils.answerIsCorrectInput(gameId, 1)){
                client.setGameRegister(gameId);
                client.addContentTologs("client: info:", "choosed game : " + gameId, 1);
                if(flag == 0){
                    client.getSender().send("SIZE? " + gameId + "***");
                }else{
                    client.getSender().send("LIST? " + gameId + "***");
                }
            }else{
                client.addContentTologs("client: warning:", gameId, 1);
                client.addContentTologs("client: warning:",
             "your game choice does'nt respect the format (between 0 to 255)", 1);
            }
        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.getGameChoiceContent().clear();
            client.backToPreviousScene();
        }
    }

    private void moveSend(int toward, String move, String mazeMoveChoice) throws IOException{
        if(Utils.answerIsCorrectInput(mazeMoveChoice, 1)){
            client.addContentTologs("client: info: taille :", String.valueOf(mazeMoveChoice.length()), 1);
            int size = mazeMoveChoice.length();
            for(int i = 0; i < 3 - size; i++){
                mazeMoveChoice = "0" + mazeMoveChoice;
            }
            client.addContentTologs("client: info:", mazeMoveChoice, 1);
            client.getGameModel().setWantedPos(toward, Integer.parseInt(mazeMoveChoice));
            client.getSender().send(move + " " + mazeMoveChoice + "***");
        }else{
            client.addContentTologs("client: warning:", ", move : " + move +
             ", number of move : " + mazeMoveChoice, 1);
            client.addContentTologs("client: warning:",
             "Wrong choice of move", 1);
        }
    }

    public void mazeMove() throws IOException{
        ImGui.text("Number of move:");
        ImGui.sameLine();
        ImGui.inputText("##MazeMoveChoice", client.getUsernameChoiceContent());
        String mazeMoveChoice = client.getUsernameChoiceContent().get();

        if (ImGui.button("Up")){
            moveSend(GameModel.TOWARD_UP, "UPMOV", mazeMoveChoice);
        }

        ImGui.sameLine();

        if (ImGui.button("Down") && Utils.answerIsCorrectInput(mazeMoveChoice, 1)) {
            moveSend(GameModel.TOWARD_DOWN, "DOMOV", mazeMoveChoice);
        }

        ImGui.sameLine();

        if (ImGui.button("Left") && Utils.answerIsCorrectInput(mazeMoveChoice, 1)) {
            moveSend(GameModel.TOWARD_LEFT, "LEMOV", mazeMoveChoice);
        }

        ImGui.sameLine();

        if (ImGui.button("Right") && Utils.answerIsCorrectInput(mazeMoveChoice, 1)) {
            moveSend(GameModel.TOWARD_RIGHT, "RIMOV", mazeMoveChoice);
        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.getGameChoiceContent().clear();
            client.backToPreviousScene();
        }
    }

    public void sendMessage(int flag) throws IOException{
        String username = "";
        if(flag == 0){
            ImGui.text("Player to send:");
            ImGui.sameLine();
            ImGui.inputText("##player", client.getUsernameChoiceContent());
        }

        ImGui.text("Your Message:");
        ImGui.sameLine();
        ImGui.inputText("##Message", client.getGameChoiceContent());
        String message = "";

        if(flag == 0){
            if(ImGui.button("Send")){
                username = client.getUsernameChoiceContent().get();
                message = client.getGameChoiceContent().get();
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
                }
            }
        }else{
            if(ImGui.button("Send")){
                message = client.getGameChoiceContent().get();
                client.addContentTologs("client: info:", "sdfdfgdgdg", flag);
                if(Utils.messageIsCorrect(message)){
                    client.getSender().send("MALL? " + message + "***");
                }else{
                    client.addContentTologs("client: warning:", message, 1);
                    client.addContentTologs("client: warning:",
                "Wrong format of message (between 0 to 200 characters)", 1);
                }
            } 
        }

        ImGui.sameLine();

        if (ImGui.button("Back") || ImGui.isKeyPressed(GLFW.GLFW_KEY_ESCAPE)) {
            client.getUsernameChoiceContent().clear();
            client.getUsernameChoiceContent().clear();
            client.backToPreviousScene();
        }

    }*/  
    
    public void joinOrCreateGamePopupDraw(String type) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {
            ImGui.text("Choose a name:");
            ImGui.sameLine();
            ImGui.inputText("###PlayerName", client.getUsernameChoiceContent());
            ImGui.sameLine();

            if (ImGui.button("Back")) {
                client.backToPreviousScene();
            }
            
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

            
        }
        ImGui.endPopup();
    }

    public void sendMessageInGamePopupDraw(String type, int flag) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {
            String username = "";
            if(flag == 0){
                ImGui.text("Player to send:");
                ImGui.sameLine();
                ImGui.inputText("##player", client.getUsernameChoiceContent());
            }

            ImGui.text("Your Message:");
            ImGui.sameLine();
            ImGui.inputText("##Message", client.getGameChoiceContent());
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

    public void nbMoveInGamePopupDraw(String type, String move, int toward) throws IOException{
        ImGui.openPopup("###" + type, 0);

        if (ImGui.beginPopupModal(type + "###" + type, ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoMove)) {
            ImGui.text("Number of move:");
            ImGui.sameLine();
            ImGui.inputText("##MazeMoveChoice", client.getUsernameChoiceContent());
            String mazeMoveChoice = client.getUsernameChoiceContent().get();
            
            if (ImGui.button("Back")) {
                client.getUsernameChoiceContent().clear();
                client.backToPreviousScene();
            }
            
            ImGui.sameLine();

            if(ImGui.button("Send")){
                if(Utils.answerIsCorrectInput(mazeMoveChoice, 1)){
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
                     "Wrong choice of move", 1);
                    client.backToPreviousScene(); 
                }
                client.getUsernameChoiceContent().clear();
            }

            
        }
        ImGui.endPopup();
    }

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

    public void display() throws IOException{
        userChoice();
        mainContainer();
    }
    
}
