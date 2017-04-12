##############################################################################
# Set project name
##############################################################################
project(TargetTowerProject)
set(PROJECT_DESCRIPTION "Control system for RP3 Target Tower")
set(PROJECT_VENDOR "ELI Beamlines")

##############################################################################
# Minimum CMake required
##############################################################################
cmake_minimum_required(VERSION 3.0)

##############################################################################
#  Set local CMake modules path
##############################################################################
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

##############################################################################
# Enable and include testing
##############################################################################
enable_testing ()
include (CTest)

##############################################################################
#  Include GIT version macro
##############################################################################
include(GetGitRevisionDescription)

##############################################################################
# Get and parse version from GIT
##############################################################################
git_describe(VERSION)

get_git_head_revision(GIT_REFSPEC VERSION_HEAD)

string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(SUBSTRING "${VERSION_HEAD}" 0 7 VERSION_SHORT_HEAD )

set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SHORT_HEAD}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(VERSION_LONG "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SHORT_HEAD}")

##############################################################################
# Set version
##############################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
                ${CMAKE_CURRENT_BINARY_DIR}/version.h)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.h")

##############################################################################
# C/C++ flags
##############################################################################
set(CMAKE_REQUIRED_INCLUDES "/usr/local/include/")
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wall -Wextra -Wno-unknown-pragmas -Wno-reorder -Wno-unused-function -Wno-narrowing -Dlinux")

option(USE_STD_THREADS "Force usage of C++ standard library thread support." ON)
find_package (Threads)

##############################################################################
# Include macros
##############################################################################
include (CheckFunctionExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckSymbolExists)
include (CheckLibraryExists)

##############################################################################
# Check for C++ headers
##############################################################################
check_include_file_cxx (iostream HAVE_IOSTREAM_HPP)
check_include_file_cxx (iomanip HAVE_IOMANIP_HPP)
check_include_file_cxx (cstddef HAVE_CSTDFEF_HPP)
check_include_file_cxx (thread HAVE_THREAD_HPP)
check_include_file_cxx (atomic HAVE_ATOMIC_HPP)
check_include_file_cxx (map HAVE_MAP_HPP)
check_include_file_cxx (vector HAVE_VECTOR_HPP)

##############################################################################
# Check for C headers
##############################################################################
check_include_files (pthread.h HAVE_PTHREAD_H)

##############################################################################
# Check for cs headers
##############################################################################
find_file(HAVE_CSBASE_H csbase.h)
if(HAVE_CSBASE_H)
    message(STATUS "Found csbase.h")
endif()
find_file(HAVE_CSPLUGIN_H csplugin.h)
if(HAVE_CSPLUGIN_H)
    message(STATUS "Found csplugin.h")
endif()
find_file(HAVE_CSUTILS_H csutils.h)
if(HAVE_CSUTILS_H)
    message(STATUS "Found csutils.h")
endif()
find_file(HAVE_CSMONOIMAGE_H csmonoimage.h)
if(HAVE_CSMONOIMAGE_H)
    message(STATUS "Found csmonoimage.h")
endif()
find_file(HAVE_CSMONOIMAGE8_H csmonoimage8.h)
if(HAVE_CSMONOIMAGE8_H)
    message(STATUS "Found csmonoimage8.h")
endif()
find_file(HAVE_CSCAMERA_H Camera.h)
if(HAVE_CSCAMERA_H)
    message(STATUS "Found Camera.h")
endif()
find_file(HAVE_CSDEVICECONFIG_H DeviceConfiguration.h)
if(HAVE_CSDEVICECONFIG_H)
    message(STATUS "Found DeviceConfiguration.h")
endif()
find_file(HAVE_CSCOMPONENTCONFIG_H ComponentConfiguration.h)
if(HAVE_CSCOMPONENTCONFIG_H)
    message(STATUS "Found ComponentConfiguration.h")
endif()
find_file(HAVE_CSMOTION_H Motion.h)
if(HAVE_CSMOTION_H)
    message(STATUS "Found Motion.h")
endif()
find_file(HAVE_CSCONTINUOUSSTAGE_H ContinuousStage.h)
if(HAVE_CSCONTINUOUSSTAGE_H)
    message(STATUS "Found ContinuousStage.h")
