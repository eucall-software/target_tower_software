/*----- PROTECTED REGION ID(TargetTowerTangoServer.cpp) ENABLED START -----*/
//static const char *RcsId = "$Id:  $";
/*
 *************************************************************************
 * @file TargetTowerTangoServer.cpp
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


#include <TargetTowerTangoServer.h>
#include <TargetTowerTangoServerClass.h>

/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer.cpp

/**
 *  TargetTowerTangoServer class description:
 *    
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name        |  Method name
//================================================================
//  State               |  Inherited (no method)
//  Status              |  Inherited (no method)
//  loadConfiguration   |  load_configuration
//  home                |  home
//  abort               |  abort
//  align               |  align
//  analyse             |  analyse
//  moveAbs             |  move_abs
//  moveRel             |  move_rel
//  run                 |  run
//  center              |  center
//  clearConfiguration  |  clear_configuration
//  disconnect          |  disconnect
//  saveImage           |  save_image
//  focus               |  focus
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  positionX   |  Tango::DevDouble	Scalar
//  positionY   |  Tango::DevDouble	Scalar
//  positionZ   |  Tango::DevDouble	Scalar
//  positionP   |  Tango::DevDouble	Scalar
//  positionT   |  Tango::DevDouble	Scalar
//  deltaZX     |  Tango::DevDouble	Scalar
//  deltaZY     |  Tango::DevDouble	Scalar
//  cameraFeed  |  Tango::DevUShort	Image  ( max = 5000 x 5000)
//================================================================

namespace TargetTowerTangoServer_ns
{
/*----- PROTECTED REGION ID(TargetTowerTangoServer::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::TargetTowerTangoServer()
 *	Description : Constructors for a Tango device
 *                implementing the classTargetTowerTangoServer
 */
//--------------------------------------------------------
TargetTowerTangoServer::TargetTowerTangoServer(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::constructor_1
}
//--------------------------------------------------------
TargetTowerTangoServer::TargetTowerTangoServer(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::constructor_2
}
//--------------------------------------------------------
TargetTowerTangoServer::TargetTowerTangoServer(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void TargetTowerTangoServer::delete_device()
{
	DEBUG_STREAM << "TargetTowerTangoServer::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::delete_device) ENABLED START -----*/
	
        //abort if moving
        if(get_state() == Tango::CLOSE || 
                get_state() == Tango::MOVING || 
                get_state() == Tango::RUNNING || 
                get_state() == Tango::OPEN){
            //abort movement
            tt->abort();
        }
        
        //kill threads
        homingMonitorStop();
        aligningMonitorStop();
        moveMonitorStop();
        
        //disconnect if standby
        if(get_state() == Tango::STANDBY){
            //disconnect
            tt->disconnect();
        }if(get_state() == Tango::DISABLE){
            tt->clearConfiguration();
        }
                
	//force release
        tt.reset();
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::delete_device
	delete[] attr_positionX_read;
	delete[] attr_positionY_read;
	delete[] attr_positionZ_read;
	delete[] attr_positionP_read;
	delete[] attr_positionT_read;
	delete[] attr_deltaZX_read;
	delete[] attr_deltaZY_read;
	delete[] attr_cameraFeed_read;
}

//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void TargetTowerTangoServer::init_device()
{
	DEBUG_STREAM << "TargetTowerTangoServer::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::init_device_before
	
	//	No device property to be read from database
	
	attr_positionX_read = new Tango::DevDouble[1];
	attr_positionY_read = new Tango::DevDouble[1];
	attr_positionZ_read = new Tango::DevDouble[1];
	attr_positionP_read = new Tango::DevDouble[1];
	attr_positionT_read = new Tango::DevDouble[1];
	attr_deltaZX_read = new Tango::DevDouble[1];
	attr_deltaZY_read = new Tango::DevDouble[1];
	attr_cameraFeed_read = new Tango::DevUShort[5000*5000];
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::init_device) ENABLED START -----*/
	
        //threading initital values
        threading.homingMonitorKill.store(false);
        threading.homingMonitorUp.store(false);
        threading.aligningMonitorKill.store(false);
        threading.aligningMonitorUp.store(false);
        threading.moveMonitorKill.store(false);
        threading.moveMonitorUp.store(false);
        
        //tt object
        tt.reset(new csrp3::targetTower::TargetTower);
        
	//move to init
        set_state(Tango::INIT);
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::init_device
}


