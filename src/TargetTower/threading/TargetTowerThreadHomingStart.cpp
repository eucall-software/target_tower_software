/*
 *************************************************************************
 * @file TargetTowerThreadHomingStart.cpp
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

    bool TargetTower::threadHomingStart(){
    
        //check if already up
        if(threading.homing.threadHomingUp.load()){
            csutils::WRITE_WARNING("The homing thread is already up!!");
            return true;
        }

        //set the kill to true to make sure it is dead
        threading.homing.threadHomingKill.store(true);

        //reset the thread object
        if(threading.homing.threadHoming.joinable()){
            threading.homing.threadHoming.join();
        }

        //set the kill to false
        threading.homing.threadHomingKill.store(false);

        //start thread
        threading.homing.threadHoming = std::thread(&TargetTower::threadHomingFce ,this);

        //store the status
        threading.homing.threadHomingUp.store(true);
    
        return true;
    }
    
}
}
