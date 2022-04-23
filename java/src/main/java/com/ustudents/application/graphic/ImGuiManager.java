package com.ustudents.application.graphic;

import com.ustudents.common.utils.Resources;
import imgui.*;
import imgui.flag.ImGuiConfigFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;

import static org.lwjgl.glfw.GLFW.glfwGetCurrentContext;
import static org.lwjgl.glfw.GLFW.glfwMakeContextCurrent;

public class ImGuiManager {
    /** The ImGui GLFW implementation. */
    private final ImGuiImplGlfw imGuiGlfw = new ImGuiImplGlfw();

    /** The ImGui OpenGL 3.X implementation. */
    private final ImGuiImplGl3 imGuiGl3 = new ImGuiImplGl3();

    public static ImFont sourceSansPro;

    public static ImFont firaCode;

    public void initialize(long windowHandle, String glslVersion, boolean enableDocking) {
        ImGui.createContext();

        final ImGuiIO io = ImGui.getIO();
        io.setIniFilename(null);

        if (enableDocking) {
            io.addConfigFlags(ImGuiConfigFlags.DockingEnable);
        }

        io.getFonts().addFontDefault();

        final ImFontConfig fontConfig = new ImFontConfig();
        fontConfig.setRasterizerMultiply(1.5f);
        fontConfig.setOversampleH(4);
        fontConfig.setOversampleV(4);

        sourceSansPro = io.getFonts().addFontFromMemoryTTF(Resources.load("/fonts/SourceSansPro/SourceSansPro-Regular.ttf"), 16.0f, fontConfig);
        firaCode = io.getFonts().addFontFromMemoryTTF(Resources.load("/fonts/FiraCode/FiraCode-Regular.ttf"), 16.0f, fontConfig);
        io.getFonts().build();

        io.setFontDefault(sourceSansPro);

        fontConfig.destroy();

        //io.addConfigFlags(ImGuiConfigFlags.ViewportsEnable);
        io.setConfigViewportsNoTaskBarIcon(true);
        imGuiGlfw.init(windowHandle, false);
        imGuiGl3.init(glslVersion);
    }

    public void startFrame() {
        imGuiGlfw.newFrame();
        ImGui.newFrame();
    }

    public void endFrame() {
        ImGui.render();
        imGuiGl3.renderDrawData(ImGui.getDrawData());

        if (ImGui.getIO().hasConfigFlags(ImGuiConfigFlags.ViewportsEnable)) {
            final long backupWindowPtr = glfwGetCurrentContext();
            ImGui.updatePlatformWindows();
            ImGui.renderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupWindowPtr);
        }
    }

    public void destroy() {
        imGuiGl3.dispose();
        imGuiGlfw.dispose();

        ImGui.destroyContext();
    }

    public ImGuiImplGlfw getImGuiGlfw() {
        return imGuiGlfw;
    }

    public ImGuiImplGl3 getImGuiGl3() {
        return imGuiGl3;
    }
}
