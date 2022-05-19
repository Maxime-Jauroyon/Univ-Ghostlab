package com.ustudents.ghostlab.other;

public class Player {
    
    private final String username;
    private final String posX;
    private final String posY;
    private final String score;


    public Player(String username, String posX, String posY, String score){
        this.username = username;
        this.posX = posX;
        this.posY = posY;
        this.score = score;
    }

    public String getUsername(){
        return username;
    }

    public String getPosX(){
        return posX;
    }

    public String getPosY(){
        return posY;
    }

    public String getScore(){
        return score;
    }
}
