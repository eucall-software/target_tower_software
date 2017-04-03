/*
 *************************************************************************
 * @file TargetTowerFineFocusP.cpp
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

    bool TargetTower::fineFocusP(const double& startingP, double& finalP){
    
        //default return
        finalP = 0.0;

        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting the 'fineFocusP' -> "
                "wrong state");
            return false;
        }
        
        if(startingP == 0.0){
            csutils::WRITE_ERROR("Aborting the 'fineFocus' -> "
                "bad input starting P position");
            return false;
        }
        
        //variables-------------------------------------------------------------
        csimage::MonoImage* image;
        std::vector<double> positions;
        std::vector<double> focuses;
        std::vector<double>::iterator indexOfBestIterator;
        uint32_t indexOfBest = 0;
        
        double startPosition = motion.axisP->position->get();
        double previousPosition = 0.0;
        double currentPosition = 0.0;
        double focus = 0.0;
        double velocityOriginal = 0.0;
        
        //check position Z
        if(startPosition != startingP){
            if(!motion.axisP->moveAbsolute(startingP)){
                csutils::WRITE_ERROR("Failed to move to the best rough focus P position");
                return false;
            }
            while(motion.axisP->isOperational()){
                std::this_thread::sleep_for(std::chrono::microseconds(motion.configuration.movementPollPeriod));
                continue;
            }
            
            //new position
            startPosition = motion.axisP->position->get();
        }
        
        //move through the Z----------------------------------------------------
        std::cout << "Start position (fine): " << startPosition << std::endl;

        //move to max range ----------------------------------------------------
        if(!motion.axisP->moveRelative(alignment.movementAlt.fineRange)){
            csutils::WRITE_ERROR("Failed to move the P axis to fine range position");
            return false;
        }
        while (motion.axisP->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(motion.configuration.movementPollPeriod)
            );
        }

        //change speed
        velocityOriginal = motion.axisP->velocity->get();
        if(!motion.axisP->velocity->set(velocityOriginal/alignment.focusVelocityCoefficientFine)){
            csutils::WRITE_WARNING("Failed to set new velocity for the P axis stepper motor "
            "while fine focus evaluation");
        }
        
        //move to min ramge && take data ---------------------------------------------
        if(!motion.axisP->moveRelative(-2*alignment.movementAlt.fineRange)){
            csutils::WRITE_ERROR("Failed to start the P axis movement for the fine focus search");
            return false;
        }
        while (motion.axisP->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(alignment.imagePoolPeriod)
            );
            
            //take current position and check if it is not the same as previous (not moving - limit switch)
            currentPosition = motion.axisP->position->get();
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
        if(!motion.axisP->velocity->set(velocityOriginal)){
            csutils::WRITE_WARNING("Failed to set back the velocity for the P axis stepper motor "
            "after fine focus evaluation");
        }

        //move to starting position
        motion.axisP->moveAbsolute(startPosition);
        while (motion.axisP->isOperational()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(motion.configuration.movementPollPeriod)
            );
        }
        std::cout << "Stop position: " << motion.axisP->position->get() << std::endl;
        
        //find best focus-------------------------------------------------------
        indexOfBestIterator = std::max_element(focuses.begin(), focuses.end());
        indexOfBest = std::distance(focuses.begin(), indexOfBestIterator);
        
        //extract---------------------------------------------------------------
        try{
            //get best Z
            finalP = positions.at(indexOfBest);
            std::cout << " FOCUS: " << focuses.at(finalP) << std::endl;
        }catch(std::out_of_range &ex){
            csutils::WRITE_ERROR("Caught 'std::out_of_range' exception while accessing positions vector");
            return false;
        }catch(std::exception &ex){
            csutils::WRITE_ERROR("Caught 'std::exception' exception while accessing positions vector");
            return false;
        }
        
        //finish
        csutils::WRITE_INFO("Finished fine focus search");
        
        return true;
    }
    
}
}
