/*
 *************************************************************************
 * @file TargetTowerThreadMovingStart.cpp
 *
 * CS Target Tower
 *
 *  Author: Ondrej Janda
 *  Copyright (c) 2014-2017 ELI Beamlines
 *  All rights reserved.
 *
 *  Project name: CS Target Tower
 *
 *  ELI Beamlines
 *  Fyzikální ústav AV ČR, v.v.i. / ELI Beamlines
 *  5. května 835
 *  252 41 Dolní Břežany
 *  Czech Republic
 *
 * *************************************************************************/

#include "TargetTower.h"

namespace csrp3{
namespace targetTower{

    bool TargetTower::threadMovingStart(axis id){
    
        //check if already up
        if(threading.moving.threadMovingUp.load()){
            csutils::WRITE_WARNING("The moving thread is already up!!");
            return true;
        }

        //set the kill to true to make sure it is dead
        threading.moving.threadMovingKill.store(true);

        //reset the thread object
        if(threading.moving.threadMoving.joinable()){
            threading.moving.threadMoving.join();
        }

        //set the kill to false
        threading.moving.threadMovingKill.store(false);

        //start thread
        threading.moving.threadMoving = std::thread(&TargetTower::threadMovingFce ,this, id);

        //store the status
        threading.moving.threadMovingUp.store(true);
    
        return true;
    }
    
}
}
