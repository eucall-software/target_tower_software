/*
 *************************************************************************
 * @file TargetTowerThreadAnalysisStop.cpp
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

    bool TargetTower::threadAnalysisStop(){
        
        //set the kill flag
        threading.analysis.threadAnalysisKill.store(true);

        //reset the thread object
        if(threading.analysis.threadAnalysis.joinable()){
            threading.analysis.threadAnalysis.join();
        }

        //store the status
        threading.analysis.threadAnalysisUp.store(false);

        return true;

    }
}
}
