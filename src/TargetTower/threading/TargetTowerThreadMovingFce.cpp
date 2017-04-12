/*
 *************************************************************************
 * @file TargetTowerThreadMovingFce.cpp
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

    void TargetTower::threadMovingFce(axis id){
    
        //check state
        if(!currentStatus.compare(state::MOVING)){
            csutils::WRITE_ERROR("Aborting the movement process monitor thread -> "
                "wrong state");
            //thread status
            threading.moving.threadMovingUp.store(false);
            return;
        }
        
        //locals
        csbase::Plugin<csif::csmotion::ContinuousStage>* move = nullptr;
        
        //determine axis
        switch(id){
            case (axis::X):
                move = &motion.axisX;
                break;
            case (axis::Y):
                move = &motion.axisY;
                break;
            case (axis::Z):
                move = &motion.axisZ;
                break;    
            case (axis::P):
                move = &motion.axisP;
                break;
            case (axis::T):
                move = &motion.axisT;
                break;    
            default:
                csutils::WRITE_ERROR("Aborting the movement process -> "
                "failed to identify the axis");
                //thread status
                threading.moving.threadMovingUp.store(false);
                return;
        }
        
        //check the movement
        while(move->get()->isOperational()){
            
            //check for end
            if(threading.moving.threadMovingKill.load()){
                move->get()->abortMovement();
                currentStatus.set(state::READY);
                
                csutils::WRITE_INFO("The Target Tower movement was aborted");
                
                //thread status
                threading.moving.threadMovingUp.store(false);
                return;
            }
            
            //sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            continue;
        }
        
        //report
        csutils::WRITE_INFO("The Target Tower movement is finished");
        
        //set state-------------------------------------------------------------
        currentStatus.set(state::READY);
        
        //thread status
        threading.moving.threadMovingUp.store(false);
        
        return;
    }
    
}
}
