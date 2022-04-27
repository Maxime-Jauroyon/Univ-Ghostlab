package com.ustudents.ghostlab.other;

import java.util.ArrayList;
import java.util.List;

public class GameModel {

    public static final char MAZE_UNKNOWN = '?';
    public static final char MAZE_PLAYER = 'P';
    public static final char MAZE_GROUND = '.';
    public static final char MAZE_WALL = '#';

    public static final int TOWARD_UNKNOWN = -1;
    public static final int TOWARD_UP = 0;
    public static final int TOWARD_DOWN = 1;
    public static final int TOWARD_LEFT = 2;
    public static final int TOWARD_RIGHT = 3;
    
    private int playerScore;
    private int[] wantedPos;
    private final List<int[]> pos = new ArrayList<>();
    private char[][] maze;

    public int[] getLastPos(){
        return pos.get(pos.size() - 1);
    }

    public char[][] getMaze(){
        return maze;
    }

    public void setScore(int score){
        this.playerScore = score;
    }

    public void setWantedPos(int toward, int value){
        int[] oldpos = getLastPos();
        if(toward == TOWARD_UP){
            wantedPos = new int[]{oldpos[0], oldpos[1]-value};
        }else if(toward == TOWARD_DOWN){
            wantedPos = new int[]{oldpos[0], oldpos[1]+value};
        }else if(toward == TOWARD_LEFT){
            wantedPos = new int[]{oldpos[0]-value, oldpos[1]};
        }else if(toward == TOWARD_RIGHT){
            wantedPos = new int[]{oldpos[0]+value, oldpos[1]};  
        }
    }

    public void setNewPos(int posX, int posY){
        pos.add(new int[]{posX, posY});
        maze[posY][posX] = MAZE_PLAYER;
    }

    public void setMaze(int height, int width){
        pos.clear();
        maze = new char[height][width];
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                maze[i][j] = MAZE_UNKNOWN; 
            }
        }
    }

    private boolean lastPosIsWantedPos(int[] newPos, int[] wantedPos){
        return newPos[0] == wantedPos[0] && newPos[1] == wantedPos[1];
    }

    private void attributeNewChar(int[] oldPos, int toward, int index, int flag){
        if(toward == TOWARD_UP){
            if(flag == 0){
                maze[oldPos[1]-index][oldPos[0]] = MAZE_GROUND;
            }else{
                maze[oldPos[1]-index][oldPos[0]] = MAZE_WALL;
            }
            
        }else if(toward == TOWARD_DOWN){
            if(flag == 0){
                maze[oldPos[1]+index][oldPos[0]] = MAZE_GROUND;
            }else{
                maze[oldPos[1]+index][oldPos[0]] = MAZE_WALL;
            }
            
            
        }else if(toward == TOWARD_LEFT){
            if(flag == 0){
                maze[oldPos[1]][oldPos[0]-index] = MAZE_GROUND;
            }else{
                maze[oldPos[1]][oldPos[0]-index] = MAZE_WALL;
            }
            
        }else if(toward == TOWARD_RIGHT){
            if(flag == 0){
                maze[oldPos[1]][oldPos[0]+index] = MAZE_GROUND;
            }else{
                maze[oldPos[1]][oldPos[0]+index] = MAZE_WALL;
            }
        }
    }

    public void updateMaze(int[] newPos){
        int[] oldPos = getLastPos();
        pos.add(newPos);
        maze[oldPos[1]][oldPos[0]] = MAZE_GROUND;
        maze[newPos[1]][newPos[0]] = MAZE_PLAYER;
        int ydiff = oldPos[1] - wantedPos[1];
        int xdiff = oldPos[0] - wantedPos[0];
        int toward;
        int diff;
        if(xdiff > 0 && ydiff == 0){
            toward = TOWARD_LEFT;
            diff = Math.abs(oldPos[0] - newPos[0]);
        }else if(xdiff < 0 && ydiff == 0){
            toward = TOWARD_RIGHT;
            diff = Math.abs(oldPos[0] - newPos[0]);
        }else if(xdiff == 0 && ydiff > 0){
            toward = TOWARD_UP;
            diff = Math.abs(oldPos[1] - newPos[1]);
        }else if(xdiff == 0 && ydiff < 0){
            toward = TOWARD_DOWN;
            diff = Math.abs(oldPos[1] - newPos[1]);
        }else{
            toward = TOWARD_UNKNOWN;
            diff = 0;
        }
        
        for(int i = 0; i < diff; i++){
            attributeNewChar(oldPos, toward, i, 0);
        }

        if(!lastPosIsWantedPos(newPos, wantedPos) && lastPosIsWantedPos(oldPos, newPos)){
            attributeNewChar(oldPos, toward, diff+1, 1);
        }else if(!lastPosIsWantedPos(newPos, wantedPos)){
            attributeNewChar(oldPos, toward, diff+1, 1);
        }
    }
}
