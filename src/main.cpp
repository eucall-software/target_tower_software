/* 
 * File:   main.cpp
 * Author: ondra
 *
 * Created on January 2, 2017, 1:35 PM
 */

#include <cstdlib>

#include "TargetTower.h"

/*
 * 
 */
int main() {
    std::string configuration = "/home/ondra/Documents/local_git/targettower/SW/"
            "cstargettower/Implementation/configs/targetTower/configuration.xml";
    csrp3::targetTower::TargetTower tt;

    //--------------------------------------------------------------------------
    if(!tt.loadConfigurationFromFile(configuration)){
        return 1;
    }
    
//    tt.clearConfiguration();
//    
//    if(!tt.loadConfigurationFromFile(configuration)){
//        return 1;
//    }
    
    //--------------------------------------------------------------------------    
    //homing
    if(!tt.home()){
        return 1;
    }
    while(tt.isHoming()){
        usleep(100000);
    }
    
    //--------------------------------------------------------------------------
    //alignment
//    if(!tt.align()){
//        return 1;
//    }
    while(tt.isAligning()){
        usleep(100000);
    }
    
    return 0;
}



