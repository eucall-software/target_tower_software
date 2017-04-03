/*
 *************************************************************************
 * @file TargetTowerAlign.cpp
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

    bool TargetTower::align(bool alignX, bool alignY, uint32_t holder){
        
        //check state
        if(!currentStatus.compare(state::READY)){
            csutils::WRITE_ERROR("Aborting 'align' -> wrong state");
            return false;
        }
        
        //check status of the motion hardware
        if(!motion.axisX->isReady()){
            csutils::WRITE_ERROR("Aborting 'align' -> the axis X is not ready for frame alignment");
            return false;
        }
        if(!motion.axisY->isReady()){
            csutils::WRITE_ERROR("Aborting 'align' -> the axis Y is not ready for frame alignment");
            return false;
        }
        if(!motion.axisZ->isReady()){
            csutils::WRITE_ERROR("Aborting 'align' -> the axis Z is not ready for frame alignment");
            return false;
        }
        if(!motion.axisT->isReady()){
            csutils::WRITE_ERROR("Aborting 'align' -> the axis T is not ready for frame alignment");
            return false;
        }
        if(!motion.axisP->isReady()){
            csutils::WRITE_ERROR("Aborting 'align' -> the axis P is not ready for frame alignment");
            return false;
        }
        
        //check status of the camera
        if(!camera.cam->isOperational()){
            csutils::WRITE_ERROR("Aborting 'align' -> the camera is not ready for frame alignment");
            return false;
        }

        //report ---------------------------------------------------------------
        
        csutils::WRITE_INFO("Aligning the Target Tower");
        
        //change state
        currentStatus.set(state::ALIGNING);
        
        //abort flag
        alignment.interuptAlignment.store(false);
        
        //start the home thread-------------------------------------------------
        if(!threadAlignmentStart(alignX,alignY, holder)){
            currentStatus.set(state::READY);
            csutils::WRITE_ERROR("Failed to start the alignment process");
            return false;
        }
        
        //done
        return true;
    }
}
}