//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void TargetTowerTangoServer::always_executed_hook()
{
	DEBUG_STREAM << "TargetTowerTangoServer::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute positionX related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_positionX(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_positionX(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_positionX) ENABLED START -----*/
	
        //check the state
        if(get_state() == Tango::STANDBY || 
                get_state() == Tango::RUNNING ||
                get_state() == Tango::MOVING ||
                get_state() == Tango::OPEN){
            
            //get the value from tt object
            double position = 0.0;
            if(tt->getPositionX(position)){
                *attr_positionX_read = position;
            }else{
                *attr_positionX_read = position;
            }
        }else{
            //set default 
            *attr_positionX_read = 0.0;
        }
        
        //Set the attribute value
	attr.set_value(attr_positionX_read);
        
        return;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_positionX
}
//--------------------------------------------------------
/**
 *	Read attribute positionY related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_positionY(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_positionY(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_positionY) ENABLED START -----*/
	//check the state
        if(get_state() == Tango::STANDBY || 
                get_state() == Tango::RUNNING ||
                get_state() == Tango::MOVING ||
                get_state() == Tango::OPEN){
            
            //get the value from tt object
            double position = 0.0;
            if(tt->getPositionY(position)){
                *attr_positionY_read = position;
            }else{
                *attr_positionY_read = position;
            }
        }else{
            //set default 
            *attr_positionY_read = 0.0;
        }
        
        //Set the attribute value
	attr.set_value(attr_positionY_read);
        
        return;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_positionY
}
//--------------------------------------------------------
/**
 *	Read attribute positionZ related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_positionZ(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_positionZ(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_positionZ) ENABLED START -----*/
	//	Set the attribute value
	//check the state
        if(get_state() == Tango::STANDBY || 
                get_state() == Tango::RUNNING ||
                get_state() == Tango::MOVING ||
                get_state() == Tango::OPEN){
            
            //get the value from tt object
            double position = 0.0;
            if(tt->getPositionZ(position)){
                *attr_positionZ_read = position;
            }else{
                *attr_positionZ_read = position;
            }
        }else{
            //set default 
            *attr_positionZ_read = 0.0;
        }
        
        //Set the attribute value
	attr.set_value(attr_positionZ_read);
        
        return;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_positionZ
}
//--------------------------------------------------------
/**
 *	Read attribute positionP related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_positionP(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_positionP(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_positionP) ENABLED START -----*/
	//check the state
        if(get_state() == Tango::STANDBY || 
                get_state() == Tango::RUNNING ||
                get_state() == Tango::MOVING ||
                get_state() == Tango::OPEN){
            
            //get the value from tt object
            double position = 0.0;
            if(tt->getPositionP(position)){
                *attr_positionP_read = position;
            }else{
                *attr_positionP_read = position;
            }
        }else{
            //set default 
            *attr_positionP_read = 0.0;
        }
        
        //Set the attribute value
	attr.set_value(attr_positionP_read);
        
        return;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_positionP
}
//--------------------------------------------------------
/**
 *	Read attribute positionT related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_positionT(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_positionT(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_positionT) ENABLED START -----*/
	//check the state
        if(get_state() == Tango::STANDBY || 
                get_state() == Tango::RUNNING ||
                get_state() == Tango::MOVING ||
                get_state() == Tango::OPEN){
            
            //get the value from tt object
            double position = 0.0;
            if(tt->getPositionT(position)){
                *attr_positionT_read = position;
            }else{
                *attr_positionT_read = position;
            }
        }else{
            //set default 
            *attr_positionT_read = 0.0;
        }
        
        //Set the attribute value
	attr.set_value(attr_positionT_read);
        
        return;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_positionT
}
//--------------------------------------------------------
/**
 *	Read attribute deltaZX related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_deltaZX(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_deltaZX(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_deltaZX) ENABLED START -----*/
	
        //get the value
        *attr_deltaZX_read = tt->deltaZX->get();
        
        // Set the attribute value
	attr.set_value(attr_deltaZX_read);
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_deltaZX
}
//--------------------------------------------------------
/**
 *	Read attribute deltaZY related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_deltaZY(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_deltaZY(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_deltaZY) ENABLED START -----*/
	//get the value
        *attr_deltaZY_read = tt->deltaZY->get();
        
        //	Set the attribute value
	attr.set_value(attr_deltaZY_read);
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_deltaZY
}
//--------------------------------------------------------
/**
 *	Read attribute cameraFeed related method
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Image max = 5000 x 5000
 */
