/*
 *************************************************************************
 * @file TargetTowerMoveRelative.cpp
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

    bool TargetTower::moveRelative(axis id, const double& distance){
        
        //check state
        if(!currentStatus.compare(state::READY)){
            csutils::WRITE_ERROR("Aborting 'moveRelative' -> wrong state");
            return false;
        }
        
        //determine axis
        csbase::Plugin<csif::csmotion::ContinuousStage>* move = nullptr;
        std::string name = "";

        //----------------------------------------------------------------------
        switch(id){
            case (axis::X):
                move = &motion.axisX;
                name = "X";
                break;
            case (axis::Y):
                move = &motion.axisY;
                name = "Y";
                break;
             case (axis::Z):
                move = &motion.axisZ;
                name = "Z";
                break;
            case (axis::P):
                move = &motion.axisP;
                name = "P";
                break;
            case (axis::T):
                move = &motion.axisT;
                name = "T";
                break;    
            default:
                csutils::WRITE_ERROR("Aborting 'moveRelative' -> failed to identify the axis");
                return false;
        }
        
        //chcek  ptr
        if(!move){
            csutils::WRITE_ERROR("Aborting 'moveRelative' -> the axis object pointer is invalid");
            return false;
        }
        
        //issue move cmd
        if(!move->get()->moveRelative(distance)){
            csutils::WRITE_ERROR("Aborting 'moveRelative' -> failed to issue the command");
            return false;
        }
        
        csutils::WRITE_INFO("Starting the relative movement on the axis " + name);
        
        //change state
        currentStatus.set(state::MOVING);
        
        //start the monitor thread
        if(!threadMovingStart(id)){
            abort();
            csutils::WRITE_ERROR("Failed to start the background movement monitoring thread. Aborting movement");
            return false;
        }
        
        return true;
    }
    
}
}
