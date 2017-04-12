/*
 *************************************************************************
 * @file TargetTowerThreadHomingFce.cpp
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

    void TargetTower::threadHomingFce(){
    
        //check state
        if(!currentStatus.compare(state::HOMING)){
            csutils::WRITE_ERROR("Aborting the homing process -> "
                "wrong state");
            //thread status
            threading.homing.threadHomingUp.store(false);
            return;
        }
        
        //home tilt and move it to minimum
        if(!threading.homing.threadHomingKill){
            //home
            if(!motion.axisP->calibrate(true)){
                csutils::WRITE_ERROR("Aborting the homing process -> "
                "the homing of axis P failed");
                currentStatus.set(state::CONFIGURED);
                //thread status
                threading.homing.threadHomingUp.store(false);    
                return;
            }else{
                csutils::WRITE_INFO("Homing finished for the axis P");
            }
        }else{
            csutils::WRITE_ERROR("Interrupted homing (axis P)");
            currentStatus.set(state::CONFIGURED);
            return;
        }
//        if(!motion.axisP->run(csif::csmotion::ContinuousStage::Direction::NEGATIVE)){
//            csutils::WRITE_WARNING("Failed to move the P axis to the minimum position");
//        }else{
//            while(motion.axisT->isOperational()){
//                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
//            }
//        }        
        
        //home the rest of the axis---------------------------------------------
        bool failed = false;
        std::future<bool> futx, futy, futz, futt;
        
        if(!threading.homing.threadHomingKill){
            //home
            futx = std::async(std::launch::async,[this](){return motion.axisX->calibrate(true);});       
            futy = std::async(std::launch::async,[this](){return motion.axisY->calibrate(true);});
            futz = std::async(std::launch::async,[this](){return motion.axisZ->calibrate(true);});
            futt = std::async(std::launch::async,[this](){return motion.axisT->calibrate(true);});
            
        }else{
            csutils::WRITE_ERROR("Homing was interrupted");
            currentStatus.set(state::CONFIGURED);
            //thread status
            threading.homing.threadHomingUp.store(false);
            return;
        }
        
        // wait for homing to finish
        if(!futx.get()){
            csutils::WRITE_ERROR("Homing of axis X failed");
            failed = true;
        }
        if(!futy.get()){
            csutils::WRITE_ERROR("Homing of axis Y failed");
            failed = true;
        }
        if(!futz.get()){
            csutils::WRITE_ERROR("Homing of axis Z failed");
            failed = true;
        }
        if(!futt.get()){
            csutils::WRITE_ERROR("Homing of axis T failed");
            failed = true;
        }
        
        // evaluate
        if(failed){
            currentStatus.set(state::CONFIGURED);
            csutils::WRITE_ERROR("The homing process of the Target Tower failed");
            //thread status
            threading.homing.threadHomingUp.store(false);
            return;
        }
        
        //move X to default positions
        if(!motion.axisX->moveAbsolute(motion.configuration.defaultX)){
            csutils::WRITE_WARNING("Failed to move the X axis to the default position of: " + 
            std::to_string(motion.configuration.defaultX) + "[mm]");
        }else{
            while(motion.axisX->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        //move Y to default positions
        if(!motion.axisY->moveAbsolute(motion.configuration.defaultY)){
            csutils::WRITE_WARNING("Failed to move the Y axis to the default position of: " + 
            std::to_string(motion.configuration.defaultY) + "[mm]");
        }else{
            while(motion.axisY->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        
        //move Z to default positions
        if(!motion.axisZ->moveAbsolute(motion.configuration.defaultZ)){
            csutils::WRITE_WARNING("Failed to move the Z axis to the default position of: " + 
            std::to_string(motion.configuration.defaultZ) + "[mm]");
        }else{
            while(motion.axisZ->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        
        //move T to default positions
        if(!motion.axisT->moveAbsolute(motion.configuration.defaultT)){
            csutils::WRITE_WARNING("Failed to move the T axis to the default position of: " + 
            std::to_string(motion.configuration.defaultT) + "[mm]");
        }else{
            while(motion.axisT->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }        
        
        //move P to default positions
        if(!motion.axisP->moveAbsolute(motion.configuration.defaultP)){
            csutils::WRITE_WARNING("Failed to move the P axis to the default position of: " + 
            std::to_string(motion.configuration.defaultP) + "[mm]");
        }else{
            while(motion.axisP->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        
        //resolution
        std::cout << "X resolution: " << motion.axisX->resolution->get() << std::endl;
        std::cout << "Y resolution: " << motion.axisY->resolution->get() << std::endl;
        std::cout << "Z resolution: " << motion.axisZ->resolution->get() << std::endl;
        std::cout << "P resolution: " << motion.axisP->resolution->get() << std::endl;
        std::cout << "T resolution: " << motion.axisT->resolution->get() << std::endl;
        
        //report
        csutils::WRITE_INFO("The Target Tower is homed and ready to use");
        
        //set state-------------------------------------------------------------
        currentStatus.set(state::READY);
        
        //thread status
        threading.homing.threadHomingUp.store(false);
        
        return;
    }
    
}
}
