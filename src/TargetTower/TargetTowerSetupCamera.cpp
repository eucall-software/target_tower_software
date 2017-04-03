/*
 *************************************************************************
 * @file TargetTowerSetupCamera.cpp
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

    bool TargetTower::setupCamera(){
        
        //check state
        if(!currentStatus.compare(state::INITIALIZED)){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> wrong state");
            return false;
        }
        
        //check input
        if(camera.configuration.serialNumber.empty()){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> the serial number string is empty");
            return false;
        }
        
        if(camera.configuration.defaultExposure == 0.0){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> the default exposure is set to 0.0");
            return false;
        }
        
        csutils::WRITE_INFO("Starting the camera");
        
        //load library --------------------------------------------------
        try{
            camera.cam.attach(camera.configuration.library);
            camera.cam->clearData();
        }catch(csexception::CSRuntimeError &ex){
            csutils::WRITE_ERROR("Caught csexception in 'setupCamera' -> "
            "while loading the camera library. Exception: " + std::string(ex.what()));
            return false;
        }catch(std::exception &ex){
            csutils::WRITE_ERROR("Caught exception in 'setupCamera' -> "
            "while loading the camera library. Exception: " + std::string(ex.what()));
            return false;
        }
        
        //find camera-----------------------------------------------------------
        if(!camera.cam->discoverSpecific(camera.configuration.serialNumber)){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to find the camera with "
            "serial number: " + camera.configuration.serialNumber);
            return false;
        }
        
        //connect to camera-----------------------------------------------------
        if(!camera.cam->connect(0)){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to connect to the camera with "
            "serial number: " + camera.configuration.serialNumber);
            return false;
        }
        
        //set continuous software trigger acquisition---------------------------
        const std::string acqMode = "Grab (Continuous) acquisition with software trigger supported";
        const std::string silentMode = "silentMode";
        int32_t mode = -1;
        
        //check supported acq modes
        if(camera.cam->parameters.acquisitionSupport->get().empty()){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> no supported acquisition modes found");
            return false;
        }
        
        //find the continuous + sw trigger
        for (uint32_t i = 0; i < camera.cam->parameters.acquisitionSupport->get().size(); i++) {
            if(camera.cam->parameters.acquisitionSupport->get().at(i).compare(6,acqMode.size(),acqMode) == 0){
                mode = i;
                break;
            }
        }
        
        //check if found and set it
        if((mode < 0) || ((uint32_t)mode > camera.cam->parameters.acquisitionSupport->get().size())){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to find the "
            "continuous acquisition mode with software trigger");
            return false;
        }else{
            if(!camera.cam->loadAcquisitionConfiguration(mode)){
                csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to set the "
                "continuous acquisition mode with software trigger");
                return false;
            }
        }
        
        //configure camera------------------------------------------------------
        //set silent mode
        if(!camera.cam->writeParameter(silentMode,"true")){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to set the camera to silent mode");
            return false;
        }
        
        //check if camera is preconfigured
        if(!camera.configuration.preconfigured){ 
            //set exposure
            if(!camera.cam->writeParameter(camera.configuration.exposureName,
                    std::to_string(camera.configuration.defaultExposure))){
                csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to set the camera exposure");
                return false;
            }

            //set pixel mode
            if(!camera.cam->writeParameter(camera.configuration.pixelFormatName,
                    camera.configuration.pixelFormatValue)){
                csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to set the camera to Mono8 "
                "pixel format");
                return false;
            }
        }
        
        //start streaming ------------------------------------------------------
        if(!camera.cam->startAcq()){
            csutils::WRITE_ERROR("Aborting 'setupCamera' -> failed to start the acquisition");
            return false;
        }
        
        //report----------------------------------------------------------------
        csutils::WRITE_INFO("The Target Tower camera is up and running");
        
        return true;
    }
    
}
}
