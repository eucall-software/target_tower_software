/*
 *************************************************************************
 * @file TargetTowerClearConfiguration.cpp
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

    bool TargetTower::clearConfiguration(){
        
        //check state
        if(!currentStatus.compare(state::CONFIGURED)){
            csutils::WRITE_ERROR("Aborting 'clearConfiguration' -> wrong state");
            return false;
        }
        
        csutils::WRITE_INFO("Clearing the current configuration");
        
        //clear motion ---------------------------------------------------------
        motion.axisX->disconnect();
        motion.axisX.free();
        motion.configuration.defaultX = 0.0;
        
        motion.axisY->disconnect();
        motion.axisY.free();
        motion.configuration.defaultY = 0.0;
        
        motion.axisZ->disconnect();
        motion.axisZ.free();
        motion.configuration.defaultZ = 0.0;
        
        motion.axisT->disconnect();
        motion.axisT.free();
        motion.configuration.defaultT = 0.0;
        
        motion.axisP->disconnect();
        motion.axisP.free();
        motion.configuration.defaultP = 0.0;
        
        motion.configuration.type.clear();
        
        //clear camera ---------------------------------------------------------
        camera.cam->stopAcq();
        camera.cam->disconnect();
        camera.cam.free();
        
        //change state----------------------------------------------------------
        currentStatus.set(state::INITIALIZED);
        
        //report
        csutils::WRITE_INFO("Configuration of the Target Tower is clear");    
        
        return true;
    }
    
}
}
