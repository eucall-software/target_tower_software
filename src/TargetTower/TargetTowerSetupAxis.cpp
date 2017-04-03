/*
 *************************************************************************
 * @file TargetTowerSetupAxis.cpp
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

    bool TargetTower::setupAxis(axis id, const std::string& configurationFile){
        
        //check state
        if(!currentStatus.compare(state::INITIALIZED)){
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> wrong state");
            return false;
        }
        
        //check input
        if(configurationFile.empty()){
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> the configuration file string is empty");
            return false;
        }
        
        //local axis var
        csbase::Plugin<csif::csmotion::ContinuousStage>* stage = nullptr; // ptr to the axis
        std::string axisUsed = "";
        std::string library = "";
        
        //determine axis
        switch (id){
            case axis::X:
                stage = &motion.axisX;
                axisUsed = "X";
                break;
            case axis::Y:
                stage = &motion.axisY;
                axisUsed = "Y";
                break;
            case axis::Z:
                stage = &motion.axisZ;
                axisUsed = "Z";
                break;
            case axis::T:
                stage = &motion.axisT;
                axisUsed = "T";
                break;
            case axis::P:
                stage = &motion.axisP;
                axisUsed = "P";
                break;    
            default:    
                csutils::WRITE_ERROR("Aborting 'setupAxis' -> failed to identify the axis");
                return false;
        }
        
        //check the ptr
        if(!stage){
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> failed to identify the axis. Pointer to axis is null");
            return false;
        }
        
        //----------------------------------------------------------------------
        
        csutils::WRITE_INFO("Setting up the axis " + axisUsed);
        
        //determine library ---------------------------------------------------------
        if(motion.configuration.type == motion.configuration.supportedTypes.phidget){
            library = motion.configuration.supportedTypes.phidgetLib;
            if(id == axis::T){
                library = motion.configuration.supportedTypes.phidgetLibAlt;
            }
        }else{
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> failed to identify the motion type");
            return false;
        }
        
        //load library ---------------------------------------------------------
        try{
            stage->attach(library);
            stage->get()->getID();
        }catch(csexception::CSRuntimeError &ex){
            csutils::WRITE_ERROR("Caught csexception in 'setupCamera' -> "
            "while loading the camera library. Exception: " + std::string(ex.what()));
            return false;
        }catch(std::exception &ex){
            csutils::WRITE_ERROR("Caught exception in 'setupCamera' -> "
            "while loading the camera library. Exception: " + std::string(ex.what()));
            return false;
        }
        
        //pass the configuration
        if(!stage->get()->loadConfigurationFromFile(configurationFile)){
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> the axis object failed "
            "to load the configuration file");
            return false;
        }
        
        //connect to the axis
        if(!stage->get()->connect()){
            csutils::WRITE_ERROR("Aborting 'setupAxis' -> the axis object failed "
            "to connect to the motion hardware");
            return false;
        }
        
        //----------------------------------------------------------------------
        
        csutils::WRITE_INFO("The axis " + axisUsed + " is up and configured");
        
        return true;
    }
    
}
}
