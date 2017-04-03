/*
 *************************************************************************
 * @file TargetTowerFocusAtPoint.cpp
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

    bool TargetTower::focusAtPoint(const AlignmentPoint& point, double& resultingZ, bool repeated){
        
        //locals
        double roughFocusZ = 0.0, fineFocusZ = 0.0;
        double providedFocusZ = 0.0;
        
        //default return
        if(repeated && resultingZ <= 0.0){
            resultingZ = 0.0;
            repeated = false;
            csutils::WRITE_WARNING("Detected repeated point focus search, but the provided rough"
            " focus Z position is not valid. Will run full focus search");
        }else if(repeated){
            providedFocusZ = resultingZ;
            resultingZ = 0.0;
        }else{
            resultingZ = 0.0;
        }
        
        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting 'focusAtPoint' -> wrong state");
            return false;
        }
        
        //move to point
        if(!motion.axisX->moveAbsolute(point.x)){
            csutils::WRITE_ERROR("Aborting 'focusAtPoint' -> failed to issue the X axis movement");
            return false;
        }
        if(!motion.axisY->moveAbsolute(point.y)){
            motion.axisX->abortMovement();
            csutils::WRITE_ERROR("Aborting 'focusAtPoint' -> failed to issue the Y axis movement");
            return false;
        }
        
        //wait for movement to finish
        while(motion.axisX->isOperational() || motion.axisY->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }
        
        //rough focus-----------------------------------------------------------
        if(repeated){
            if(!mildFocus(providedFocusZ, roughFocusZ)){
                if(!roughFocus(roughFocusZ)){
                    csutils::WRITE_ERROR("Failed to find rough Z axis position");
                    return false;
                }
            }
        }else{
            if(!roughFocus(roughFocusZ)){
            csutils::WRITE_ERROR("Failed to find rough Z axis position");
            return false;
            }
        }
        if(roughFocusZ == 0.0){
            csutils::WRITE_ERROR("The returned rough Z axis position is not valid");
            return false;
        }
        
        //move to z
        if(!motion.axisZ->moveAbsolute(roughFocusZ)){
            csutils::WRITE_ERROR("Aborting 'focusAtPoint' -> failed to move to the rough focus Z axis position");
            return false;
        }
        while(motion.axisZ->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }
        
        //fine focus
        if(!fineFocus(roughFocusZ,fineFocusZ)){
            csutils::WRITE_ERROR("Failed to find fine Z axis position");
            return false;
        }
        if(fineFocusZ == 0.0){
            csutils::WRITE_ERROR("The returned fine Z axis position is not valid");
            return false;
        }
        
        //move to z
        if(!motion.axisZ->moveAbsolute(fineFocusZ)){
            csutils::WRITE_ERROR("Aborting 'focusAtPoint' -> failed to move to the fine focus Z axis position");
            return false;
        }
        while(motion.axisZ->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }
        
        //report 
        csutils::WRITE_INFO("Found focus Z coordinate [mm]: " + std::to_string(fineFocusZ));
        resultingZ = fineFocusZ;
        
        return true;
    }
    
}
}
