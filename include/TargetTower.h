/*
 *************************************************************************
 * @file TargetTower.h
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

#ifndef TARGETTOWER_H
#define	TARGETTOWER_H

#include <thread>   //std::thread
#include <atomic>   //std::atomic<bool>   
#include <future>   // futures
#include <memory>   //std::unique_ptr

//cs stuff ---------------------------------------------------------------------
#include <csplugin.h>
#include <csbase.h>
#include <csutils.h>
#include <Camera.h>
#include <ContinuousStage.h>
#include <ComponentConfiguration.h>
#include <DeviceConfiguration.h>

//point for alignment-----------------------------------------------------------
#include "AlignmentPoint.h"

namespace csrp3{
namespace targetTower{
 
class TargetTower : public csbase::Component{
public:
    //the axis identification
    enum class axis{
        X = 0,
        Y = 1,
        Z = 2,
        T = 3,
        P = 4
    };
public:
    /**
     * Constructor
     */
    TargetTower();
    /**
     * Copy constructor
     */
    TargetTower(const TargetTower& orig) = delete;
    /**
     *  Move constructor
     */
    TargetTower(TargetTower&& orig) = delete;
    /**
     * Assignment operator
     */
    TargetTower& operator=(const TargetTower& right) = delete;
    /**
     * Move operator
     */
    TargetTower& operator=(TargetTower&& right) = delete;
    /**
     * Destructor
     */
    ~TargetTower();

public:

    /**
     * Clears the current configuration
     * @return return flag
     */
    bool clearConfiguration();
    
    /**
     * Homes the Target Tower and then moves to default positions for each axis
     * Background process
     * @return return flag
     */
    bool home();
    
    /**
     * Starts the frame aligning process for the selected axis and holder
     * Background process
     * @param alignX - if to align axis X
     * @param alignY - if to align axis Y 
     * @param holder - holder to align 
     * @return return flag
     */
    bool align(bool alignX = true, bool alignY = true,  uint32_t holder = 0);
    
    /**
     * Runs analysis on the specified holder. Can be run only on holders with  finished alignment.
     * Will find center of all samples on the plate also finds the focus point for each sample.
     * Runs in a background thread
     * @param holder the number of holder to analyze. Default == 0 -> means analyzes everything
     * @return return flag
     */
    bool analyze(uint32_t holder = 0);
    
    /**
     * Disconnects from the devices and cleans the configuration
     * @return return flag
     */
    bool disconnect();
    
public: // move cmnds

    /**
     * Starts the absolute movement on specified axis; background process
     * @param id - the axis to move csrp3::targetTower::axis type
     * @param position - the absolute position to move to [mm]
     * @return return flag
     */
    bool moveAbsolute(axis id, const double& position);

    /**
     * Starts the relative movement on specified axis; background process
     * @param id - the axis to move csrp3::targetTower::axis type
     * @param distance - the relative distance to move about [mm]
     * @return return flag
     */
    bool moveRelative(axis id, const double& distance);
    
    /**
     * Starts moving towards a axis limit; background process
     * @param id - the axis to move csrp3::targetTower::axis type
     * @param positiveDirection - The movement direction {true == moves to maximum}
     * @return return flag
     */
    bool run(axis id, const bool& positiveDirection);    
    
    /**
     * Aborts the current movement based process {HOMING, ALIGNING, ANALYSING, MOVING}
     * @return return flag
     */
    bool abort();    
    
    /**
     * Finds the best focus for the current position
     * @return 
     */
    bool focus();
    
public:

    /**
     * Return a pointer to the latest image
     * @param image the latest image
     * @return return flag
     */
    bool retrieveLatestImage(csimage::MonoImage** image);

    /**
     * Returns the current X axis position; Available after homing
     * @param position - the current position [mm]
     * @return return flag
     */
    bool getPositionX(double& position);
    
    /**
     * Returns the current Y axis position; Available after homing
     * @param position - the current position [mm]
     * @return return flag
     */
    bool getPositionY(double& position);

    /**
     * Returns the current Z axis position; Available after homing
     * @param position - the current position [mm]
     * @return return flag
     */
    bool getPositionZ(double& position);

    /**
     * Returns the current P axis position; Available after homing
     * @param position - the current position [mm]
     * @return return flag
     */
    bool getPositionP(double& position);

    /**
     * Returns the current T axis position; Available after homing
     * @param position - the current position [mm]
     * @return return flag
     */
    bool getPositionT(double& position);    
    
