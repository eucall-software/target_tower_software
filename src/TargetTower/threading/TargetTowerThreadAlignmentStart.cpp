/*
 *************************************************************************
 * @file TargetTowerThreadAlignmentStart.cpp
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

    bool TargetTower::threadAlignmentStart(bool alignX, bool alignY,  uint32_t holder){
    
        //check if already up
        if(threading.aligning.threadAligningUp.load()){
            csutils::WRITE_WARNING("The alignment thread is already up!!");
            return true;
        }

        //set the kill to true to make sure it is dead
        threading.aligning.threadAligningKill.store(true);

        //reset the thread object
        if(threading.aligning.threadAligning.joinable()){
            threading.aligning.threadAligning.join();
        }

        //set the kill to false
        threading.aligning.threadAligningKill.store(false);

        //start thread
        threading.aligning.threadAligning = std::thread(&TargetTower::threadAlignmentFce,this, alignX, alignY, holder);

        //store the status
        threading.aligning.threadAligningUp.store(true);
    
        return true;
    }
    
}
}
