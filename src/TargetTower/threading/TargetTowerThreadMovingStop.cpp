/*
 *************************************************************************
 * @file TargetTowerThreadMovingStop.cpp
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

    bool TargetTower::threadMovingStop(){
        
        //set the kill flag
        threading.moving.threadMovingKill.store(true);

        //reset the thread object
        if(threading.moving.threadMoving.joinable()){
            threading.moving.threadMoving.join();
        }

        //store the status
        threading.moving.threadMovingUp.store(false);

        return true;

    }
}
}
