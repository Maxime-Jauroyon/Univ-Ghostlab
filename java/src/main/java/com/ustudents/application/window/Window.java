package com.ustudents.application.window;

import com.ustudents.application.Application;
import com.ustudents.application.event.EventDispatcher;
import com.ustudents.application.window.empty.EmptyWindow;
import com.ustudents.application.window.glfw.GLFWWindow;
import com.ustudents.application.math.Vector2i;

public class Window {
    public enum Type {
        Windowed,
        Fullscreen,
        Borderless
    }

    EmptyWindow windowManager;

    public void initialize(String name, Vector2i size, boolean vsync) {
        switch (Application.get().getWindowSystemType()) {
            case Empty:
                windowManager = new EmptyWindow();
                break;
            case GLFW:
                windowManager = new GLFWWindow();
                break;
        }

        windowManager.initialize(name, size, vsync);
    }

    public void clear() {
        windowManager.clear();
    }

    public void swap() {
        windowManager.swap();
    }

    public void destroy() {
        windowManager.destroy();
    }

    public void show(boolean show) {
        windowManager.show(show);
    }

    public boolean shouldQuit() {
        return windowManager.shouldQuit();
    }

    public void pollEvents() {
        windowManager.pollEvents();
    }

    public String getName() {
        return windowManager.getName();
    }

    public void setName(String name) {
        windowManager.setName(name);
    }

    public Vector2i getSize() {
        return windowManager.getSize();
    }

    public void setSize(Vector2i size) {
        windowManager.setSize(size);
    }

    public long getHandle() {
        return windowManager.getHandle();
    }

    public Vector2i getPosition() {
        return windowManager.getPosition();
    }

    public void setVsync(boolean enabled) {
        windowManager.setVsync(enabled);
    }

    public void changeIcon(String filePath) {
        windowManager.changeIcon(filePath);
    }

    public EventDispatcher getSizeChanged() {
        return windowManager.sizeChanged;
    }

    public EventDispatcher getKeyStateChanged() {
        return windowManager.keyStateChanged;
    }

    public EventDispatcher getMouseButtonStateChanged() {
        return windowManager.mouseButtonStateChanged;
    }

    public EventDispatcher getCursorMoved() {
        return windowManager.cursorMoved;
    }

    public EventDispatcher getScrollMoved() {
        return windowManager.scrollMoved;
    }

    public void switchType(Window.Type type) {
        windowManager.switchType(type);
    }

    public static Window get() {
        return Application.get().getWindow();
    }

    public EmptyWindow getWindow() {
        return windowManager;
    }

    public void chooseDefaultType() {
        switchType(Type.values()[0]);
    }
}
