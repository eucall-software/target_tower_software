/*
 *************************************************************************
 * @file TargetTowerAlignFrameY.cpp
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

    bool TargetTower::alignFrameY(){
        
        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting 'alignFrameY' -> wrong state");
            return false;
        }
        
        //locals
        double finalZaxisP1 = 0.0, finalZaxisP3 = 0.0;
        double distanceP1 = 0.0, distanceP3 = 0.0;
//        double finalZaxisP1alt = 0.0, finalZaxisP3alt = 0.0;
        double movement = 0.0, deltaZ = 0.0;
        double currentT = 0.0, requestedT = 0.0;
        bool reachedLimit = false;
        bool startFromTop = true; // if to find focus for P1 first
        bool repeated = false;
        
        //----------------------------------------------------------------------
        for (uint32_t i = 0; i < alignment.maxAlignmentSteps; i++) {
                
            if(alignment.interuptAlignment.load()){
                csutils::WRITE_WARNING("Aborting alignment on the user request");
                break;
            }
            
            csutils::WRITE_INFO("Attempt no. " + std::to_string(i+1) + " to minimize delta Z on the axis Y");
            
            if(startFromTop){
                //focus at P1
                if(!focusAtPoint(alignment.alignmentPoints.p1, finalZaxisP1, repeated)){
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

                //focus at P3
                if(!focusAtPoint(alignment.alignmentPoints.p3, finalZaxisP3, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P3");
                    return false;
                }
                if(finalZaxisP3 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                //just moved down
                startFromTop = false;
                
            }else{
                //focus at P3
                if(!focusAtPoint(alignment.alignmentPoints.p3, finalZaxisP3, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P3");
                    return false;
                }
                if(finalZaxisP3 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                if(alignment.interuptAlignment.load()){
                    csutils::WRITE_WARNING("Aborting alignment on the user request");
                    break;
                }
                
                //focus at P1
                if(!focusAtPoint(alignment.alignmentPoints.p1, finalZaxisP1, repeated)){
                    csutils::WRITE_ERROR("Failed to find the focus for the point P1");
                    return false;
                }
                if(finalZaxisP1 == 0.0){
                    csutils::WRITE_ERROR("The returned focus Z axis position is incorrect");
                    return false;
                }
                
                //just moved up
                startFromTop = true;
            }
            
            if(!repeated){
                repeated = true;
            }

            //calculate
            distanceP1 = fabs(alignment.movement.distancePaxis - alignment.alignmentPoints.p1.x);
            distanceP3 = fabs(alignment.movement.distancePaxis - alignment.alignmentPoints.p3.x);
//            finalZaxisP1alt = finalZaxisP1 * (distanceP1/(distanceP1 + distanceP3));
//            finalZaxisP3alt = finalZaxisP3 * (distanceP3/(distanceP1 + distanceP3));
            deltaZ = fabs(finalZaxisP1-finalZaxisP3);
            currentT = motion.axisT->position->get();
            
            //save delta
            parameters.alignment.deltaZY_internal = deltaZ;
        
            //summary
            std::cout << "--------Summary for alignment run no "<< i+1 << "---------" << std::endl;
            std::cout << "P1 focus spot [mm]: " << finalZaxisP1 << std::endl;
            std::cout << "P3 focus spot [mm]: " << finalZaxisP3 << std::endl;
//            std::cout << "P1 focus spot recalculated [mm]: " << finalZaxisP1alt << std::endl;
//            std::cout << "P2 focus spot recalculated [mm]: " << finalZaxisP3alt << std::endl;
            std::cout << "Z delta recalculated [mm]: " << deltaZ << std::endl;
            std::cout << "T is [mm]: " << currentT << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;
            
            //check for exit condition
            if(deltaZ <= alignment.movement.deltaZLimit ){ // finished
                csutils::WRITE_INFO("The delta Z for Y axis is in the limit (30 um)");
                break;
            }else if(reachedLimit){
                csutils::WRITE_INFO("The Y axis reached the limit");
                break;
            }
            
            //last run?
            if(i + 1 == alignment.maxAlignmentSteps){
                csutils::WRITE_INFO("Reached the steps limit");
                break;
            }
            
            //evaluate movement
            if(finalZaxisP1 < finalZaxisP3){
                movement = 1 * (deltaZ);
            }else if(finalZaxisP3 < finalZaxisP1){
                movement = -1 * (deltaZ);
            }else{ // wrong state
                csutils::WRITE_ERROR("Failure during Y axis alignment -> delta Z assessment");
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
            requestedT = currentT + movement;
            if(requestedT < motion.axisT->geometricPositionMin->get()){
                if(!motion.axisP->run(csif::csmotion::ContinuousStage::Direction::NEGATIVE)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the T "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = true;
            }else if(requestedT > motion.axisT->geometricPositionMax->get()){
                if(!motion.axisP->run(csif::csmotion::ContinuousStage::Direction::POSITIVE)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the T "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = true;
            }else{
                //move P
                if(!motion.axisT->moveAbsolute(requestedT)){
                    csutils::WRITE_ERROR("Failed to issue the correction move on the T "
                    "axis while X axis alignment");
                    return false;
                }
                reachedLimit = false;
            }
            
            //finish he movement
            while(motion.axisT->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }
        
        //report
        csutils::WRITE_INFO("Finished the Y axis alignment with deltaZ [mm]: " + 
        std::to_string(deltaZ));
        
        return true;
    }
    
}
}
