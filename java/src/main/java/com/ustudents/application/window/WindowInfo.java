package com.ustudents.application.window;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/** Annotation interface for a command. */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface WindowInfo {
    /** Name of the command. */
    String title();

    int width();

    int height();
}