public: // inherited from csbase::Component
            
    /**
     * Returns the id of the component.
     * @return the id of the component
     */
    uint32_t getID() const override{
        return csbase.id;
    }
    
    /**
     * Sets the id of the component.
     * @param id - the new id 
     * @return return flag
     */
    bool setID(const uint32_t id) override{
        csbase.id = id;
        return true;
    }
            
    /**
     * Tells if the component is configurable.
     * @return true if configurable
     */
    bool isConfigurable() override{
        return csbase.configurable;
    }

    /**
     * Loads configuration from a file.
     * @param xmlFilePath - the configuration file path; Local file 
     * @return return flag;
     */
    bool loadConfigurationFromFile(const std::string& xmlFilePath) override;

public: // states
    
    /**
     * States if the device is in INITIALIZED state
     * @return true if in INITIALIZED state
     */
    bool isInitialized(){
        return currentStatus.compare(state::INITIALIZED);
    }
    
    /**
     * States if the device is in CONFIGURED state
     * @return true if in CONFIGURED state
     */
    bool isConfigured(){
        return currentStatus.compare(state::CONFIGURED);
    }
    
    /**
     * States if the device is in HOMING state
     * @return true if in HOMING state
     */
    bool isHoming(){
        return currentStatus.compare(state::HOMING);
    }
    
    /**
     * States if the device is in ALIGNING state
     * @return true if in ALIGNING state
     */
    bool isAligning(){
        return currentStatus.compare(state::ALIGNING);
    }
    
    /**
     * States if the device is in READY state
     * @return true if in READY state
     */
    bool isReady(){
        return currentStatus.compare(state::READY);
    }
    
    /**
     * States if the device is in ANALYSING state
     * @return true if in ANALYSING state
     */
    bool isAnalysing(){
        return currentStatus.compare(state::ANALYSING);
    }
    
    /**
     * States if the device is in MOVING state
     * @return true if in MOVING state
     */
    bool isMoving(){
        return currentStatus.compare(state::MOVING);
    }
    
public: // not implemented features
    
    /**
     * Loads configuration from a database. Not Implemented.
     * @return false;
     */
    bool loadConfigurationFromDB() override{
        csutils::WRITE_ERROR("The function 'loadConfigurationFromDB' is not implemented");
        return false;
    }
    
    /**
     * Loads configuration from a stream. Not Implemented.
     * @return false;
     */
    bool loadConfigurationFromStream(const std::string& xmlStream) override{
        (void)xmlStream;
        csutils::WRITE_ERROR("The function 'loadConfigurationFromStream' is not implemented");
        return false;
    }
        
    /**
     * Saves configuration to a database. Not Implemented.
     * @return false;
     */
    bool saveConfigurationToDB() override{
        csutils::WRITE_ERROR("The function 'saveConfigurationToDB' is not implemented");
        return false;
    }

    /**
     * Saves configuration to a file. Not Implemented.
     * @return false;
     */    
    bool saveConfigurationToFile(std::string& xmlFilePath) override{
        (void)xmlFilePath;
        csutils::WRITE_ERROR("The function 'saveConfigurationToFile' is not implemented");
        return false;
    }

    /**
     * Saves configuration to a stream. Not Implemented.
     * @return false;
     */    
    bool saveConfigurationToStream(std::string& xmlStream) override{
        (void)xmlStream;
        csutils::WRITE_ERROR("The function 'saveConfigurationToStream' is not implemented");
        return false;
    }
    
