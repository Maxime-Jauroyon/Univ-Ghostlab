package com.ustudents.application.window.events;

import com.ustudents.application.event.Event;
import com.ustudents.application.math.Vector2f;

public class CursorMovedEvent extends Event {
    public Vector2f position;

    public CursorMovedEvent(Vector2f position) {
        this.position = position;
    }
}