##############################################################################
# Set project name
##############################################################################
project(TargetTower)

##############################################################################
# To see what makes do
##############################################################################
#set(CMAKE_VERBOSE_MAKEFILE ON)

##############################################################################
# Set include directories
##############################################################################

##############################################################################
# Set link libraries
##############################################################################
link_directories("${CMAKE_BINARY_DIR}/lib")
link_directories("${PROJECT_BINARY_DIR}/lib")

##############################################################################
# find libraries
##############################################################################
FIND_LIBRARY(DL_LIB dl)
if(NOT DL_LIB)
    message(FATAL_ERROR "Not found dl library")
endif()

FIND_LIBRARY(CONFIG_LIB ConfigLib)
if(NOT CONFIG_LIB)
    message(FATAL_ERROR "Not found configuration library (mandatory - part of csbase)")
endif()

FIND_LIBRARY(PLUGIN_LIB csplugin)
if(NOT PLUGIN_LIB)
    message(FATAL_ERROR "Not found csplugin library (mandatory - part of csbase)")
endif()

FIND_LIBRARY(CSIMAGE_LIB csimage)                                                                                                                                   
if(NOT CSIMAGE_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found csimage (mandatory - part of csimage)")                                                                                                                    
endif()      

FIND_LIBRARY(PNG_LIB png14)
if(NOT PNG_LIB)
    message(FATAL_ERROR "Not found png15 (mandatory for cscamera package - https://sourceforge.net/projects/libpng/files/libpng14/)")
endif()

FIND_LIBRARY(CSCAMERA_LIB cscamera_genicam)
if(NOT CSCAMERA_LIB)                                                                                                                                               
    message(FATAL_ERROR "Not found genICamera")                                                                                                                    
endif()      

# Target Tower class ------------------------------------------------------------------------------
add_subdirectory(./TargetTower)

# Main application executable --------------------------------------------------
set (MAIN_SOURCES
		./main.cpp)
add_executable (main
	${MAIN_SOURCES})
target_link_libraries (main
                        targetTower
                        ${DL_LIB}
                        ${PLUGIN_LIB}
                        ${PNG_LIB}
                        ${CSIMAGE_LIB}
                        -pthread
)