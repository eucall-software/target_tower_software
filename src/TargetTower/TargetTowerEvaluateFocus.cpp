/*
 *************************************************************************
 * @file TargetTowerEvaluateFocus.cpp
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

    bool TargetTower::evaluateFocus(const csimage::MonoImage& image, double& focus){

        //default output
        focus = 0;
        
        //check input
        if(image.w.get() == 0 || image.h.get() == 0){
            csutils::WRITE_ERROR("Aborting 'evaluateFocus' -> the image is not defined");
            return false;
        }
        
        //local variables
        uint32_t size = 0;
        uint32_t height = image.h.get();
        uint32_t width = image.w.get();
        uint16_t* data = nullptr;

        //get pointer
        image.getData(&data, size);
        if(!data){
            csutils::WRITE_ERROR("Aborting 'evaluateFocus' -> failed to get data pointer");
            return false;
        }

        //result vector
        std::vector< double > data2(size);
        
        //
        // Image processing
        //
        double mean = 0;
        for( size_t iy = 1; iy < height-1; iy++ ){
            for( size_t ix = 1; ix < width-1; ix++ ){

                int i1 = ( iy-1 )*width + ix-1;
                int i2 = i1 + width;
                int i3 = i2 + width;
                
                double gx = (double)1.0 * data[i1] + (double)2.0 * data[i1+1] + (double)1.0 * data[i1+2];
                gx += (double)-1.0 * data[i3] - (double)2.0 * data[i3+1] - (double)1.0 * data[i3+2];
                
                double gy = (double)1.0 * data[i1] + (double)2.0 * data[i2] + (double)1.0 * data[i3];
                gy += (double)-1.0 * data[i1+2] - (double)2.0 * data[i2+2] - (double)1.0 * data[i3+2];
                
                gx /= 1024;
                gy /= 1024;
                
                double gg = gx*gx + gy*gy; // no sqrt
                
                int index = ( iy - 1 ) * ( width - 2 ) + ( ix - 1 );
                
                data2[index] = gg;
                mean += gg;
            }
        }
        
        //calculate------------------------------------------------------------
        int ndata2 = ( width - 2 ) * ( height - 2 );
        mean /= ndata2;

        double rms2 = 0;
        
        for( int ii = 0; ii < ndata2; ii++ ){
            rms2 += ( mean - data2[ii] ) * ( mean - data2[ii] );
        }

        rms2 /= ndata2;

        focus = rms2;
        
        return true;
    }
}
}
