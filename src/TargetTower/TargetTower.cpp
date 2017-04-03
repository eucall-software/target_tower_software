/*
 *************************************************************************
 * @file TargetTower.cpp
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

    TargetTower::TargetTower() {
        
        //initial state
        currentStatus.set(state::INITIALIZED);
        
        //thread initial variables
        threading.homing.threadHomingKill.store(false);
        threading.homing.threadHomingUp.store(false);
        threading.aligning.threadAligningKill.store(false);
        threading.aligning.threadAligningUp.store(false);
        threading.moving.threadMovingKill.store(false);
        threading.moving.threadMovingUp.store(false);
        threading.focusing.threadFocusingKill.store(false);
        threading.focusing.threadFocusingUp.store(false);
        threading.analysis.threadAnalysisKill.store(false);
        threading.analysis.threadAnalysisUp.store(false);
        
        //abort flags
        alignment.interuptAlignment.store(false);
        
        //assign parameters
        deltaZX.reset(new csutils::rop<double>(&parameters.alignment.deltaZX_internal));
        deltaZY.reset(new csutils::rop<double>(&parameters.alignment.deltaZY_internal));
        
    }

    TargetTower::~TargetTower() {

        //stop movement
        if(currentStatus.compare(state::HOMING) || 
                currentStatus.compare(state::ALIGNING) || 
                currentStatus.compare(state::ANALYSING)){
            //abort
            if(!abort()){
                csutils::WRITE_WARNING("Failure in the destructor -> failed to abort movement");
            }
        }
        
        //disconnect
        if(currentStatus.compare(state::READY)){
            //disconnect
            if(!disconnect()){
                csutils::WRITE_WARNING("Failure in the destructor -> failed to disconnect");
            }
        }
        
        //check on threads------------------------------------------------------
        threadAnalysisStop();
        threadFocusingStop();
        threadMovingStop();
        threadHomingStop();
        threadAlignmentStop();
        
    }
}
}
