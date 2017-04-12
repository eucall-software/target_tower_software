/*
 *************************************************************************
 * @file TargetTowerRoughFocus.cpp
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

    bool TargetTower::roughFocus(double& finalZ){

        //default return
        finalZ = 0.0;
        
        //check state
        if(!(currentStatus.compare(state::ALIGNING) || currentStatus.compare(state::ANALYSING))){
            csutils::WRITE_ERROR("Aborting the 'roughFocus' -> "
                "wrong state");
            return false;
        }
        
        //variables-------------------------------------------------------------
        csimage::MonoImage* image = nullptr;
        std::vector<double> positions;
        std::vector<double> focuses;
        std::vector<double>::iterator indexOfBestIterator;
        uint32_t indexOfBest = 0;
        
        double startPosition = motion.axisZ->position->get();
        double previousPosition = 0.0;
        double currentPosition = 0.0;
        double focus = 0.0;
        double velocityOriginal = 0.0;
        
        //move through the Z
        std::cout << "Start position: " << startPosition << std::endl;

        //move to max
        if(!motion.axisZ->run(csif::csmotion::ContinuousStage::Direction::POSITIVE)){
            csutils::WRITE_ERROR("Failed to move the Z axis to maximum position");
            return false;
        }
        while (motion.axisZ->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(motion.configuration.movementPollPeriod)
            );
        }

        //change speed
        velocityOriginal = motion.axisZ->velocity->get();
        if(!motion.axisZ->velocity->set(velocityOriginal/alignment.focusVelocityCoefficientRough)){
            csutils::WRITE_WARNING("Failed to set new velocity for the Z axis stepper motor "
            "while rough focus evaluation");
        }
        
        //move to min && take data ---------------------------------------------
        motion.axisZ->run(csif::csmotion::ContinuousStage::Direction::NEGATIVE);
        while (motion.axisZ->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(alignment.imagePoolPeriod)
            );
            
            //take current position and check if it is not the same as previous (not moving - limit switch)
            currentPosition = motion.axisZ->position->get();
            if(currentPosition == previousPosition){
                continue; //if the same continue
            }
            
            //get image
            if(!retrieveLatestImage(&image) || !image){
                csutils::WRITE_ERROR("Failed to extract the image");
                continue;
            }

            if(!evaluateFocus(*image, focus)){
                csutils::WRITE_ERROR("Failed the focus evaluation");
                continue;
            }
            
            //save position
            previousPosition = currentPosition;
            positions.push_back(currentPosition);
            focuses.push_back(focus);
            
            //
            continue;
        }//moving&&search loop
        
        //change the velocity back----------------------------------------------
        if(!motion.axisZ->velocity->set(velocityOriginal)){
            csutils::WRITE_WARNING("Failed to set back the velocity for the Z axis stepper motor "
            "after focus evaluation");
        }

        //move to starting position
        motion.axisZ->moveAbsolute(startPosition);
        while (motion.axisZ->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(motion.configuration.movementPollPeriod)
            );
        }
        std::cout << "Stop position: " << motion.axisZ->position->get() << std::endl;
        
        //find best focus-------------------------------------------------------
        indexOfBestIterator = std::max_element(focuses.begin(), focuses.end());
        indexOfBest = std::distance(focuses.begin(), indexOfBestIterator);
        
        //extract---------------------------------------------------------------
        try{
            //get best Z
            finalZ = positions.at(indexOfBest);
        }catch(std::out_of_range &ex){
            csutils::WRITE_ERROR("Caught 'std::out_of_range' exception while accessing positions vector");
            return false;
        }catch(std::exception &ex){
            csutils::WRITE_ERROR("Caught 'std::exception' exception while accessing positions vector");
            return false;
        }
        
        //finish
        csutils::WRITE_INFO("Finished rough focus search");
        
        return true;
    }
    
}
}
