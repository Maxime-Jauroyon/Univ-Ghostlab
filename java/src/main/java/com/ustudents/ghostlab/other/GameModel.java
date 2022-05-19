package com.ustudents.ghostlab.other;

import java.util.ArrayList;
import java.util.List;

import com.ustudents.ghostlab.scene.SceneData;

public class GameModel {

    /**All type of maze char. */
    public static final char MAZE_UNKNOWN = '?';
    public static final char MAZE_PLAYER = 'P';
    public static final char MAZE_GROUND = '.';
    public static final char MAZE_WALL = '#';

    /**All type of direction. */
    public static final int TOWARD_UNKNOWN = -1;
    public static final int TOWARD_UP = 0;
    public static final int TOWARD_DOWN = 1;
    public static final int TOWARD_LEFT = 2;
    public static final int TOWARD_RIGHT = 3;

    private int[] wantedPos;
    private final List<int[]> pos = new ArrayList<>();
    private char[][] maze;
    private final List<Player> players = new ArrayList<>();

    /**
     * @return Get last pos.
     */
    public int[] getLastPos(){
        return pos.get(pos.size() - 1);
    }

    /**
     * @return Get maze.
     */
    public char[][] getMaze(){
        return maze;
    }

    /**
     * @return Get players.
     */
    public List<Player> players(){
        return players;
    }

    /**
     * Set wanted position.
     * @param toward Toward a next position.
     * @param value distance to the next position.
     */
    public void setWantedPos(int toward, int value){
        int[] oldpos = getLastPos();
        if(toward == SceneData.BUTTON_MAZEMOVE_TOWARD_UP){
            wantedPos = new int[]{oldpos[0], oldpos[1]-value};
        }else if(toward == SceneData.BUTTON_MAZEMOVE_TOWARD_DOWN){
            wantedPos = new int[]{oldpos[0], oldpos[1]+value};
        }else if(toward == SceneData.BUTTON_MAZEMOVE_TOWARD_LEFT){
            wantedPos = new int[]{oldpos[0]-value, oldpos[1]};
        }else if(toward == SceneData.BUTTON_MAZEMOVE_TOWARD_RIGHT){
            wantedPos = new int[]{oldpos[0]+value, oldpos[1]};  
        }
    }

    /**
     * Set new position.
     * @param posX New position x
     * @param posY New position y
     */
    public void setNewPos(int posX, int posY){
        pos.add(new int[]{posX, posY});
        maze[posY][posX] = MAZE_PLAYER;
    }

    /**
     * Initialize maze.
     * @param height Height of maze.
     * @param width Witdh of maze.
     */
    public void setMaze(int height, int width){
        pos.clear();
        maze = new char[height][width];
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                maze[i][j] = MAZE_UNKNOWN; 
            }
        }
    }

    /**
     * Add player into the list of players.
     * @param username Player username.
     * @param posX Player position x.
     * @param posY Player position y.
     * @param score Player score.
     */
    public void addInPlayers(String username, String posX, String posY, String score){
        players.add(new Player(username, posX, posY, score));
    }

    /**
     * Check if the last position is the wanted position.
     * @param newPos New position.
     * @param wantedPos Wanted position.
     * @return true if the last position is the wanted position, 
     * else false.
     */
    private boolean lastPosIsWantedPos(int[] newPos, int[] wantedPos){
        return newPos[0] == wantedPos[0] && newPos[1] == wantedPos[1];
    }

    /**
     * Change information into the maze (in function of players moves).
     * @param oldPos Start position.
     * @param toward Toward the final position.
     * @param index Number of move (actually did).
     * @param flag 0 to transform position to MAZE_GROUND, 
     * else MAZE_WALL.
     */
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

    /**
     * To update maze.
     * @param newPos Wanted position.
     */
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

    /**
     * Update player info.
     * @param username Player username.
     * @param posX Player position x.
     * @param posY Player position y.
     * @param score Player score.
     */
    public void updatePlayerInfo(String username, String posX, String posY, String score){
        for(Player wanted: players){
            wanted.setPosX(posX);
            wanted.setPosY(posY);
            wanted.setScore(score);
        }
    }
}
