package com.ustudents.application.window.events;

import com.ustudents.application.event.Event;

public class KeyStateChangedEvent extends Event {
    public int key;
    public int scancode;
    public int action;
    public int mods;

    public KeyStateChangedEvent(int key, int scancode, int action, int mods) {
        this.key = key;
        this.scancode = scancode;
        this.action = action;
        this.mods = mods;
    }
}