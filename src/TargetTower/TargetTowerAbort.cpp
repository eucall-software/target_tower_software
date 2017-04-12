/*
 *************************************************************************
 * @file TargetTowerAbort.cpp
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

    bool TargetTower::abort(){
        
        //check state
        if(currentStatus.compare(state::READY) ||
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::INITIALIZED)){
            csutils::WRITE_ERROR("Aborting 'abort' -> wrong state");
            return false;
        }
        
        //report ---------------------------------------------------------------
        
        csutils::WRITE_INFO("Aborting the current process");
        
        //Manage abort based on the state---------------------------------------
        switch(currentStatus.get()){
            case state::ALIGNING:
                   alignment.interuptAlignment.store(true);
                break;
            case state::HOMING:
                break;
            case state::ANALYSING:
                analysis.interuptAnalysis.store(true);
                break;    
            case state::MOVING:
                if(motion.axisX->isOperational()){
                    motion.axisX->abortMovement();
                }
                if(motion.axisY->isOperational()){
                    motion.axisY->abortMovement();
                }
                if(motion.axisZ->isOperational()){
                    motion.axisZ->abortMovement();
                }
                if(motion.axisT->isOperational()){
                    motion.axisT->abortMovement();
                }
                if(motion.axisP->isOperational()){
                    motion.axisP->abortMovement();
                }
                break;    
            default:
                csutils::WRITE_ERROR("Failed to determine the state for the abort call");
                return false;
        }
        
        //done
        return true;
    }
    
}
}
