##############################################################################
# Set project name
##############################################################################
project(TargetTower_class)

##############################################################################
# C/C++ flags
##############################################################################
set (CMAKE_CXX_FLAGS "-g -pthread -std=c++11 -Wall -Wextra -Wno-unknown-pragmas -Wno-reorder -Wno-unused-function")

##############################################################################
# Specify include directories
##############################################################################
include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("./")

##############################################################################
# find libraries
##############################################################################

FIND_LIBRARY(CSMOTION_LIB continuousStage_Phidget_Stepper)
if(NOT CSMOTION_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found phidget motion library")                                                                                                                    
endif()  

FIND_LIBRARY(PHIDGET_LIB phidget21)
if(NOT PHIDGET_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found phidget21 library")                                                                                                                    
endif()  


##############################################################################
# Add executable
##############################################################################
set (TARGETTOWER_SOURCES
#public
    ./TargetTower.cpp
    ./TargetTowerLoadConfiguration.cpp
    ./TargetTowerClearConfiguration.cpp
    ./TargetTowerHome.cpp
    ./TargetTowerAlign.cpp
    ./TargetTowerAnalyze.cpp
    ./TargetTowerDisconnect.cpp

#public
    ./TargetTowerAbort.cpp
    ./TargetTowerMoveAbsolute.cpp
    ./TargetTowerMoveRelative.cpp
    ./TargetTowerRun.cpp
    ./TargetTowerFocus.cpp

#public
    ./TargetTowerGetPosition.cpp
    ./TargetTowerRetrieveLatestImage.cpp

# private
    ./TargetTowerSetupCamera.cpp
    ./TargetTowerSetupAxis.cpp
    ./TargetTowerAlignFrameX.cpp
    #./TargetTowerAlignFrameX2.cpp
    ./TargetTowerAlignFrameY.cpp
    ./TargetTowerAlignHolderY.cpp
    ./TargetTowerAlignHolderX.cpp
    ./TargetTowerFocusAtPoint.cpp
    ./TargetTowerEvaluateFocus.cpp
    ./TargetTowerRoughFocus.cpp
    ./TargetTowerRoughFocusP.cpp
    ./TargetTowerFineFocus.cpp
    ./TargetTowerFineFocusP.cpp
    ./TargetTowerMildFocus.cpp
    ./TargetTowerMoveToPoint.cpp
    ./TargetTowerAnalyzeHolder.cpp

#threads
    ./threading/TargetTowerThreadHomingFce.cpp
    ./threading/TargetTowerThreadHomingStart.cpp
    ./threading/TargetTowerThreadHomingStop.cpp
    ./threading/TargetTowerThreadAlignmentFce.cpp
    ./threading/TargetTowerThreadAlignmentStart.cpp
    ./threading/TargetTowerThreadAlignmentStop.cpp
    ./threading/TargetTowerThreadMovingFce.cpp
    ./threading/TargetTowerThreadMovingStart.cpp
    ./threading/TargetTowerThreadMovingStop.cpp
    ./threading/TargetTowerThreadFocusingFce.cpp
    ./threading/TargetTowerThreadFocusingStart.cpp
    ./threading/TargetTowerThreadFocusingStop.cpp
    ./threading/TargetTowerThreadAnalysisFce.cpp
    ./threading/TargetTowerThreadAnalysisStart.cpp
    ./threading/TargetTowerThreadAnalysisStop.cpp

)

add_library (targetTower
            SHARED
            ${TARGETTOWER_SOURCES})

target_link_libraries (targetTower
                        ${CSMOTION_LIB} 
                        ${PHIDGET_LIB}
                        ${CSCAMERA_LIB}
                        ${PLUGIN_LIB}
                        ${DL_LIB}
                        ${PNG_LIB}
                        pthread
)

set_target_properties (targetTower PROPERTIES
		VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
		SOVERSION ${VERSION_MAJOR})

##############################################################################
# install script
##############################################################################
install(TARGETS targetTower
	EXPORT ${PROJECT_NAME}Targets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include)