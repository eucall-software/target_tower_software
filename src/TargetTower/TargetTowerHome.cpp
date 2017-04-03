/*
 *************************************************************************
 * @file TargetTowerHome.cpp
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

    bool TargetTower::home(){
        
        //check state
        if(!currentStatus.compare(state::CONFIGURED)){
            csutils::WRITE_ERROR("Aborting 'home' -> wrong state");
            return false;
        }
        
        //check status of the motion hardware
        if(!motion.axisX->isConnected()){
            csutils::WRITE_ERROR("Aborting 'home' -> the axis X is not ready for homing");
            return false;
        }
        if(!motion.axisY->isConnected()){
            csutils::WRITE_ERROR("Aborting 'home' -> the axis Y is not ready for homing");
            return false;
        }
        if(!motion.axisZ->isConnected()){
            csutils::WRITE_ERROR("Aborting 'home' -> the axis Z is not ready for homing");
            return false;
        }
        if(!motion.axisT->isConnected()){
            csutils::WRITE_ERROR("Aborting 'home' -> the axis T is not ready for homing");
            return false;
        }
        if(!motion.axisP->isConnected()){
            csutils::WRITE_ERROR("Aborting 'home' -> the axis P is not ready for homing");
            return false;
        }

        //report ---------------------------------------------------------------
        
        csutils::WRITE_INFO("Homing the Target Tower");
        
        //change state
        currentStatus.set(state::HOMING);
        
        //start the home thread-------------------------------------------------
        if(!threadHomingStart()){
            currentStatus.set(state::CONFIGURED);
            csutils::WRITE_ERROR("Failed to start the homing process");
            return false;
        }
        
        return true;
    }
    
}
}