private: // private implementations

    /**
     * Sets up the camera of the TT. Connects, configures and starts continuous streaming
     * @return return flag
     */
    bool setupCamera();
    
    /**
     * Sets up the axis of the TT. The function configures and connects the/to motion hardware 
     * @param id - id of the axis {X,Y,Z,T,P}
     * @param configurationFile - configuration file for the axis
     * @return return flag
     */
    bool setupAxis(axis id, const std::string& configurationFile);
    
    /**
     * Aligns the X axis of the frame
     * @return return flag
     */
    bool alignFrameX();
    
    /**
     * Aligns the Y axis of the frame
     * @return return flag
     */
    bool alignFrameY();    

    /**
     * Aligns the X axis of the holder1
     * @return return flag
     */
    bool alignHolderX();

    /**
     * Aligns the Y axis of the holder1
     * @return return flag
     */
    bool alignHolderY();  
    
    /**
     * Runs analysis of the sample holder
     * @param holder - the number of the sample holder
     * @return  return flag
     */
    bool analyzeHolder(uint32_t holder);
    
    /**
     * Moves to a certain alignment point and find the best Z axis position based on focus
     * @param point - the alignment point
     * @param resultingZ - the Z position with the best focus
     * @return return flag
     */
    bool focusAtPoint(const AlignmentPoint& point, double& resultingZ, bool repeated=false);
    
    /**
     * Looks at the current x,y positions for the best focus.
     * Moves through the whole Z axis range. Returns the best z axis position in terms of focus
     * @param finalZ - the best Z axis position
     * @return return flag
     */
    bool roughFocus(double& finalZ);
    
    /**
     * Looks at the current x,y positions for the best focus using axis P.
     * Moves through the whole P axis rough range. Returns the best P axis position in terms of focus
     * @param finalP - the best P axis position
     * @return return flag
     */
    bool roughFocusP(double& finalP);
    
    /**
     * Looks at the current x,y positions for the best focus.
     * Same as 'roughFocus' but in a limited range (configurable), starting from provided z axis position
     * @param startingZ - the starting Z axis position
     * @param finalZ - the best Z axis position
     * @return return flag
     */
    bool mildFocus(const double& startingZ, double& finalZ);
    
    /**
     * Looks at the current x,y positions for the best focus.
     * Moves through a limited (configuration) Z axis range at small steps. Returns the best z axis position in terms of focus
     * @param startingZ - the starting Z axis position
     * @param finalZ - the best Z axis position
     * @return return flag
     */
    bool fineFocus(const double& startingZ, double& finalZ);
    
    /**
     * Looks at the current x,y positions for the best focus.
     * Moves through a limited (configuration) P axis range at small steps. Returns the best P axis position in terms of focus
     * @param startingP - the starting P axis position
     * @param finalP - the best P axis position
     * @return return flag
     */
    bool fineFocusP(const double& startingP, double& finalP);
        
    
    /**
     * Evaluates the focus of the provided image
     * @param image - the image to analyze
     * @param focus - the focus value of the image
     * @return return flag
     */
    bool evaluateFocus(const csimage::MonoImage& image, double& focus);
    
    /**
     * Moves to a defined alignment point
     * @param point - the alignment point
     * @return return flag
     */
    bool moveToPoint(const AlignmentPoint& point);
    
private: // threading
    
    /**
     * Starts the homing thread
     * @return return flag
     */
    bool threadHomingStart();
    
    /**
     * Stops the homing thread
     * @return return flag
     */
    bool threadHomingStop();
    
    /**
     * The function executing by the homing thread
     */
    void threadHomingFce();
    
    /**
     * Starts the alignment thread
     * @return return flag
     */
    bool threadAlignmentStart(bool alignX = true, bool alignY = true,  uint32_t holder = 0);
    
    /**
     * Stops the alignment thread
     * @return return flag
     */
    bool threadAlignmentStop();
    
    /**
     * The function executing by the alignment thread
     */
    void threadAlignmentFce(bool alignX = true, bool alignY = true,  uint32_t holder= 0);    
    
    /**
     * Starts the movement thread
     * @return return flag
     */
    bool threadMovingStart(axis id);
    
    /**
     * Stops the movement thread
     * @return return flag
     */
    bool threadMovingStop();
    
    /**
     * The function executing by the movement thread
     * @param id - the axis id
     */
    void threadMovingFce(axis id);  

    /**
     * Starts the focusing thread
     * @return return flag
     */
    bool threadFocusingStart();
    
    /**
     * Stops the focusing thread
     * @return return flag
     */
    bool threadFocusingStop();
    
    /**
     * The function executing by the focusing thread
     * @param id - the axis id
     */
    void threadFocusingFce();      
    
    /**
     * Starts the analysis thread
     * @return return flag
     */
    bool threadAnalysisStart(uint32_t holder = 0);
    
    /**
     * Stops the analysis thread
     * @return return flag
     */
    bool threadAnalysisStop();
    
    /**
     * The function executing by the analysis thread
     * @param id - the axis id
     */
    void threadAnalysisFce(uint32_t holder = 0);      

