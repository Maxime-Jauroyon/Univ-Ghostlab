package com.ustudents.ghostlab.interaction;

import com.ustudents.ghostlab.client.Client;

public abstract class InteractionPerPhase {
    protected final Client client;

    public InteractionPerPhase(Client client){
        this.client = client;
    }
}
