/*
 *************************************************************************
 * @file TargetTowerAnalyzeHolder.cpp
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

    bool TargetTower::analyzeHolder(uint32_t holder){
        
        //check state
        if(!currentStatus.compare(state::ANALYSING)){
            csutils::WRITE_ERROR("Aborting 'analyzeHolder' -> wrong state");
            return false;
        }
        
        //check holder no
        if(holder > analysis.noOfHolders){
            csutils::WRITE_ERROR("Aborting 'analyzeHolder' -> the provided number of holder is incorrect");
            return false;
        }
        
        //----------------------------------------------------------------------
        //locals
        bool movingDown = true; // if moving down or up
        csimage::MonoImage* image = nullptr;
        double p1FocusZrough = 0.0;
        double p1FocusZfine = 0.0;
        bool takeImages = false;
        double offsetX = 0.0;
        double offsetY = 0.0;
        uint32_t noSamplesX = 0;
        uint32_t noSamplesY = 0;
        AlignmentPoint p1;
        std::string imageNameHolder = "holder";
        std::string imageNameSample = "_sample";
        std::string imageNameCoordinatesX = "";
        std::string imageNameCoordinatesY = "";
        
        //check current holder
        switch(holder){
            case 1:
                if(!analysis.holder1.aligned){
                    csutils::WRITE_ERROR("Aborting 'analyzeHolder' -> the requested "
                    "holder is not aligned");
                    return false;
                }
                
                //ok to go
                takeImages = analysis.holder1.takeImages;
                offsetX = analysis.holder1.offsetX;
                offsetY = analysis.holder1.offsetY;
                noSamplesX = analysis.holder1.samplesX;
                noSamplesY = analysis.holder1.samplesY;
                p1 = analysis.holder1.points.s_1_1;
                imageNameHolder += "_1";
                
                break;
            default:
                csutils::WRITE_ERROR("Aborting 'analyzeHolder' -> the requested "
                "holder analysis is not supported");
                return false;
        }
        
        //analyze---------------------------------------------------------------
        
        //move to p1 (expected center)
        if(!moveToPoint(p1)){
            csutils::WRITE_ERROR("Failed to move to the first sample during 'analyzeHolder'");
            return false;
        }
        
        //find focus 
        if(!roughFocus(p1FocusZrough) || p1FocusZrough == 0.0){
            csutils::WRITE_ERROR("Failed to move to find the rough focus for "
            "the first sample during 'analyzeHolder'");
            return false;
        }
        if(!fineFocus(p1FocusZrough,p1FocusZfine) || p1FocusZfine == 0.0){
            csutils::WRITE_ERROR("Failed to move to find the fine focus for the "
            "first sample during 'analyzeHolder'");
            return false;
        }
        
        //move around
        //x axis
        for (uint32_t i = 1; i <= noSamplesX; i++) {
            imageNameCoordinatesX.clear();
            imageNameCoordinatesX = "_X" + std::to_string(i);
            
            //y axis
            if(movingDown){
                for (uint32_t j = 0; j < noSamplesY; j++) {  
                    imageNameCoordinatesY.clear();
                    imageNameCoordinatesY = "_Y" + std::to_string(j+1);

                    //delay for image
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    
                    //get image && save image
                    if(!retrieveLatestImage(&image) || !image){
                        csutils::WRITE_ERROR("Failed to extract the image");
                    }else{
                        if(!csimage::savePNG(imageNameHolder + imageNameSample + 
                                imageNameCoordinatesX + imageNameCoordinatesY + ".png" , *image)){
                                csutils::WRITE_ERROR("Failed to save the image of the first sample");
                        }
                    }

                    //check for interruption
                    if(analysis.interuptAnalysis.load()){
                        csutils::WRITE_WARNING("Interrupted the analysis on the user request");
                        return false;
                    }

                    //move to the next Y (down is +)
                    if(!motion.axisY->moveRelative(1 * offsetY)){
                        csutils::WRITE_ERROR("Failed to move in the Y axis during 'analyzeHolder'");
                        return false;
                    }
                    while(motion.axisY->isOperational()){
                        std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
                    }
                    
                } // for j end
                //now will go up
                movingDown = false;
            }else{
                for (uint32_t j = noSamplesY -1 ; j > 0; j--) {
                    imageNameCoordinatesY.clear();
                    imageNameCoordinatesY = "_Y" + std::to_string(j+1);

                    //delay for image
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    
                    //get image && save image
                    if(!retrieveLatestImage(&image) || !image){
                        csutils::WRITE_ERROR("Failed to extract the image");
                    }else{
                        if(!csimage::savePNG(imageNameHolder + imageNameSample + 
                                imageNameCoordinatesX + imageNameCoordinatesY + ".png" , *image)){
                                csutils::WRITE_ERROR("Failed to save the image of the first sample");
                        }
                    }

                    //check for interruption
                    if(analysis.interuptAnalysis.load()){
                        csutils::WRITE_WARNING("Interrupted the analysis on the user request");
                        return false;
                    }

                    //move to the next Y (up is -)
                    if(!motion.axisY->moveRelative( - 1 * offsetY)){
                        csutils::WRITE_ERROR("Failed to move in the Y axis during 'analyzeHolder'");
                        return false;
                    }
                    while(motion.axisY->isOperational()){
                        std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
                    }
                    
                } // for j end
                //now will go down
                movingDown = true;
            }// if down end
            
            //move in the x by offset right mean -
            if(!motion.axisX->moveRelative( -1 * offsetX)){
                csutils::WRITE_ERROR("Failed to move in the X axis during 'analyzeHolder'");
                return false;
            }
            while(motion.axisX->isOperational()){
                std::this_thread::sleep_for(std::chrono::milliseconds(motion.configuration.movementPollPeriod));
            }
        }// for x end
        
        //report
        csutils::WRITE_INFO("Finished the analysis of the sample holder no." + std::to_string(holder));
        
        return true;
    }
    
}
}
