import org.gradle.internal.os.OperatingSystem

group = "com.ustudents"
version = "1.0"

plugins {
    java
    application
    id("com.github.johnrengelman.shadow").version("7.1.2")
}

repositories {
    mavenCentral()
}

java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
}

dependencies {
    val os: OperatingSystem = OperatingSystem.current()
    val lwjglVersion: String by project
    val imGuiVersion: String by project

    var platform = ""
    when {
        os.isWindows -> {
            platform = "natives-windows"
        }
        os.isLinux -> {
            platform = "natives-linux"
        }
        os.isMacOsX -> {
            platform = "natives-macos"
        }
    }

    implementation(platform("org.lwjgl:lwjgl-bom:$lwjglVersion"))
    implementation("org.lwjgl:lwjgl:$lwjglVersion")
    implementation("org.lwjgl:lwjgl-glfw:$lwjglVersion")
    implementation("org.lwjgl:lwjgl-openal:$lwjglVersion")
    implementation("org.lwjgl:lwjgl-opengl:$lwjglVersion")
    implementation("org.lwjgl:lwjgl-stb:$lwjglVersion")
    runtimeOnly("org.lwjgl:lwjgl::$platform")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$platform")
    runtimeOnly("org.lwjgl:lwjgl-openal::$platform")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$platform")
    runtimeOnly("org.lwjgl:lwjgl-stb::$platform")

    implementation("io.github.spair:imgui-java-binding:$imGuiVersion")
    implementation("io.github.spair:imgui-java-lwjgl3:$imGuiVersion")
    implementation("io.github.spair:imgui-java-$platform:$imGuiVersion")
}

tasks {
    compileJava {
        options.encoding = "UTF-8"
    }
    compileTestJava {
        options.encoding = "UTF-8"
    }

    jar {
        manifest {
            attributes("Main-Class" to "com.ustudents.ghostlab.Main")
        }
    }
}

// Set main class.
application.mainClass.set("com.ustudents.ghostlab.Main")

// Changes the standard input (useful because Gradle can hide the input in some cases).
val run by tasks.getting(JavaExec::class) {
    standardInput = System.`in`
}

// List of JVM options to pass.
var jvmOptions = mutableListOf<String>()
// Gradle has an issue supporting unicode within Powershell or cmd.exe,
// you need to use `chcp 65001` to enable unicode characters
// (this is not an issue in distributed builds, only within gradle commands output).
jvmOptions.add("-Dfile.encoding=utf-8")
// Pass an IDE name information to know within the engine's code if we are debugging within an IDE.
if (project.gradle.startParameter.taskNames.contains("run") && System.getProperty("idea.vendor.name") == "JetBrains") {
    jvmOptions.add("-Dide=JetBrains")
}
// Needed by LWJGL to create the GLFW window.
if (OperatingSystem.current().isMacOsX) {
    jvmOptions.add("-XstartOnFirstThread")
}
application.applicationDefaultJvmArgs = jvmOptions
