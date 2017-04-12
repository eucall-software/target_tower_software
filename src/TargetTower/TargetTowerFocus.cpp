/*
 *************************************************************************
 * @file TargetTowerFocus.cpp
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

    bool TargetTower::focus(){
        
        //check state
        if(!currentStatus.compare(state::READY)){
            csutils::WRITE_ERROR("Aborting 'focus' -> wrong state");
            return false;
        }
        
        //change state
        currentStatus.set(state::ALIGNING);
        
        //interrupt flag
        alignment.interuptAlignment.store(false);
        
        //start the monitor thread
        if(!threadFocusingStart()){
            csutils::WRITE_ERROR("Failed to start the background focusing thread");
            return false;
        }
        
        //done
        return true;
    }
    
}
}
