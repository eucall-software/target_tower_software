/*
 *************************************************************************
 * @file TargetTowerDisconnect.cpp
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

    bool TargetTower::disconnect(){
        
        //check state
        if(!currentStatus.compare(state::READY)){
            csutils::WRITE_ERROR("Aborting 'align' -> wrong state");
            return false;
        }

        //report ---------------------------------------------------------------
        
        csutils::WRITE_INFO("Disconnecting the Target Tower from motion and camera system");
        
        //disconnect stuff-------------------------------------------------------
        if(!motion.axisX->disconnect()){
            csutils::WRITE_WARNING("Failed to disconnect from the X axis");
        }
        if(!motion.axisY->disconnect()){
            csutils::WRITE_WARNING("Failed to disconnect from the Y axis");
        }
        if(!motion.axisZ->disconnect()){
            csutils::WRITE_WARNING("Failed to disconnect from the Z axis");
        }
        if(!motion.axisP->disconnect()){
            csutils::WRITE_WARNING("Failed to disconnect from the P axis");
        }
        if(!motion.axisT->disconnect()){
            csutils::WRITE_WARNING("Failed to disconnect from the T axis");
        }
        if(!camera.cam->stopAcq()){
            csutils::WRITE_WARNING("Failed to stop the camera stream");
        }else{
            if(!camera.cam->disconnect()){
                csutils::WRITE_WARNING("Failed to disconnect from the camera");
            }
        }
        
        //change state
        currentStatus.set(state::CONFIGURED);
        
        //Clear the configuration-------------------------------------------------
        if(!clearConfiguration()){
            csutils::WRITE_WARNING("Failed to clear the current configuration while disconnecting");
        }
        
        // kill threads
        threadAlignmentStop();
        threadHomingStop();
        
        //finish
        return true;
    }
    
}
}
