/*
 *************************************************************************
 * @file TargetTowerThreadAnalysisStart.cpp
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

    bool TargetTower::threadAnalysisStart(uint32_t holder){
    
        //check if already up
        if(threading.analysis.threadAnalysisUp.load()){
            csutils::WRITE_WARNING("The focusing thread is already up!!");
            return true;
        }

        //set the kill to true to make sure it is dead
        threading.analysis.threadAnalysisKill.store(true);

        //reset the thread object
        if(threading.analysis.threadAnalysis.joinable()){
            threading.analysis.threadAnalysis.join();
        }

        //set the kill to false
        threading.analysis.threadAnalysisKill.store(false);

        //start thread
        threading.analysis.threadAnalysis = std::thread(&TargetTower::threadAnalysisFce ,this, holder);

        //store the status
        threading.analysis.threadAnalysisUp.store(true);
    
        return true;
    }
    
}
}
