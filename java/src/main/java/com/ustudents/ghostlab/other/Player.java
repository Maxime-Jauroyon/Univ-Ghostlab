package com.ustudents.ghostlab.other;

public class Player {
    
    private final String username;
    private String posX;
    private String posY;
    private String score;


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

    public void setPosX(String posX){
        this.posX = posX;
    }

    public void setPosY(String posY){
        this.posY = posY;
    }

    public void setScore(String score){
        this.score = score;
    }
    
}
