/*
 *************************************************************************
 * @file TargetTowerThreadAlignmentStop.cpp
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

    bool TargetTower::threadAlignmentStop(){
        
        //set the kill flag
        threading.aligning.threadAligningKill.store(true);

        //reset the thread object
        if(threading.aligning.threadAligning.joinable()){
            threading.aligning.threadAligning.join();
        }

        //store the status
        threading.aligning.threadAligningUp.store(false);

        return true;

    }
}
}
