package com.ustudents.ghostlab.menu;

import com.ustudents.ghostlab.client.Client;

public class MainScene extends Scene{

    public MainScene(Client client){
        super(client);
    }

    public void display(){
        header();
        footer();
    }
    
}