//--------------------------------------------------------
void TargetTowerTangoServer::read_cameraFeed(Tango::Attribute &attr)
{
	DEBUG_STREAM << "TargetTowerTangoServer::read_cameraFeed(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::read_cameraFeed) ENABLED START -----*/
	
        if(get_state() == Tango::INIT){
            
            //get the "NO image ready" image
            attr.set_value(attr_cameraFeed_read, 1, 1);
            return;
        }else{
            // get latest image
            if(!tt->retrieveLatestImage(&imageToReturn) || !imageToReturn){
                csutils::WRITE_ERROR("Image retrieval failure (Tango level)");
                attr.set_value(attr_cameraFeed_read, 1, 1);
                return;
            }
        
            // get image properties
            uint32_t w = 1,h = 1;
            uint32_t size = 0;

            w = imageToReturn->w.get();
            h = imageToReturn->h.get();

            // extract data
            uint16_t* dataPointer = NULL;// = static_cast<uint8_t*>(attr_cameraFeed_read);
            imageToReturn->getData(&dataPointer,size);

            // send data
            attr.set_value(dataPointer, w, h);

            return;
        }
        
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::read_cameraFeed
}

//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void TargetTowerTangoServer::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::add_dynamic_attributes) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic attributes if any
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command loadConfiguration related method
 *	Description: 
 *
 *	@param argin 
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::load_configuration(Tango::DevString argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::loadConfiguration()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::load_configuration) ENABLED START -----*/

        //check state
        if(get_state() != Tango::INIT){
            csutils::WRITE_ERROR("Aborting 'load_configuration' -> wrong state (Tango)");
            return false;
        }

        //check input
        if(!argin){
            csutils::WRITE_ERROR("Aborting 'load_configuration' -> input string invalid (Tango)");
            return false;
        }
        
        //get input
        std::string file = std::string(argin);
        
        //pas input
        if(!tt->loadConfigurationFromFile(file)){
            csutils::WRITE_ERROR("The 'load_configuration' failed -> the Target Tower "
            "failed to load the configuration file (Tango)");
            return false;
        }
        
        //move to init
        set_state(Tango::DISABLE);
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::load_configuration
	return argout;
}
//--------------------------------------------------------
/**
 *	Command home related method
 *	Description: 
 *
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::home()
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::home()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::home) ENABLED START -----*/
	
        //check state
        if(get_state() != Tango::DISABLE){
            csutils::WRITE_ERROR("Aborting 'home' -> wrong state (Tango)");
            return false;
        }
        
        //start homing
        if(!tt->home()){
            csutils::WRITE_ERROR("Failed the 'home' -> the Target Tower "
            "failed run the homing (Tango)");
            return false;
        }
        
        //move to CLOSE
        set_state(Tango::CLOSE);
        
        if(!homingMonitorStart()){
            tt->abort(); // abort homing
            set_state(Tango::DISABLE);
            return false;
        }
        
        //set output
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::home
	return argout;
}
//--------------------------------------------------------
/**
 *	Command abort related method
 *	Description: 
 *
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::abort()
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::abort()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::abort) ENABLED START -----*/
	
        //check state
        if(!(get_state() == Tango::CLOSE || 
                get_state() == Tango::MOVING || 
                get_state() == Tango::RUNNING || 
                get_state() == Tango::OPEN)){
            csutils::WRITE_ERROR("Aborting 'abort' -> wrong state (Tango)");
            return false;
        }
        
        //call abort
        if(!tt->abort()){
            csutils::WRITE_ERROR("The Target Tower failed to call abort");
            return false;
        }
        
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::abort
	return argout;
}
//--------------------------------------------------------
/**
 *	Command align related method
 *	Description: 
 *
 *	@param argin align x {0,1}, align y {0,1}
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::align(const Tango::DevVarULongArray *argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::align()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::align) ENABLED START -----*/
	
        //check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'align' -> wrong state (Tango)");
            return false;
        }
        
        //check length
        if((*argin).length() == 0){
            csutils::WRITE_ERROR("Aborting 'align' -> not enough input parameters (Tango)");
            return false;
        }
        
        //length ? 2
        if((*argin).length() != 3){
            csutils::WRITE_ERROR("Aborting 'align' -> the number parameters is not good (Tango)");
            return false;
        }
        
        //locals
        bool alignX = false;
        bool alignY = false;
        uint32_t holder = 0;
        if((*argin)[0] == 1){
            alignX = true;
        }
        if((*argin)[1] == 1){
            alignY = true;
        }
        holder = (*argin)[2];
        
        //start aligning
        if(!tt->align(alignX,alignY, holder)){
            csutils::WRITE_ERROR("Failed the 'align' -> the Target Tower "
            "failed run the aligning (Tango)");
            return false;
        }
        
        //move to RUNNING
        set_state(Tango::RUNNING);
        
        if(!aligningMonitorStart()){
            tt->abort(); // abort homing
            set_state(Tango::STANDBY);
            return false;
        }
        
        //set return
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::align
	return argout;
}
//--------------------------------------------------------
/**
 *	Command analyse related method
 *	Description: 
 *
 *	@param argin holder no
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::analyse(Tango::DevULong argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::analyse()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::analyse) ENABLED START -----*/
	
        //check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'analyse' -> wrong state (Tango)");
            return false;
        }
        
        //call the function
        if(!tt->analyze(argin)){
            csutils::WRITE_ERROR("Failed to call the analysis function of the Target Tower (Tango)");
            return false;
        }
        
        //set state
        set_state(Tango::OPEN);
        
        //start the thread monitor
        if(!analyzeMonitorStart()){
            
            //abort
            tt->abort();
            
            //set state
            set_state(Tango::STANDBY);
            
            return false;
        }
        
        //return
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::analyse
	return argout;
}
//--------------------------------------------------------
/**
 *	Command moveAbs related method
 *	Description: x,y,z,t,p
 *
 *	@param argin axis, distance
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::move_abs(const Tango::DevVarDoubleArray *argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::moveAbs()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::move_abs) ENABLED START -----*/
	
	//check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'move_abs' -> wrong state (Tango)");
            return false;
        }
        
        //check pointer
        if(!argin){
            csutils::WRITE_ERROR("Aborting 'move_abs' -> wrong input pointer (Tango)");
            return false;
        }
        
        //check size
        if((*argin).length() != 2){
            csutils::WRITE_ERROR("Aborting 'move_abs' -> wrong input size (Tango)");
            return false;
        }
        
        //locals
        uint32_t axis = 0;
        double position = 0.0;
        csrp3::targetTower::TargetTower::axis ax;
        
        //check values
        axis = (*argin)[0];
        position = (*argin)[1];
        
        if(axis > 4){
            csutils::WRITE_ERROR("Aborting 'move_abs' -> wrong axis identifier (Tango)");
            return false;
        }
        
        switch(axis){
            case 0:
                ax = csrp3::targetTower::TargetTower::axis::X;
                break;
            case 1:
                ax = csrp3::targetTower::TargetTower::axis::Y;
                break;
            case 2:
                ax = csrp3::targetTower::TargetTower::axis::Z;
                break;
            case 3:
                ax = csrp3::targetTower::TargetTower::axis::T;
                break;
            case 4:
                ax = csrp3::targetTower::TargetTower::axis::P;
                break;
            default:
                csutils::WRITE_ERROR("Aborting 'move_abs' -> failed to identify the axis (Tango)");
                return false;
        }
        
        //call the movement cmd
        if(!tt->moveAbsolute(ax, position)){
            csutils::WRITE_ERROR("Aborting 'move_abs' -> the Target Tower failed "
            "the move command (Tango)");
            return false;
        }
        
        //move to moving
        set_state(Tango::MOVING);
        
        //start the monitor
        if(!moveMonitorStart()){
            tt->abort();
            csutils::WRITE_ERROR("Aborting 'move_abs' -> failed to start the movement monitor thread (Tango)");
            return false;
        }
        
        //finish
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::move_abs
	return argout;
}
//--------------------------------------------------------
/**
 *	Command moveRel related method
 *	Description: x,y,z,t,p
 *
 *	@param argin axis, distance
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::move_rel(const Tango::DevVarDoubleArray *argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::moveRel()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::move_rel) ENABLED START -----*/
	
	//check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'move_rel' -> wrong state (Tango)");
            return false;
        }
        
        //check pointer
        if(!argin){
            csutils::WRITE_ERROR("Aborting 'move_rel' -> wrong input pointer (Tango)");
            return false;
        }
        
        //check size
        if((*argin).length() != 2){
            csutils::WRITE_ERROR("Aborting 'move_rel' -> wrong input size (Tango)");
            return false;
        }
        
        //locals
        uint32_t axis = 0;
        double distance = 0.0;
        csrp3::targetTower::TargetTower::axis ax;
        
        //check values
        axis = (*argin)[0];
        distance = (*argin)[1];
        
        if(axis > 4){
            csutils::WRITE_ERROR("Aborting 'move_rel' -> wrong axis identifier (Tango)");
            return false;
        }
        
        switch(axis){
            case 0:
                ax = csrp3::targetTower::TargetTower::axis::X;
                break;
            case 1:
                ax = csrp3::targetTower::TargetTower::axis::Y;
                break;
            case 2:
                ax = csrp3::targetTower::TargetTower::axis::Z;
                break;
            case 3:
                ax = csrp3::targetTower::TargetTower::axis::T;
                break;
            case 4:
                ax = csrp3::targetTower::TargetTower::axis::P;
                break;
            default:
                csutils::WRITE_ERROR("Aborting 'move_rel' -> failed to identify the axis (Tango)");
                return false;
        }
        
        //call the movement cmd
        if(!tt->moveRelative(ax, distance)){
            csutils::WRITE_ERROR("Aborting 'move_rel' -> the Target Tower failed "
            "the move command (Tango)");
            return false;
        }
        
        //move to moving
        set_state(Tango::MOVING);
        
        //start the monitor
        if(!moveMonitorStart()){
            tt->abort();
            csutils::WRITE_ERROR("Aborting 'move_rel' -> failed to start the movement monitor thread (Tango)");
            return false;
        }
        
        //finish
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::move_rel
	return argout;
}
//--------------------------------------------------------
/**
 *	Command run related method
 *	Description: x,y,z,t,p
 *
 *	@param argin axis, direction
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::run(const Tango::DevVarULongArray *argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::run()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::run) ENABLED START -----*/
	
	//check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'run' -> wrong state (Tango)");
            return false;
        }
        
        //check pointer
        if(!argin){
            csutils::WRITE_ERROR("Aborting 'run' -> wrong input pointer (Tango)");
            return false;
        }
        
        //check size
        if((*argin).length() != 2){
            csutils::WRITE_ERROR("Aborting 'run' -> wrong input size (Tango)");
            return false;
        }
        
        //locals
        uint32_t axis = 0, direction = 0.0;
        csrp3::targetTower::TargetTower::axis ax;
        bool positiveRun = false;
        
        //check values
        axis = (*argin)[0];
        direction = (*argin)[1];
        
        if(axis > 4){
            csutils::WRITE_ERROR("Aborting 'run' -> wrong axis identifier (Tango)");
            return false;
        }
        
        switch(axis){
            case 0:
                ax = csrp3::targetTower::TargetTower::axis::X;
                break;
            case 1:
                ax = csrp3::targetTower::TargetTower::axis::Y;
                break;
            case 2:
                ax = csrp3::targetTower::TargetTower::axis::Z;
                break;
            case 3:
                ax = csrp3::targetTower::TargetTower::axis::T;
                break;
            case 4:
                ax = csrp3::targetTower::TargetTower::axis::P;
                break;
            default:
                csutils::WRITE_ERROR("Aborting 'run' -> failed to identify the axis (Tango)");
                return false;
        }
        
        //determine direction
        if(direction == 0){
            positiveRun = false;
        }else if(direction == 1){
            positiveRun = true;
        }else{
            csutils::WRITE_ERROR("Aborting 'run' -> failed to determine the direction of run "
            "only {0 and 1} allowed (Tango)");
            return false;
        }
        
        //call the movement cmd
        if(!tt->run(ax, positiveRun)){
            csutils::WRITE_ERROR("Aborting 'run' -> the Target Tower failed "
            "the move command (Tango)");
            return false;
        }
        
        //move to moving
        set_state(Tango::MOVING);
        
        //start the monitor
        if(!moveMonitorStart()){
            tt->abort();
            csutils::WRITE_ERROR("Aborting 'run' -> failed to start the movement monitor thread (Tango)");
            return false;
        }
        
        //finish
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::run
	return argout;
}
//--------------------------------------------------------
/**
 *	Command center related method
 *	Description: 
 *
 *	@param argin holder, sample
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::center(const Tango::DevVarULongArray *argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::center()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::center) ENABLED START -----*/
        (void)argin;
        
