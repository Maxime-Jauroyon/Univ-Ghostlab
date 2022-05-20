package com.ustudents.ghostlab.scene;

import com.ustudents.application.graphic.ImGuiManager;
import com.ustudents.ghostlab.client.Client;

import org.lwjgl.glfw.GLFW;

import imgui.ImGui;
import imgui.ImGuiListClipper;
import imgui.callback.ImListClipperCallback;
import imgui.flag.ImGuiWindowFlags;
import static java.lang.System.exit;

import java.io.IOException;
import java.util.List;

public abstract class Scene {

    protected final Client client;

    public Scene(Client client){
        this.client = client;
    }

    /**
     * To display the main container.
     */
    protected void mainContainer() throws IOException{
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.begin("Ghostlab Client", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);

        if (ImGui.beginMenuBar()) {
            if (ImGui.beginMenu("File")) {
                if (ImGui.menuItem("Quit")) {
                    exit(0);
                }
                ImGui.endMenu();
            }
            ImGui.endMenuBar();
        }

        ImGui.end();

        ImGui.setNextWindowPos(0, ImGui.getIO().getDisplaySizeY() * 0.6f);
        ImGui.setNextWindowSize(ImGui.getIO().getDisplaySizeX(), ImGui.getIO().getDisplaySizeY() * 0.4f);
        ImGui.begin("Console", ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar);

        if (ImGui.beginMenuBar()) {
            if (ImGui.beginMenu("View")) {
                if (ImGui.menuItem("Clear Logs")) {
                    client.getLogs().clear();
                }

                ImGui.separator();

                ImGui.menuItem("Show Info", null, client.getConsoleShowInfo());
                ImGui.menuItem("Show Warning", null, client.getConsoleShowWarning());
                ImGui.menuItem("Show Error", null, client.getConsoleShowError());

                ImGui.endMenu();
            }

            ImGui.endMenuBar();
        }

        ImGui.text("Enter a command:");
        ImGui.sameLine();
        ImGui.inputText("##Command", client.getConsoleCommand());
        ImGui.sameLine();

        if ((ImGui.button("Send")) || ImGui.isKeyPressed(GLFW.GLFW_KEY_ENTER)) {
            String command = client.getConsoleCommand().get();
            client.addContentTologs("$", command, 1);
            client.getConsoleCommand().clear();
            if(command.equals("q") || command.equals("e") ||
               command.equals("quit") || command.equals("exit")){
                exit(0);
            }else if(command.equals("h") || command.equals("help")){
                client.helpcommand();
            }else if(command.equals("v") || command.equals("version")){
                client.addContentTologs("client: info:", "version : 1.0.0", 1);
            }else{
                client.addContentTologs("client: warning:", "invalid option `" + command + "`!", 1);
                client.addContentTologs("client: warning:", "use `h` for more informations.", 1);
            }
        }

        ImGui.beginChild("##Logs", 0, 0, true, ImGuiWindowFlags.HorizontalScrollbar);
        ImGui.pushFont(ImGuiManager.firaCode);

        List<String> visibleLogs = client.getVisibleLog();
        ImGuiListClipper.forEach(visibleLogs.size()/*client.getLogs().size()*/, new ImListClipperCallback() {
            public void accept(int i) {
                String command = visibleLogs.get(i);
                if(command.startsWith("client: warning:")) 
                    ImGui.textColored(215, 215, 0, 255, command);
                    else if(command.startsWith("client: error:"))
                        ImGui.textColored(255, 0, 0, 255, command);   
                    else if(command.startsWith("$"))
                        ImGui.textColored(57, 255, 20, 255, command); 
                    else
                        ImGui.textUnformatted(command);
            }
        });

        if (client.getConsoleShouldScroll().get()) {
            ImGui.setScrollHereX(1.0f);
            ImGui.setScrollHereY(1.0f);
            client.getConsoleShouldScroll().set(false);
        }

        ImGui.popFont();
        ImGui.endChild();
        ImGui.end();

    }
    
}
