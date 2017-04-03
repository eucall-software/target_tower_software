/*
 *************************************************************************
 * @file TargetTowerAnalyze.cpp
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

    bool TargetTower::analyze(uint32_t holder){
        
        //check state
        if(!currentStatus.compare(state::READY)){
            csutils::WRITE_ERROR("Aborting 'analyze' -> wrong state");
            return false;
        }
        
        //report ---------------------------------------------------------------
        
        csutils::WRITE_INFO("Starting the analysis process");
        
        //set the state
        currentStatus.set(state::ANALYSING);
        
        //abort flag
        analysis.interuptAnalysis.store(false);
        
        //start the thread
        if(!threadAnalysisStart(holder)){
            currentStatus.set(state::READY);
            csutils::WRITE_ERROR("Failed to start the analysis thread");
            return false;
        }
        
        //done
        return true;
    }
    
}
}
