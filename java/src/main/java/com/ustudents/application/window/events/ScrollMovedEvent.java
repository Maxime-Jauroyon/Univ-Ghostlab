package com.ustudents.application.window.events;

import com.ustudents.application.event.Event;
import com.ustudents.application.math.Vector2f;

public class ScrollMovedEvent extends Event {
    public Vector2f offets;

    public ScrollMovedEvent(Vector2f offets) {
        this.offets = offets;
    }
}