package com.ustudents.application.window.glfw;

import com.ustudents.common.log.Out;
import com.ustudents.application.Application;
import com.ustudents.application.window.Window;
import com.ustudents.application.window.empty.EmptyWindow;
import com.ustudents.application.window.events.*;
import com.ustudents.application.math.Vector2f;
import com.ustudents.application.math.Vector2i;
import imgui.ImGui;
import imgui.ImGuiIO;
import org.lwjgl.BufferUtils;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Objects;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.glfw.GLFW.glfwGetPrimaryMonitor;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL11.GL_COLOR_BUFFER_BIT;
import static org.lwjgl.stb.STBImage.stbi_load;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.NULL;

@SuppressWarnings("unchecked")
public class GLFWWindow extends EmptyWindow {
    private String name;

    private Vector2i size;

    private Window.Type type;

    private long windowHandle;

    private String glslVersion;

    @Override
    public void initialize(String name, Vector2i size, boolean vsync) {
        this.name = name;
        this.size = size;
        this.type = Window.Type.Windowed;
        this.glslVersion = "";

        glfwSetErrorCallback(new GLFWErrorCallback() {
            @Override
            public void invoke(int error, long description) {
                Out.println(error + ": " + description);
            }
        });

        if (!glfwInit()) {
            String errorMessage = "Unable to initialize glfw!";
            Out.printlnError(errorMessage);
            throw new IllegalStateException(errorMessage);
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, 0);
        glfwWindowHint(GLFW_RESIZABLE, 1);

        setGlslVersion();


        windowHandle = glfwCreateWindow(size.x, size.y, name, NULL, NULL);

        if (windowHandle == NULL) {
            String errorMessage = "Failed to create the glfw window!";
            Out.printlnError(errorMessage);
            throw new RuntimeException(errorMessage);
        }

        try (MemoryStack stack = stackPush()) {
            IntBuffer pWidth = stack.mallocInt(1);
            IntBuffer pHeight = stack.mallocInt(1);

            glfwGetWindowSize(windowHandle, pWidth, pHeight);

            GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            assert vidmode != null;

            glfwSetWindowPos(
                    windowHandle,
                    (vidmode.width() - pWidth.get(0)) / 2,
                    (vidmode.height() - pHeight.get(0)) / 2
            );
        }

        glfwMakeContextCurrent(windowHandle);
        setVsync(vsync);
        GL.createCapabilities();
        actualizeCursorType();
        setupCallbacks();
    }

    @Override
    public void clear() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    @Override
    public void swap() {
        glfwSwapBuffers(windowHandle);
    }

    @Override
    public void destroy() {
        glfwFreeCallbacks(windowHandle);
        glfwDestroyWindow(windowHandle);
        glfwTerminate();
        Objects.requireNonNull(glfwSetErrorCallback(null)).free();
    }

    @Override
    public void show(boolean show) {
        if (show) {
            glfwShowWindow(windowHandle);
            glfwFocusWindow(windowHandle);
        } else {
            glfwHideWindow(windowHandle);
        }
    }

    @Override
    public void actualizeCursorType() {

    }

    @Override
    public boolean shouldQuit() {
        return glfwWindowShouldClose(windowHandle);
    }

    @Override
    public void pollEvents() {
        glfwPollEvents();
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
        glfwSetWindowTitle(getHandle(), name);
    }

    @Override
    public Vector2i getSize() {
        return size;
    }

    @Override
    public void setSize(Vector2i size) {
        this.size = size;
    }

    @Override
    public long getHandle() {
        return windowHandle;
    }

    @Override
    public Vector2i getPosition() {
        IntBuffer x = BufferUtils.createIntBuffer(1);
        IntBuffer y = BufferUtils.createIntBuffer(1);

        glfwGetWindowPos(windowHandle, x, y);

        return new Vector2i(x.get(0), y.get(0));
    }

    public void setVsync(boolean enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    @Override
    public void changeIcon(String filePath) {
        ByteBuffer imageBuffer;
        int width, heigh;
        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer comp = stack.mallocInt(1);
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);

            imageBuffer = stbi_load(filePath, w, h, comp, 4);
            if (imageBuffer == null) {
                throw new IllegalStateException("Failed to load window icon.");
            }
            width = w.get();
            heigh = h.get();
        }
        GLFWImage image = GLFWImage.malloc(); GLFWImage.Buffer imagebf = GLFWImage.malloc(1);
        image.set(width, heigh, imageBuffer);
        imagebf.put(0, image);
        glfwSetWindowIcon(windowHandle, imagebf);
    }

    public String getGlslVersion() {
        return glslVersion;
    }

    private void setGlslVersion() {
        glslVersion = "#version 330 core";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    }

    private void resize(Vector2i size) {
        if (size.x > 0 && size.y > 0) {
            this.size = size;
            Application.get().forceResizeBeforeNextFrame();
        }
    }

    private void setupCallbacks() {
        glfwSetWindowSizeCallback(windowHandle, new GLFWWindowSizeCallback() {
            @Override
            public void invoke(long window, int width, int height) {
                resize(new Vector2i(width, height));
                sizeChanged.dispatch(new SizeChangedEvent(new Vector2i(width, height)));
            }
        });

        glfwSetKeyCallback(windowHandle, new GLFWKeyCallback() {
            @Override
            public void invoke(long window, int key, int scancode, int action, int mods) {
                Application.get().getImGuiManager().getImGuiGlfw().keyCallback(window, key, scancode, action, mods);

                final ImGuiIO io = ImGui.getIO();

                if (io.getWantTextInput()) {
                    return;
                }

                keyStateChanged.dispatch(new KeyStateChangedEvent(key, scancode, action, mods));
            }
        });

        glfwSetMouseButtonCallback(windowHandle, new GLFWMouseButtonCallback() {
            @Override
            public void invoke(long window, int button, int action, int mods) {
                Application.get().getImGuiManager().getImGuiGlfw().mouseButtonCallback(window, button, action, mods);

                final ImGuiIO io = ImGui.getIO();

                if (io.getWantCaptureMouse()) {
                    return;
                }

                mouseButtonStateChanged.dispatch(new MouseButtonStateChangedEvent(button, action, mods));
            }
        });

        glfwSetCursorPosCallback(windowHandle, new GLFWCursorPosCallback() {
            @Override
            public void invoke(long window, double xpos, double ypos) {
                cursorMoved.dispatch(new CursorMovedEvent(new Vector2f((float)xpos, (float)ypos)));
            }
        });

        glfwSetScrollCallback(windowHandle, new GLFWScrollCallback() {
            @Override
            public void invoke(long window, double xoffset, double yoffset) {
                Application.get().getImGuiManager().getImGuiGlfw().scrollCallback(window, xoffset, yoffset);

                final ImGuiIO io = ImGui.getIO();

                if (io.getWantCaptureMouse()) {
                    return;
                }

                scrollMoved.dispatch(new ScrollMovedEvent(new Vector2f((float)xoffset, (float)yoffset)));
            }
        });

        glfwSetCharCallback(windowHandle, new GLFWCharCallback() {
            @Override
            public void invoke(long window, int codepoint) {
                Application.get().getImGuiManager().getImGuiGlfw().charCallback(window, codepoint);
            }
        });

        glfwSetMonitorCallback(new GLFWMonitorCallback() {
            @Override
            public void invoke(long monitor, int event) {
                Application.get().getImGuiManager().getImGuiGlfw().monitorCallback(monitor, event);
            }
        });
    }

    @Override
    public void switchType(Window.Type type) {

    }
}
