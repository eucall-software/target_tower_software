/*
 *************************************************************************
 * @file TargetTowerThreadFocusingFce.cpp
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

    void TargetTower::threadFocusingFce(){
    
        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting the focusing thread -> wrong state");
            //thread status
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        
        csutils::WRITE_INFO("Looking for the bes focus at the current position");
        
        //local
        double roughFocusZ = 0.0; //[mm]
        double fineFocusZ = 0.0; //[mm]
        
        //rough
        if(!roughFocus(roughFocusZ)){
            csutils::WRITE_ERROR("Failed to run rough focus");
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        if(!motion.axisZ->moveAbsolute(roughFocusZ)){
            csutils::WRITE_ERROR("Aborting 'focus' -> failed to move to the rough focus Z axis position");
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        while(motion.axisZ->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }        
        
        if(alignment.interuptAlignment.load()){
            csutils::WRITE_WARNING("Aborting alignment on the user request");
            currentStatus.set(state::READY);
        
            //thread status
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        
        //fine
        if(!fineFocus(roughFocusZ,fineFocusZ)){
            csutils::WRITE_ERROR("Failed to run fine focus");
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        if(!motion.axisZ->moveAbsolute(fineFocusZ)){
            csutils::WRITE_ERROR("Aborting 'focus' -> failed to move to the fine focus Z axis position");
            threading.focusing.threadFocusingUp.store(false);
            return;
        }
        while(motion.axisZ->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }        
        
        //report
        csutils::WRITE_INFO("Found the best focus on the Z axis at [mm]: " + std::to_string(fineFocusZ));
        
        //set state-------------------------------------------------------------
        currentStatus.set(state::READY);
        
        //thread status
        threading.focusing.threadFocusingUp.store(false);
        
        return;
    }
    
}
}
