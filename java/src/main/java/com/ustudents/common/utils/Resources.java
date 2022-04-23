package com.ustudents.common.utils;

import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Objects;

public class Resources {
    public static byte[] load(String name) {
        try {
            return Files.readAllBytes(Paths.get(Objects.requireNonNull(Resources.class.getResource(name)).toURI()));
        } catch (IOException | URISyntaxException e) {
            throw new RuntimeException(e);
        }
    }
}
