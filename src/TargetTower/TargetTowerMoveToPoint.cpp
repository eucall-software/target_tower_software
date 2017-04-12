/*
 *************************************************************************
 * @file TargetTowerMoveToPoint.cpp
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

    bool TargetTower::moveToPoint(const AlignmentPoint& point){
        
        //check state
        if(!(currentStatus.compare(state::ALIGNING) || currentStatus.compare(state::ANALYSING))){
            csutils::WRITE_ERROR("Aborting 'moveToPoint' -> wrong state");
            return false;
        }
        
        //move to point
        if(!motion.axisX->moveAbsolute(point.x)){
            csutils::WRITE_ERROR("Aborting 'moveToPoint' -> failed to issue the X axis movement");
            return false;
        }
        if(!motion.axisY->moveAbsolute(point.y)){
            motion.axisX->abortMovement();
            csutils::WRITE_ERROR("Aborting 'moveToPoint' -> failed to issue the Y axis movement");
            return false;
        }
        
        //wait for movement to finish
        while(motion.axisX->isOperational() || motion.axisY->isOperational()){
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }
        
        return true;
    }
    
}
}