//	//check state
//        if(get_state() != Tango::STANDBY){
//            csutils::WRITE_ERROR("Aborting 'center' -> wrong state (Tango)");
//            return false;
//        }
//        
//        //move to init
//        set_state(Tango::MOVING);
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::center
	return argout;
}
//--------------------------------------------------------
/**
 *	Command clearConfiguration related method
 *	Description: 
 *
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::clear_configuration()
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::clearConfiguration()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::clear_configuration) ENABLED START -----*/
	
	//check state
        if(get_state() != Tango::DISABLE){
            csutils::WRITE_ERROR("Aborting 'clear_configuration' -> wrong state (Tango)");
            return false;
        }
        
        //clear the configuration
        if(!tt->clearConfiguration()){
            csutils::WRITE_ERROR("The 'clear_configuration' failed -> the Target Tower "
            "failed to load the configuration file (Tango)");
            return false;
        }
        
        //move to init
        set_state(Tango::INIT);
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::clear_configuration
	return argout;
}
//--------------------------------------------------------
/**
 *	Command disconnect related method
 *	Description: 
 *
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::disconnect()
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::disconnect()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::disconnect) ENABLED START -----*/
	
	//check state
        if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'disconnect' -> wrong state (Tango)");
            return false;
        }
        
        //disconnect
        if(!tt->disconnect()){
            csutils::WRITE_ERROR("Failed the 'disconnect' call -> the Target Tower "
            "failed to disconnect(Tango)");
            return false;
        }
        
        //move to init
        set_state(Tango::INIT);
        argout = true;
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::disconnect
	return argout;
}
//--------------------------------------------------------
/**
 *	Command saveImage related method
 *	Description: 
 *
 *	@param argin file path+name
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::save_image(Tango::DevString argin)
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::saveImage()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::save_image) ENABLED START -----*/
	
        //check state != INIT
	if(get_state() == Tango::INIT){
            csutils::WRITE_ERROR("Aborting 'saveImage' -> wrong state (Tango)");
            return false;
        }
        
        //check input
        std::string fileName = std::string(argin);
        if(fileName.empty()){
            csutils::WRITE_ERROR("Aborting 'saveImage' -> the provided name is empty (Tango)");
            return false;
        }
	
        //extract the current image
        csimage::MonoImage* localImage;
        uint16_t* data = nullptr;
        uint32_t size = 0;
        if(!tt->retrieveLatestImage(&localImage) || !localImage){
            csutils::WRITE_ERROR("Aborting 'saveImage' -> failed to extract image from the camera (Tango)");
        }else{
            localImage->getData(&data, size);
            if(!data || size == 0){
                csutils::WRITE_ERROR("Aborting 'saveImage' -> failed to extract image data (Tango)");
                return false;
            }
        }
        
        //save
        if(!csimage::savePNG(fileName, *localImage)){
            csutils::WRITE_ERROR("Failed to save the image (Tango)");
            return false;
        }
        
        argout = true;
        
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::save_image
	return argout;
}
//--------------------------------------------------------
/**
 *	Command focus related method
 *	Description: 
 *
 *	@returns 
 */
