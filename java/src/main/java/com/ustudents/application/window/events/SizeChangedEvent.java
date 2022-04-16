package com.ustudents.application.window.events;

import com.ustudents.application.event.Event;
import com.ustudents.application.math.Vector2i;

public class SizeChangedEvent extends Event {
    public Vector2i size;

    public SizeChangedEvent(Vector2i size) {
        this.size = size;
    }
}