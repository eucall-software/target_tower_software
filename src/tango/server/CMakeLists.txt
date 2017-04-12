##############################################################################
# Set project name
##############################################################################
project (TowerTangoServer)

##############################################################################
# Check TANGO variables
##############################################################################
if(NOT(TANGO_INCLUDE_DIRS OR TANGO_LIBRARY_DIRS))
    message(WARNING "Stopping compilation of the Tango servers. Missing Tango installation")
    return()
endif()

##############################################################################
# Set TANGO include directories
##############################################################################
include_directories(${TANGO_INCLUDE_DIRS})
include_directories ("./")

##############################################################################
# Set TANGO link libraries
##############################################################################
link_directories(${TANGO_LIBRARY_DIRS})

##############################################################################
# find libs
##############################################################################
FIND_LIBRARY(DL_LIB dl)
if(NOT DL_LIB)
    message(FATAL_ERROR "Not found dl library")
endif()

FIND_LIBRARY(CONFIG_LIB ConfigLib)
if(NOT CONFIG_LIB)
    message(FATAL_ERROR "Not found configuration library")
endif()

FIND_LIBRARY(PLUGIN_LIB csplugin)
if(NOT PLUGIN_LIB)
    message(FATAL_ERROR "Not found csplagin library")
endif()

FIND_LIBRARY(CSIMAGE_LIB csimage)                                                                                                                                   
if(NOT CSIMAGE_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found csimage (mandatory - part of csimage)")                                                                                                                    
endif()      

FIND_LIBRARY(PNG_LIB png14)
if(NOT PNG_LIB)
    message(FATAL_ERROR "Not found png14 (mandatory for cscamera package - https://sourceforge.net/projects/libpng/files/libpng14/)")
endif()

FIND_LIBRARY(CSCAMERA_LIB cscamera_genicam)
if(NOT CSCAMERA_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found genICamera")                                                                                                                    
endif() 

FIND_LIBRARY(CSMOTION_LIB continuousStage_Phidget_Stepper)
if(NOT CSMOTION_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found phidget motion library")                                                                                                                    
endif()  

FIND_LIBRARY(PHIDGET_LIB phidget21)
if(NOT PHIDGET_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found phidget21 library")                                                                                                                    
endif()  

##############################################################################
# Set TANGO server target
##############################################################################

set(TANGO_SERVER TargetTowerTangoServer)
set(TANGO_SERVER_SOURCES
   ./ClassFactory.cpp
   ./${TANGO_SERVER}Class.cpp 
   ./${TANGO_SERVER}.cpp
   ./${TANGO_SERVER}StateMachine.cpp
   ./main.cpp
   )
add_executable(${TANGO_SERVER} ${TANGO_SERVER_SOURCES})
target_link_libraries(${TANGO_SERVER}
                      ${TANGO_LIBRARIES}
                      ${CSMOTION_LIB} 
                      ${PHIDGET_LIB}
                      ${CSCAMERA_LIB}
                      ${CSIMAGE_LIB}
                      ${PNG_LIB}
                        targetTower
                        pthread
)

##############################################################################
# install script
##############################################################################
install(TARGETS TargetTowerTangoServer
	EXPORT ${PROJECT_NAME}Targets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include)