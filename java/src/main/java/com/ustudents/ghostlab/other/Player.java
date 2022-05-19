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

    /**
     * @return Get the player username.
     */
    public String getUsername(){
        return username;
    }

    /**
     * @return Get the player position x.
     */
    public String getPosX(){
        return posX;
    }

    /**
     * @return Get the player position y.
     */
    public String getPosY(){
        return posY;
    }

    /**
     * @return Get the player score.
     */
    public String getScore(){
        return score;
    }

    /**
     * Set the player Position x.
     * @param posX Position x.
     */
    public void setPosX(String posX){
        this.posX = posX;
    }

    /**
     * Set the player Position y.
     * @param posX Position y.
     */
    public void setPosY(String posY){
        this.posY = posY;
    }

    /**
     * Set the player score.
     * @param posX Player score.
     */
    public void setScore(String score){
        this.score = score;
    }
    
}
