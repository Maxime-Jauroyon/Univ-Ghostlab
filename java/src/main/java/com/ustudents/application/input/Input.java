package com.ustudents.application.input;

import com.ustudents.application.Application;
import com.ustudents.application.event.EventListener;
import com.ustudents.application.window.events.KeyStateChangedEvent;
import com.ustudents.application.window.events.MouseButtonStateChangedEvent;
import com.ustudents.application.input.empty.EmptyInput;
import com.ustudents.application.input.glfw.GLFWInput;
import com.ustudents.application.math.Vector2f;
import com.ustudents.application.math.Vector4f;

public class Input {
    private static EmptyInput inputManager;

    public static void initialize() {
        switch (Application.get().getInputSystemType()) {
            case Empty:
                inputManager = new EmptyInput();
                break;
            case GLFW:
                inputManager = new GLFWInput();
                break;
        }

        inputManager.initialize();
    }

    public static void update(float dt) {
        inputManager.update(dt);
    }

    public static boolean isKeyDown(int key) {
        return inputManager.isKeyDown(key);
    }

    public static boolean isKeyUp(int key) {
        return inputManager.isKeyUp(key);
    }

    public static boolean isKeyPressed(int key) {
        return inputManager.isKeyPressed(key);
    }

    public static boolean isKeyReleased(int key) {
        return inputManager.isKeyReleased(key);
    }

    public static boolean isMouseDown(int button) {
        return inputManager.isMouseDown(button);
    }

    public static boolean isMouseUp(int key) {
        return inputManager.isMouseUp(key);
    }

    public static boolean isMousePressed(int button) {
        return inputManager.isMousePressed(button);
    }

    public static boolean isMouseRelease(int button) {
        return inputManager.isMouseRelease(button);
    }

    public static int getScroll() {
        return inputManager.getScroll();
    }

    public static Vector2f getMousePos() {
        return inputManager.getMousePos();
    }

    public static boolean isMouseInViewRect(Vector4f viewRect) {
        return inputManager.isMouseInViewRect(viewRect);
    }

    public static void recalculateMousePosition() {
        inputManager.recalculateMousePosition();
    }

    public static boolean isActionSuccessful(String action){
        return false;
    }

    public static int findKey(){
        return ((GLFWInput)inputManager).findKey();
    }

    public static boolean actionExists(String name) {
        return false;
    }

    public static void resetKeyAndButton() {
        inputManager.resetKeyAndButton();
    }

    public static void stopInputHandling() {
        inputManager.stopInputHandling();
    }

    public static void addKeyStateChangedListener(EventListener<KeyStateChangedEvent> listener, String name) {
        inputManager.addKeyStateChangedListener(listener, name);
    }

    public static void addMouseButtonStateChangedListener(EventListener<MouseButtonStateChangedEvent> listener, String name) {
        inputManager.addMouseButtonStateChangedListener(listener, name);
    }

    public static void removeKeyStateChangedListener(String name) {
        inputManager.removeKeyStateChangedListener(name);
    }

    public static void removeMouseButtonStateChangedListener(String name) {
        inputManager.removeMouseButtonStateChangedListener(name);
    }
}
