/*
 *************************************************************************
 * @file TargetTowerThreadAlignmentFce.cpp
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

    void TargetTower::threadAlignmentFce(bool alignX, bool alignY,  uint32_t holder){
    
        //check state
        if(!currentStatus.compare(state::ALIGNING)){
            csutils::WRITE_ERROR("Aborting the alignment process -> "
                "wrong state");
            //thread status
            threading.aligning.threadAligningUp.store(false);
            return;
        }
        
        //check input
        if(!alignX && !alignY){
            csutils::WRITE_ERROR("Aborting the alignment process -> "
                " the user did't specify what axis to align");
            //set state
            currentStatus.set(state::READY);
            //thread status
            threading.aligning.threadAligningUp.store(false);
            return;
        }
        
        //Run the x alignment
        if(alignX && !alignment.interuptAlignment.load()){
            //running frame
            if(holder == 0){
                if(!alignFrameX()){
                    csutils::WRITE_ERROR("Failed to align the frame of the Target Tower (axis X)");
                    //set state
                    currentStatus.set(state::READY);
                    //thread status
                    threading.aligning.threadAligningUp.store(false);
                    return;
                }
            }else{
                if(!alignHolderX()){    
                    csutils::WRITE_ERROR("Failed to align the frame of the Target Tower (axis X)");
                    //set state
                    currentStatus.set(state::READY);
                    //thread status
                    threading.aligning.threadAligningUp.store(false);
                    return;
                }
                
                //alignment done?
                if(alignment.interuptAlignment.load()){
                    analysis.holder1.alignedX = false;
                }else{
                    analysis.holder1.alignedX = true;
                }
            }
        }

        //Run the y alignment
        if(alignY && !alignment.interuptAlignment.load()){
            if(holder == 0){
                if(!alignFrameY()){
                    csutils::WRITE_ERROR("Failed to align the frame of the Target Tower (axis Y)");
                    //set state
                    currentStatus.set(state::READY);
                    //thread status
                    threading.aligning.threadAligningUp.store(false);
                    return;
                }
            }else{
                if(!alignHolderY()){
                    csutils::WRITE_ERROR("Failed to align the frame of the Target Tower (axis Y)");
                    //set state
                    currentStatus.set(state::READY);
                    //thread status
                    threading.aligning.threadAligningUp.store(false);
                    return;
                }
                //alignment done?
                if(alignment.interuptAlignment.load()){
                    analysis.holder1.alignedY = false;
                }else{
                    analysis.holder1.alignedY = true;
                }
            }
        }
        
        //evaluate
        switch(holder){
            case 0:
                //frame alignment
                break;
            case 1: // holder 1
                analysis.holder1.aligned = (analysis.holder1.alignedX && analysis.holder1.alignedY);
            break;
                
        }

        //set state-------------------------------------------------------------
        currentStatus.set(state::READY);
        
        //thread status
        threading.aligning.threadAligningUp.store(false);
        
        return;
    }
    
}
}
