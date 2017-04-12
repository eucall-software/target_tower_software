/*
 *************************************************************************
 * @file TargetTowerThreadHomingStop.cpp
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

    bool TargetTower::threadHomingStop(){
        
        //set the kill flag
        threading.homing.threadHomingKill.store(true);

        //reset the thread object
        if(threading.homing.threadHoming.joinable()){
            threading.homing.threadHoming.join();
        }

        //store the status
        threading.homing.threadHomingUp.store(false);

        return true;

    }
}
}