private:
    
    struct{
        uint32_t id; // id of the component
        bool configurable = true; // is the component configurable??
    }csbase; //csbase related variables
    
    struct{
        csbase::Plugin<csif::csmotion::ContinuousStage> axisX; // the axis x object
        csbase::Plugin<csif::csmotion::ContinuousStage> axisY; // the axis y object
        csbase::Plugin<csif::csmotion::ContinuousStage> axisZ; // the axis z object
        csbase::Plugin<csif::csmotion::ContinuousStage> axisT; // the axis tip object
        csbase::Plugin<csif::csmotion::ContinuousStage> axisP; // the axis pitch object
        struct{
            std::string type = ""; // the type of motion hardware
            double defaultX = 0.0; // the default position for axis X (after homing)
            double defaultY = 0.0; // the default position for axis Y (after homing)
            double defaultZ = 0.0; // the default position for axis Z (after homing)
            double defaultT = 0.0; // the default position for axis T (after homing)
            double defaultP = 0.0; // the default position for axis P (after homing)
            const uint32_t movementPollPeriod = 100; // poll period for movement [ms]
            struct{
                const std::string phidget = "phidget";
                const std::string phidgetAlt = "phidgetSS";
                const std::string phidgetLib = "libcontinuousStage_Phidget_Stepper.so";
                const std::string phidgetLibAlt = "libcontinuousStage_Phidget_StepperSS.so";
            }supportedTypes;
        }configuration; // configuration variables for the motion
    }motion; //motion related variables
    
    struct{
        csbase::Plugin<csif::cscamera::Camera> cam; // the camera object
        struct{
            bool preconfigured = false;
            const std::string library = "libcscamera_genicam.so"; // the cscamera library
            std::string serialNumber = ""; // the serial number of the camera for the TT
            const std::string exposureName = "ExposureTime"; // the exposure name 
            double defaultExposure = 0.0; // the default exposure time for the TT camera
            const std::string pixelFormatName = "PixelFormat"; // the pixel format parameter
            const std::string pixelFormatValue = "Mono8"; // the pixel format value
        }configuration; // configuration variables for the camera
    }camera; //camera related variables
    
    struct{
        struct{
            std::thread threadHoming; // the thread executing homing
            std::atomic<bool> threadHomingUp; // if the thread is up
            std::atomic<bool> threadHomingKill; // if the thread is top be terminated
        }homing;
        struct{
            std::thread threadAligning; // the thread executing aligning
            std::atomic<bool> threadAligningUp; // if the thread is up
            std::atomic<bool> threadAligningKill; // if the thread is top be terminated
        }aligning;
        struct{
            std::thread threadMoving; // the thread executing movement
            std::atomic<bool> threadMovingUp; // if the thread is up
            std::atomic<bool> threadMovingKill; // if the thread is top be terminated
        }moving;
        struct{
            std::thread threadFocusing; // the thread executing focus
            std::atomic<bool> threadFocusingUp; // if the thread is up
            std::atomic<bool> threadFocusingKill; // if the thread is top be terminated
        }focusing;
        struct{
            std::thread threadAnalysis; // the thread executing analysis
            std::atomic<bool> threadAnalysisUp; // if the thread is up
            std::atomic<bool> threadAnalysisKill; // if the thread is top be terminated
        }analysis;
    }threading; // threading related stuff

    struct{
        uint32_t maxAlignmentSteps = 10; // the max tries to minimize delta Z
        uint32_t imagePoolPeriod = 30; // how often are images taken for focus calculation [ms]
        const uint32_t focusVelocityCoefficientRough = 10; // how much the velocity is divided for rough focus search 
        const uint32_t focusVelocityCoefficientFine = 50; // how much the velocity is divided for fine focus search 
        std::atomic<bool> interuptAlignment; // if to stop alignment
        struct{
            AlignmentPoint p1; // the first point to look for focus, center one
            AlignmentPoint p2; // the second point to look for focus, x axis alignment
            AlignmentPoint p3; // the third point to look for focus, y axis alignment
            struct{
                AlignmentPoint h1;
                AlignmentPoint h2;
                AlignmentPoint h3;
                AlignmentPoint h4;
            }holder1;
        }alignmentPoints;
        struct{
            double range = 1; // the +- range for fine focus search -> Z axis
            double mildRange = 1; // the +- range for fine focus search -> Z axis
            double deltaZLimit = 0.005; // the acceptable delta Z [mm]
            double distancePaxis = 60; // the position of the P axis on the X axis [mm] 
            uint32_t firstStepDivider = 2; // the coefficient used to reduce (divide) the movement size when the deltaZ is in between 30 - 50 [um]
            uint32_t secondStepDivider = 10; // the coefficient used to reduce (divide) the movement size when the deltaZ is in less than 30 [um]
            uint32_t thirdStepDivider = 100; // the coefficient used to reduce (divide) the movement size when the deltaZ is in less than 100 [um]
        }movement;
        struct{
            double fineRange = 0.1; // [mm] the fine range for T/P axis movement
            double roughRange = 1; // [mm] the rough range for T/P axis movement
            double zRange = 1; // [mm] the Z axis range for alignment method
        }movementAlt;
        struct{
            double thresholdFirst = 0.05; // the movement treshold  [mm]
            double thresholdSecond = 0.03; // the movement treshold  [mm]
            double thresholdThird = 0.01; // the movement treshold  [mm]
        }tresholding;
    }alignment;
    
    struct{
        std::atomic<bool> interuptAnalysis; // if to interupt the analysis process
        const uint32_t noOfHolders = 10; // total number of holders
        struct{
            bool aligned = false; // if the holder 1 is aligned
            bool alignedX = false; // if the holder 1 is aligned on axis X
            bool alignedY = false; // if the holder 1 is aligned on axis Y
            double offsetX = 3.735; // [mm] offset of sample centers for x axis of the holder 1
            double offsetY = 3.355; // [mm] offset of sample centers for y axis of the holder 1
            bool takeImages = false; // if to take images when focus of sample center found
            uint32_t samplesX = 10; // number of samples in x axis
            uint32_t samplesY = 10; // number of samples in y axis
            struct{
                AlignmentPoint s_1_1; // point of center of sample {1,1} --> {x,y}
            }points;
        }holder1;
    }analysis;
    
    //the possible states for the TT
    enum class state{
        INITIALIZED = 0, // created object
        CONFIGURED = 1, // configuration loaded
        HOMING = 2, // device is homing; background process
        ALIGNING = 3, // the process if alignment is running; background process
        READY = 4, // the device is homed and aligned; Ready to work
        ANALYSING = 5, // the device is analysing all the samples; background process
        MOVING = 6 // the device is moving to a requested position; background process
    };
    csutils::rwpp<state> currentStatus; // the status variable
    
    struct{
        struct{
            double deltaZX_internal = 0.0; // the current delta Z for axis X
            double deltaZY_internal = 0.0; // the current delta Z for axis Y
        }alignment;
    }parameters;
    
public:
    
    std::unique_ptr<csutils::rop<double>> deltaZX; // the current delta Z for axis X
    std::unique_ptr<csutils::rop<double>> deltaZY;// the current delta Z for axis Y

    
};

}
}



#endif	/* TARGETTOWER_H */

