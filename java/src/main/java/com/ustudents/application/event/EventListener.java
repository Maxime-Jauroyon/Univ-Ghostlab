package com.ustudents.application.event;

public interface EventListener<T extends Event> {
    public void onReceived(Class<?> dataType, T data);
}
