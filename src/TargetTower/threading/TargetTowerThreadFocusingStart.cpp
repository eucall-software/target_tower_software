/*
 *************************************************************************
 * @file TargetTowerThreadFocusingStart.cpp
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

    bool TargetTower::threadFocusingStart(){
    
        //check if already up
        if(threading.focusing.threadFocusingUp.load()){
            csutils::WRITE_WARNING("The focusing thread is already up!!");
            return true;
        }

        //set the kill to true to make sure it is dead
        threading.focusing.threadFocusingKill.store(true);

        //reset the thread object
        if(threading.focusing.threadFocusing.joinable()){
            threading.focusing.threadFocusing.join();
        }

        //set the kill to false
        threading.focusing.threadFocusingKill.store(false);

        //start thread
        threading.focusing.threadFocusing = std::thread(&TargetTower::threadFocusingFce ,this);

        //store the status
        threading.focusing.threadFocusingUp.store(true);
    
        return true;
    }
    
}
}
