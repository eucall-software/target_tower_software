/*
 *************************************************************************
 * @file TargetTowerGetPosition.cpp
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

    bool TargetTower::getPositionX(double& position){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED) || 
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting 'getPositionX' -> wrong state");
            return false;
        }
        
        //get the value
        position = motion.axisX->position->get();
        
        return true;
    }

    bool TargetTower::getPositionY(double& position){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED) || 
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting 'getPositionY' -> wrong state");
            return false;
        }
        
        //get the value
        position = motion.axisY->position->get();
        
        return true;
    }
    
    bool TargetTower::getPositionZ(double& position){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED) || 
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting 'getPositionZ' -> wrong state");
            return false;
        }
        
        //get the value
        position = motion.axisZ->position->get();
        
        return true;
    }

    bool TargetTower::getPositionP(double& position){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED) || 
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting 'getPositionP' -> wrong state");
            return false;
        }
        
        //get the value
        position = motion.axisP->position->get();
        
        return true;
    }

    bool TargetTower::getPositionT(double& position){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED) || 
                currentStatus.compare(state::CONFIGURED) || 
                currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting 'getPositionT' -> wrong state");
            return false;
        }
        
        //get the value
        position = motion.axisT->position->get();
        
        return true;
    }    
    
}
}
