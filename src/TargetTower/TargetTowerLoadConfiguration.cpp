/*
 *************************************************************************
 * @file TargetTowerLoadConfiguration.cpp
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

    bool TargetTower::loadConfigurationFromFile(const std::string& xmlFilePath){
        
        //check state
        if(!currentStatus.compare(state::INITIALIZED)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> wrong state");
            return false;
        }
        
        //check input
        if(xmlFilePath.empty()){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> the path string is empty");
            return false;
        }
        
        csutils::WRITE_INFO("Loading configuration from the file: " + xmlFilePath);
        
        //set local variables --------------------------------------------------
        std::string configurationX = "";
        std::string configurationY = "";
        std::string configurationZ = "";
        std::string configurationT = "";
        std::string configurationP = "";
        int16_t preconfigured = 0;
        uint32_t takeImagesH1 = 0;
        
        //device configuration
        csbase::DeviceConfiguration mainConfig;
        mainConfig.setName("Target Tower");
        mainConfig.setManufacturer("ELI");
        mainConfig.setDeviceType("target tower");
        mainConfig.setSerialNumber("N/A");
        //component configuration
        csbase::ComponentConfiguration motionConfig;
        motionConfig.setName("motion");
        csbase::ComponentConfiguration cameraConfig;
        cameraConfig.setName("camera");
        csbase::ComponentConfiguration alignmentConfig;
        alignmentConfig.setName("alignment");
        csbase::ComponentConfiguration alignmentConfig2;
        alignmentConfig2.setName("alignment2");
        csbase::ComponentConfiguration analysisConf;
        analysisConf.setName("analysis");
                
        //set configuration-----------------------------------------------------
        mainConfig.registerParameter<std::string>("motionType",
                                         "",&motion.configuration.type, false);
        mainConfig.registerParameter<double>("defaultX",
                                         "",&motion.configuration.defaultX, false);
        mainConfig.registerParameter<double>("defaultY",
                                         "",&motion.configuration.defaultY, false);
        mainConfig.registerParameter<double>("defaultZ",
                                         "",&motion.configuration.defaultZ, false);
        mainConfig.registerParameter<double>("defaultT",
                                         "",&motion.configuration.defaultT, false);
        mainConfig.registerParameter<double>("defaultP",
                                         "",&motion.configuration.defaultP, false);
        mainConfig.registerParameter<uint32_t>("imagePoolPeriod",
                                         "",&alignment.imagePoolPeriod, false);
        // register sub configurations
        mainConfig.registerSubConfiguration(&motionConfig);
        mainConfig.registerSubConfiguration(&cameraConfig);
        mainConfig.registerSubConfiguration(&alignmentConfig);
        mainConfig.registerSubConfiguration(&alignmentConfig2);
        mainConfig.registerSubConfiguration(&analysisConf);
        
        //set subconfigurations
        motionConfig.registerParameter<std::string>("configurationX",
                                         "",&configurationX, false);
        motionConfig.registerParameter<std::string>("configurationY",
                                         "",&configurationY, false);
        motionConfig.registerParameter<std::string>("configurationZ",
                                         "",&configurationZ, false);
        motionConfig.registerParameter<std::string>("configurationT",
                                         "",&configurationT, false);
        motionConfig.registerParameter<std::string>("configurationP",
                                         "",&configurationP, false);
        
        cameraConfig.registerParameter<std::string>("serialNumber",
                                         "",&camera.configuration.serialNumber, false);
        cameraConfig.registerParameter<double>("defaultExposure",
                                         "",&camera.configuration.defaultExposure, false);
        cameraConfig.registerParameter<int16_t>("preconfigured",
                                         "",&preconfigured, false);
        
        alignmentConfig.registerParameter<double>("p1x",
                                         "",&alignment.alignmentPoints.p1.x, false);
        alignmentConfig.registerParameter<double>("p1y",
                                         "",&alignment.alignmentPoints.p1.y, false);
        alignmentConfig.registerParameter<double>("p2x",
                                         "",&alignment.alignmentPoints.p2.x, false);
        alignmentConfig.registerParameter<double>("p2y",
                                         "",&alignment.alignmentPoints.p2.y, false);
        alignmentConfig.registerParameter<double>("p3x",
                                         "",&alignment.alignmentPoints.p3.x, false);
        alignmentConfig.registerParameter<double>("p3y",
                                         "",&alignment.alignmentPoints.p3.y, false);
        alignmentConfig.registerParameter<double>("fineRange",
                                         "",&alignment.movement.range, false);
        alignmentConfig.registerParameter<double>("mildRange",
                                         "",&alignment.movement.mildRange, false);
        alignmentConfig.registerParameter<double>("deltaZLimit",
                                         "",&alignment.movement.deltaZLimit, false);
        alignmentConfig.registerParameter<double>("distancePaxis",
                                         "",&alignment.movement.distancePaxis, false);
        alignmentConfig.registerParameter<uint32_t>("maxAlignmentSteps",
                                         "",&alignment.maxAlignmentSteps, false);
        alignmentConfig.registerParameter<uint32_t>("firstStepDivider",
                                         "",&alignment.movement.firstStepDivider, false);
        alignmentConfig.registerParameter<uint32_t>("secondStepDivider",
                                         "",&alignment.movement.secondStepDivider, false);
        alignmentConfig.registerParameter<uint32_t>("thirdStepDivider",
                                         "",&alignment.movement.thirdStepDivider, false);
        alignmentConfig.registerParameter<double>("h1x",
                                         "",&alignment.alignmentPoints.holder1.h1.x, false);
        alignmentConfig.registerParameter<double>("h1y",
                                         "",&alignment.alignmentPoints.holder1.h1.y, false);
        alignmentConfig.registerParameter<double>("h2x",
                                         "",&alignment.alignmentPoints.holder1.h2.x, false);
        alignmentConfig.registerParameter<double>("h2y",
                                         "",&alignment.alignmentPoints.holder1.h2.y, false);
        alignmentConfig.registerParameter<double>("h3x",
                                         "",&alignment.alignmentPoints.holder1.h3.x, false);
        alignmentConfig.registerParameter<double>("h3y",
                                         "",&alignment.alignmentPoints.holder1.h3.y, false);
        alignmentConfig.registerParameter<double>("h4x",
                                         "",&alignment.alignmentPoints.holder1.h4.x, false);
        alignmentConfig.registerParameter<double>("h4y",
                                         "",&alignment.alignmentPoints.holder1.h4.y, false);
        
        alignmentConfig2.registerParameter<double>("fineRange",
                                         "",&alignment.movementAlt.fineRange, false);
        alignmentConfig2.registerParameter<double>("roughRange",
                                         "",&alignment.movementAlt.roughRange, false);
        alignmentConfig2.registerParameter<double>("zRange",
                                         "",&alignment.movementAlt.zRange, false);
        
        analysisConf.registerParameter<double>("offsetX",
                                         "",&analysis.holder1.offsetX, false);
        analysisConf.registerParameter<double>("offsetY",
                                         "",&analysis.holder1.offsetY, false);
        analysisConf.registerParameter<double>("s1x",
                                         "",&analysis.holder1.points.s_1_1.x, false);
        analysisConf.registerParameter<double>("s1y",
                                         "",&analysis.holder1.points.s_1_1.y, false);
        analysisConf.registerParameter<uint32_t>("imagesH1",
                                         "",&takeImagesH1, false);
        analysisConf.registerParameter<uint32_t>("samplesX",
                                         "",&analysis.holder1.samplesX, false);
        analysisConf.registerParameter<uint32_t>("samplesY",
                                         "",&analysis.holder1.samplesY, false);
        
        //load configuration----------------------------------------------------
        try{
            mainConfig.deserializeXMLFromFile(xmlFilePath);
        }catch(csexception::CSRuntimeError &ex){
            csutils::WRITE_ERROR("Caught csexception in 'loadConfigurationFromFile' -> "
            "while loading the configuration file. Exception: " + std::string(ex.what()));
            return false;
        }catch(std::exception &ex){
            csutils::WRITE_ERROR("Caught exception in 'loadConfigurationFromFile' -> "
            "while loading the configuration file");
            return false;
        }
        
        //set camera------------------------------------------------------------
        if(preconfigured == 1){ // process the preconfigured flag form configuration
            camera.configuration.preconfigured = true;
        }
        
        if(!setupCamera()){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the target tower camera");
            return false;
        }
        
        //set motion------------------------------------------------------------
        if(!setupAxis(axis::X, configurationX)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the axis X");
            return false;
        }
        if(!setupAxis(axis::Y, configurationY)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the axis Y");
            return false;
        }
        if(!setupAxis(axis::Z, configurationZ)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the axis Z");
            return false;
        }
        if(!setupAxis(axis::T, configurationT)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the axis T");
            return false;
        }
        if(!setupAxis(axis::P, configurationP)){
            csutils::WRITE_ERROR("Aborting 'loadConfigurationFromFile' -> failed to setup "
            "the axis P");
            return false;
        }
        
        //misc
        if(takeImagesH1 == 1){
            analysis.holder1.takeImages = true;
        }
        
        //change state----------------------------------------------------------
        currentStatus.set(state::CONFIGURED);
        
        //report
        csutils::WRITE_INFO("Configuration of the Target Tower is done");    
        
        return true;
    }
    
}
}
