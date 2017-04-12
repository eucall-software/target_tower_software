/*
 *************************************************************************
 * @file TargetTowerAlignHolderX.cpp
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

    bool TargetTower::alignHolderX(){
        
        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting 'alignFrameX' -> wrong state");
            return false;
        }
        
        //locals
        double finalZaxisP1 = 0.0, finalZaxisP2 = 0.0;
        double movement = 0.0, deltaZ = 0.0;
        double currentP = 0.0, requestedP = 0.0;
        bool reachedLimit = false;
        bool startFromTop = true; // if to find focus for P1 first
        bool repeated = false;
        
        //----------------------------------------------------------------------
        for (uint32_t i = 0; i < alignment.maxAlignmentSteps; i++) {
            
            if(alignment.interuptAlignment.load()){
                csutils::WRITE_WARNING("Aborting alignment on the user request");
                break;
            }
                
            csutils::WRITE_INFO("Attempt no. " + std::to_string(i+1) + " to minimize delta Z on the axis X");
            
            if(startFromTop){
                //focus at P1
                if(!focusAtPoint(alignment.alignmentPoints.holder1.h1, finalZaxisP1, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P1");
                    return false;
                }
                if(finalZaxisP1 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                if(alignment.interuptAlignment.load()){
                    csutils::WRITE_WARNING("Aborting alignment on the user request");
                    break;
                }

                //focus at P2
                if(!focusAtPoint(alignment.alignmentPoints.holder1.h2, finalZaxisP2, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P2");
                    return false;
                }
                if(finalZaxisP2 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                //just moved right
                startFromTop = false;                
            }else{
                //focus at P2
                if(!focusAtPoint(alignment.alignmentPoints.holder1.h2, finalZaxisP2, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P2");
                    return false;
                }
                if(finalZaxisP2 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                if(alignment.interuptAlignment.load()){
                    csutils::WRITE_WARNING("Aborting alignment on the user request");
                    break;
                }

                //focus at P1
                if(!focusAtPoint(alignment.alignmentPoints.holder1.h1, finalZaxisP1, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P1");
                    return false;
                }
                if(finalZaxisP1 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }                
                
                //just moved left
                startFromTop = true;                                
            }
            
            if(!repeated){
                repeated = true;
            }            

            //calculate
            deltaZ = fabs(finalZaxisP1-finalZaxisP2);
            currentP = motion.axisP->position->get();
            
            //save delta
            parameters.alignment.deltaZX_internal = deltaZ;
        
            //summary
            std::cout << "--------Summary for alignment run no "<< i+1 << "---------" << std::endl;
            std::cout << "P1 focus spot [mm]: " << finalZaxisP1 << std::endl;
            std::cout << "P2 focus spot [mm]: " << finalZaxisP2 << std::endl;
            std::cout << "Z delta [mm]: " << deltaZ << std::endl;            
            std::cout << "P is [mm]: " << currentP << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;
            
            //check for exit condition
            if(deltaZ <= alignment.movement.deltaZLimit ){ // finished
                csutils::WRITE_INFO("The delta Z for X axis is in the limit (30 um)");
                break;
            }else if(reachedLimit){
                csutils::WRITE_INFO("The P axis reached the limit");
                break;
            }
            
            //last run?
            if(i + 1 == alignment.maxAlignmentSteps){
                csutils::WRITE_INFO("Reached the steps limit");
                break;
            }            
            
            //evaluate movement
            if(finalZaxisP1 < finalZaxisP2){
                movement = - 2 * (deltaZ);
            }else if(finalZaxisP2 < finalZaxisP1){
                movement = + 2 * (deltaZ);
            }else{ // wrong state
                csutils::WRITE_ERROR("Failure during X axis alignment -> delta Z assessment");
            }
            
            //apply scaling
            if(deltaZ <= alignment.tresholding.thresholdFirst){
                movement = movement / (double)alignment.movement.firstStepDivider; 
            }else if(deltaZ <= alignment.tresholding.thresholdSecond){
                movement = movement / (double)alignment.movement.secondStepDivider;
            }else if(deltaZ <= alignment.tresholding.thresholdThird){
                movement = movement / (double)alignment.movement.thirdStepDivider;
            }     
            
            if(alignment.interuptAlignment.load()){
                csutils::WRITE_WARNING("Aborting alignment on the user request");
                break;
            }            

            //calculate new P
            requestedP = currentP + movement;
            if(requestedP < motion.axisP->geometricPositionMin->get()){
                if(!motion.axisP->run(csif::csmotion::ContinuousStage::Direction::NEGATIVE)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the P "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = true;
            }else if(requestedP > motion.axisP->geometricPositionMax->get()){
                if(!motion.axisP->run(csif::csmotion::ContinuousStage::Direction::POSITIVE)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the P "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = true;
            }else{
                //move P
                if(!motion.axisP->moveAbsolute(requestedP)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the P "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = false;
            }
            
            //finish he movement
            while(motion.axisP->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        
        //report
        csutils::WRITE_INFO("Finished the X axis alignment with deltaZ [mm]: " + 
        std::to_string(deltaZ));
        
        return true;
    }
    
}
}