endif()

##############################################################################
# Output locations
##############################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##############################################################################
# CPack Settings
##############################################################################
include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_SET_DESTDIR true)
##############################################################################
# Use -DCMAKE_INSTALL_PREFIX for package prefix
##############################################################################
#set(CPACK_INSTALL_PREFIX /usr/local)

##############################################################################
# Enable package
##############################################################################
include (CPack)

##############################################################################
# Specify include directories
##############################################################################
include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_BINARY_DIR}")

##############################################################################                                                                                
#  Build TANGO interface                                                                                                                                      
##############################################################################                                                                                
SET(ENABLE_TANGO_SERVER TRUE)                                                                                                                                
SET(ENABLE_TANGO_CLIENT FALSE)                                                                                                                                
IF (ENABLE_TANGO_SERVER OR ENABLE_TANGO_CLIENT) 
##############################################################################
#  Normally, you shoul not modify the code between dashed lines (below).
#  Unless you know what you are doing, obviously.
#  Do not forget to properly tell PkgConfig where it should look
#  for the tango.pc file. For instance:
#
#  export "PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/tango/lib/pkgconfig" 
#  export "PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig"
#
#  The previous command must be executed in the command line BEFORE
#  executing CMAKE.
#
##############################################################################

##############################################################################
#  Check for PkgConfig
##############################################################################
find_package(PkgConfig)

##############################################################################
#  Check for TANGO
##############################################################################
PKG_CHECK_MODULES(TANGO tango>=9.2.0 REQUIRED)
add_definitions("-std=c++11" ${tango_LDFLAGS})

##############################################################################
# Set TANGO include directories
##############################################################################
include_directories(${TANGO_INCLUDE_DIRS})
include_directories(./)

##############################################################################
#  Build TANGO server
##############################################################################
IF (ENABLE_TANGO_SERVER)
add_subdirectory (src/tango/server)
ENDIF ()


ENDIF ()
##############################################################################
# Find doxygen
##############################################################################
find_package(Doxygen)
IF (DOXYGEN_FOUND)
   set(DOC_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
   set(DOC_AUX_DIR ${DOC_ROOT_DIR}/auxfiles)
   configure_file(${DOC_AUX_DIR}/Doxyfile.in ${DOC_AUX_DIR}/Doxyfile @ONLY)
   configure_file(${DOC_AUX_DIR}/latex/header.tex.in ${DOC_AUX_DIR}/latex/header.tex @ONLY)
   execute_process(COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/doc")
   execute_process(COMMAND cp -R ${DOC_AUX_DIR} ${CMAKE_CURRENT_BINARY_DIR}/doc/)
   find_program(DOXYFILE_MAKE make)
   add_custom_target(dochtml ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/auxfiles/Doxyfile  > /dev/null 2>&1
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
      
   IF (MAKE_LATEX_DOC)
   add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf
      COMMAND pdflatex refman.tex  > /dev/null 2>&1
      COMMAND pdflatex refman.tex  > /dev/null 2>&1
      COMMAND pdflatex refman.tex  > /dev/null 2>&1
      COMMAND cp refman.pdf ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf
      COMMENT "Generating LaTeX documentation"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/
      DEPENDS dochtml
      #VERBATIM
      )   
   add_custom_target(doclatex ALL
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf
      )
   add_custom_target(
      doc
      DEPENDS dochtml
      DEPENDS doclatex
      )
   ELSE ()
   add_custom_target(
      doc
      DEPENDS dochtml
      )   
   ENDIF ()
ENDIF (DOXYGEN_FOUND)

##############################################################################
# Define configure file
##############################################################################
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

##############################################################################
# Add TargetTower class
##############################################################################
add_subdirectory (./src)

##############################################################################
# Installation setup
##############################################################################
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/html DESTINATION doc/${PROJECT_NAME})
install(DIRECTORY etc/ DESTINATION etc/${PROJECT_NAME} OPTIONAL)
install(DIRECTORY configs/ DESTINATION share/${PROJECT_NAME}/configs OPTIONAL)

##############################################################################
# Installation setup - define cmake scripts to be running after installation
##############################################################################
install(CODE "MESSAGE(\"${PACKAGE_NAME} has been installed\")")


