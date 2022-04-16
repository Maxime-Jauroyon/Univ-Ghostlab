package com.ustudents.application;

import com.ustudents.application.window.WindowInfo;
import com.ustudents.application.window.glfw.GLFWWindow;
import com.ustudents.common.Program;
import com.ustudents.application.input.InputSystemType;
import com.ustudents.application.window.WindowSystemType;
import com.ustudents.application.graphic.Viewport;
import com.ustudents.application.graphic.ImGuiManager;
import com.ustudents.application.input.Input;
import com.ustudents.application.math.Vector2i;
import com.ustudents.common.utils.Timer;
import com.ustudents.application.window.Window;

/** The main class of the project. */
@SuppressWarnings("unused")
public abstract class Application extends Program {
    /** The window manager. */
    protected final Window window = new Window();

    /** The ImGui manager (handle most debugging tools). */
    protected final ImGuiManager imGuiManager = new ImGuiManager();

    /** The timer (handle delta time management). */
    protected final Timer timer = new Timer();

    /** Defines if we should quit the game. */
    protected boolean shouldQuit = false;

    protected boolean vsync = true;

    /** Defines if we should resize at the next frame. */
    protected boolean shouldResize = true;

    /** The name of the game instance. */
    protected static String instanceName = "game";

    /** The game instance. */
    private static Application application;

    /** Class constructor. */
    public Application() {
        application = this;
    }

    /**
     * Run the core of the project.
     *
     * @param args The arguments.
     * @return the exit code.
     */
    @Override
    protected int main(String[] args) {
        if (!showHelp && !showVersion) {
            initializeInternals(args);
            startApplicationLoop();
            destroyInternals();
        }

        return 0;
    }

    /** Quits the game. */
    public void quit() {
        shouldQuit = true;
    }

    /** Force a resize of the game render viewport and camera matrices before the next frame. */
    public void forceResizeBeforeNextFrame() {
        shouldResize = true;
    }

    /** @return the game. */
    public static Application get() {
        return application;
    }

    /** @return the instance's name. */
    public static String getInstanceName() {
        return instanceName;
    }

    /** @return the window. */
    public Window getWindow() {
        return window;
    }

    /** @return the ImGui manager. */
    public ImGuiManager getImGuiManager() {
        return imGuiManager;
    }

    /** @return the timer. */
    public Timer getTimer() {
        return timer;
    }

    /** @return if the V-Sync is enabled. */
    public boolean getVsync() {
        return vsync;
    }

    public WindowSystemType getWindowSystemType() {
        return WindowSystemType.GLFW;
    }

    public InputSystemType getInputSystemType() {
        return InputSystemType.GLFW;
    }


    public boolean shouldQuit() {
        return window.shouldQuit() || shouldQuit;
    }

    /**
     * Changes the V-Sync state.
     *
     * @param vsync The new V-Sync state.
     */
    public void setVsync(boolean vsync) {
        window.setVsync(vsync);
    }

    /** Initialize the game. */
    protected void initialize() {

    }

    /** Updates the game's logic. */
    protected void update(float dt) {

    }

    /** Renders the game on the screen. */
    protected void render() {

    }

    /** Renders ImGui content. */
    protected void renderImGui() {

    }

    /** Destroys the game's data. */
    protected void destroy() {

    }

    /** Initialize everything. */
    public void initializeInternals(String[] args) {
        String title = "Application";
        int width = 1280;
        int height = 720;
        if (getClass().isAnnotationPresent(WindowInfo.class)) {
            title = getClass().getAnnotation(WindowInfo.class).title();
            width = getClass().getAnnotation(WindowInfo.class).width();
            height = getClass().getAnnotation(WindowInfo.class).height();
        }
        window.initialize(title, new Vector2i(width, height), vsync);
        imGuiManager.initialize(window.getHandle(), ((GLFWWindow)window.getWindow()).getGlslVersion(), false);
        Input.initialize();
        initialize();
        window.show(true);
        window.switchType(Window.Type.Windowed);
    }

    /** Starts the game loop. */
    public void startApplicationLoop() {
        window.pollEvents();

        while (!shouldQuit()) {
            updateInternal();
            renderInternal();
            window.pollEvents();
        }
    }

    /** Updates the game logic. */
    private void updateInternal() {
        timer.update();
        float dt = timer.getDeltaTime();
        update(dt);
        Input.update(dt);
    }

    /** Renders the game. */
    private void renderInternal() {
        timer.render();
        window.clear();
        Viewport.resize(window.getSize());
        imGuiManager.startFrame();

        render();
        renderImGui();

        imGuiManager.endFrame();
        window.swap();
    }

    /** Destroy everything. */
    public void destroyInternals() {
        destroy();
        window.show(false);
        imGuiManager.destroy();
        window.destroy();
    }
}
