/*
 *************************************************************************
 * @file TargetTowerThreadAnalysisFce.cpp
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

    void TargetTower::threadAnalysisFce(uint32_t holder){
    
        //check state
        if(!currentStatus.compare(state::ANALYSING)){
            csutils::WRITE_ERROR("Aborting the analysis thread -> wrong state");
            //thread status
            threading.analysis.threadAnalysisUp.store(false);
            return;
        }
        
        //----------------------------------------------------------------------

        //specific holder??
        if(holder != 0){
            //analyze specific
            if(!analyzeHolder(holder)){
                csutils::WRITE_ERROR("Failure during the sample holder 1 analysis");
            }else{
                //report
                csutils::WRITE_INFO("Finished the analysis of all available sample holders");
            }
        }else{

            //analyze holder 1
            if(analysis.holder1.aligned){

                //info
                csutils::WRITE_INFO("Starting the analysis of the sample holder 1");

                //analyze
                if(!analyzeHolder(1)){
                    csutils::WRITE_ERROR("Failure during the sample holder 1 analysis");
                }

            }else{
                csutils::WRITE_WARNING("Skipping the sample holder 1. The holder is not aligned. "
                "Run holder alignment first");
            }

            //report
            csutils::WRITE_INFO("Finished the analysis of all available sample holders");
        }
        
        
        
        //set state-------------------------------------------------------------
        currentStatus.set(state::READY);
        
        //thread status
        threading.analysis.threadAnalysisUp.store(false);
        
        return;
    }
    
}
}