//--------------------------------------------------------
Tango::DevBoolean TargetTowerTangoServer::focus()
{
	Tango::DevBoolean argout;
	DEBUG_STREAM << "TargetTowerTangoServer::focus()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::focus) ENABLED START -----*/
	
	 //check state != INIT
	if(get_state() != Tango::STANDBY){
            csutils::WRITE_ERROR("Aborting 'focus' -> wrong state (Tango)");
            return false;
        }
	
        if(!tt->focus()){
            csutils::WRITE_ERROR("Failure during the 'focus' (Tango)");
            return false;
        }
        
        //move to RUNNING
        set_state(Tango::RUNNING);
        
        if(!aligningMonitorStart()){
            tt->abort(); // abort homing
            set_state(Tango::STANDBY);
            return false;
        }        
        
        argout = true;
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::focus
	return argout;
}
//--------------------------------------------------------
/**
 *	Method      : TargetTowerTangoServer::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void TargetTowerTangoServer::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServer::add_dynamic_commands) ENABLED START -----*/
	
	//	Add your own code to create and add dynamic commands if any
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::add_dynamic_commands
}

/*----- PROTECTED REGION ID(TargetTowerTangoServer::namespace_ending) ENABLED START -----*/

    bool TargetTowerTangoServer::homingMonitorStart() {
        
        //check status
        if(get_state() != Tango::CLOSE){
            csutils::WRITE_ERROR("Aborting 'homingMonitorStart' -> wrong state (Tango)");
            return false;
        }
        
        //check thread status
        if(threading.homingMonitorUp.load()){
            csutils::WRITE_WARNING("The monitor thread is already up");
            return true;
        }else{
            //thread object status
            if(threading.homingMonitor.joinable()){
                threading.homingMonitorKill.store(true);
                threading.homingMonitor.join();
            }
            
            //stop thread flag
            threading.homingMonitorKill.store(false);
            
            //run the thread
            threading.homingMonitor = 
                    std::thread(&TargetTowerTangoServer::homingMonitorThreadFce, this);
            
            //thread flag
            threading.homingMonitorUp.store(true);
        }
        
        return true;
    }

    bool TargetTowerTangoServer::homingMonitorStop() {
        
        //stop thread flag
        threading.homingMonitorKill.store(true);

        //thread object status
        if(threading.homingMonitor.joinable()){
            threading.homingMonitor.join();
        }

        //thread flag
        threading.homingMonitorUp.store(false);
        
        return true;
    }
    
    void TargetTowerTangoServer::homingMonitorThreadFce() {

        while(!threading.homingMonitorKill.load()){
            if(get_state() == Tango::CLOSE && tt->isReady()){
                
                set_state(Tango::STANDBY);
                
                threading.homingMonitorKill.store(true);
                continue;
            }
            usleep(threading.homingMonitorPeriod);
        }
        
        //set thread status flag 
        threading.homingMonitorUp.store(false);
        
        return;
    }
    
    bool TargetTowerTangoServer::aligningMonitorStart(){
        
        //check status
        if(get_state() != Tango::RUNNING){
            csutils::WRITE_ERROR("Aborting 'aligningMonitorStart' -> wrong state (Tango)");
            return false;
        }
        
        //check thread status
        if(threading.aligningMonitorUp.load()){
            csutils::WRITE_WARNING("The aligning monitor thread is already up");
            return true;
        }else{
            //thread object status
            if(threading.aligningMonitor.joinable()){
                threading.aligningMonitorKill.store(true);
                threading.aligningMonitor.join();
            }
            
            //stop thread flag
            threading.aligningMonitorKill.store(false);
            
            //run the thread
            threading.aligningMonitor = 
                    std::thread(&TargetTowerTangoServer::aligningMonitorThreadFce, this);
            
            //thread flag
            threading.aligningMonitorUp.store(true);
        }
        
        return true;
    }

    bool TargetTowerTangoServer::aligningMonitorStop() {
        
        //stop thread flag
        threading.aligningMonitorKill.store(true);

        //thread object status
        if(threading.aligningMonitor.joinable()){
            threading.aligningMonitor.join();
        }

        //thread flag
        threading.aligningMonitorUp.store(false);
        
        return true;
    }
    
    void TargetTowerTangoServer::aligningMonitorThreadFce() {

        while(!threading.aligningMonitorKill.load()){
            if(get_state() == Tango::RUNNING && tt->isReady()){
                
                set_state(Tango::STANDBY);
                
                threading.aligningMonitorKill.store(true);
                continue;
            }
            usleep(threading.aligningMonitorPeriod);
        }
        
        //set thread status flag 
        threading.aligningMonitorUp.store(false);
        
        return;
    }    
    
    bool TargetTowerTangoServer::moveMonitorStart(){
        
        //check status
        if(get_state() != Tango::MOVING){
            csutils::WRITE_ERROR("Aborting 'moveMonitorStart' -> wrong state (Tango)");
            return false;
        }
        
        //check thread status
        if(threading.moveMonitorUp.load()){
            csutils::WRITE_WARNING("The move monitor thread is already up");
            return true;
        }else{
            //thread object status
            if(threading.moveMonitor.joinable()){
                threading.moveMonitorKill.store(true);
                threading.moveMonitor.join();
            }
            
            //stop thread flag
            threading.moveMonitorKill.store(false);
            
            //run the thread
            threading.moveMonitor = 
                    std::thread(&TargetTowerTangoServer::moveMonitorThreadFce, this);
            
            //thread flag
            threading.moveMonitorUp.store(true);
        }
        
        return true;
    }

    bool TargetTowerTangoServer::moveMonitorStop() {
        
        //stop thread flag
        threading.moveMonitorKill.store(true);

        //thread object status
        if(threading.moveMonitor.joinable()){
            threading.moveMonitor.join();
        }

        //thread flag
        threading.moveMonitorUp.store(false);
        
        return true;
    }
    
    void TargetTowerTangoServer::moveMonitorThreadFce() {

        while(!threading.moveMonitorKill.load()){
            if(get_state() == Tango::MOVING && tt->isReady()){
                
                set_state(Tango::STANDBY);
                
                threading.moveMonitorKill.store(true);
                continue;
            }
            usleep(threading.moveMonitorPeriod);
        }
        
        //set thread status flag 
        threading.moveMonitorUp.store(false);
        
        return;
    }      

    bool TargetTowerTangoServer::analyzeMonitorStart(){
        
        //check status
        if(get_state() != Tango::OPEN){
            csutils::WRITE_ERROR("Aborting 'analyzeMonitorStart' -> wrong state (Tango)");
            return false;
        }
        
        //check thread status
        if(threading.analyzeMonitorUp.load()){
            csutils::WRITE_WARNING("The analyze monitor thread is already up");
            return true;
        }else{
            //thread object status
            if(threading.analyzeMonitor.joinable()){
                threading.analyzeMonitorKill.store(true);
                threading.analyzeMonitor.join();
            }
            
            //stop thread flag
            threading.analyzeMonitorKill.store(false);
            
            //run the thread
            threading.analyzeMonitor = 
                    std::thread(&TargetTowerTangoServer::analyzeMonitorThreadFce, this);
            
            //thread flag
            threading.analyzeMonitorUp.store(true);
        }
        
        return true;
    }

    bool TargetTowerTangoServer::analyzeMonitorStop() {
        
        //stop thread flag
        threading.analyzeMonitorKill.store(true);

        //thread object status
        if(threading.analyzeMonitor.joinable()){
            threading.analyzeMonitor.join();
        }

        //thread flag
        threading.analyzeMonitorUp.store(false);
        
        return true;
    }
    
    void TargetTowerTangoServer::analyzeMonitorThreadFce() {

        while(!threading.analyzeMonitorKill.load()){
            if(get_state() == Tango::OPEN && tt->isReady()){
                
                set_state(Tango::STANDBY);
                
                threading.analyzeMonitorKill.store(true);
                continue;
            }
            usleep(threading.analyzeMonitorPeriod);
        }
        
        //set thread status flag 
        threading.analyzeMonitorUp.store(false);
        
        return;
    }      

    
/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::namespace_ending
} //	namespace
