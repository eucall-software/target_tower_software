/*
 *************************************************************************
 * @file TargetTowerRetrieveLatestImage.cpp
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

    bool TargetTower::retrieveLatestImage(csimage::MonoImage** image){
        
        //check state
        if(currentStatus.compare(state::INITIALIZED)){
            csutils::WRITE_ERROR("Aborting 'retrieveLatestImage' -> wrong state");
            return false;
        }
        
            //extract image
        if(camera.cam->retrieveLatestData(image)){
            return true;
        }
    
        //if fails
        image = nullptr;
    
        return false;
    }
    
}
}
