/*
 *************************************************************************
 * @file TargetTowerTangoServer.h
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


#ifndef TargetTowerTangoServer_H
#define TargetTowerTangoServer_H

#include <memory>
#include <thread>
#include <atomic>

#include <tango.h> // Tango API

#include "TargetTower.h" // the tower API


/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer.h

/**
 *  TargetTowerTangoServer class description:
 *    
 */

namespace TargetTowerTangoServer_ns
{
/*----- PROTECTED REGION ID(TargetTowerTangoServer::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations

/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::Additional Class Declarations

class TargetTowerTangoServer : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(TargetTowerTangoServer::Data Members) ENABLED START -----*/

private:

    std::unique_ptr<csrp3::targetTower::TargetTower> tt; // the Target Tower object
    csimage::MonoImage* imageToReturn; // the image returned from the tt
    
    //sequence monitoring thread
    struct{
        std::thread homingMonitor;
        std::atomic<bool> homingMonitorKill;
        std::atomic<bool> homingMonitorUp;
        const uint32_t homingMonitorPeriod = 100000; // 100 ms        
        
        std::thread aligningMonitor;
        std::atomic<bool> aligningMonitorKill;
        std::atomic<bool> aligningMonitorUp;
        const uint32_t aligningMonitorPeriod = 100000; // 100 ms 
        
        std::thread moveMonitor;
        std::atomic<bool> moveMonitorKill;
        std::atomic<bool> moveMonitorUp;
        const uint32_t moveMonitorPeriod = 100000; // 100 ms
        
        std::thread analyzeMonitor;
        std::atomic<bool> analyzeMonitorKill;
        std::atomic<bool> analyzeMonitorUp;
        const uint32_t analyzeMonitorPeriod = 100000; // 100 ms        
        
    }threading;
    
/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::Data Members


//	Attribute data members
public:
	Tango::DevDouble	*attr_positionX_read;
	Tango::DevDouble	*attr_positionY_read;
	Tango::DevDouble	*attr_positionZ_read;
	Tango::DevDouble	*attr_positionP_read;
	Tango::DevDouble	*attr_positionT_read;
	Tango::DevDouble	*attr_deltaZX_read;
	Tango::DevDouble	*attr_deltaZY_read;
	Tango::DevUShort	*attr_cameraFeed_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TargetTowerTangoServer(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TargetTowerTangoServer(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	TargetTowerTangoServer(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~TargetTowerTangoServer() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : TargetTowerTangoServer::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute positionX related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_positionX(Tango::Attribute &attr);
	virtual bool is_positionX_allowed(Tango::AttReqType type);
/**
 *	Attribute positionY related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_positionY(Tango::Attribute &attr);
	virtual bool is_positionY_allowed(Tango::AttReqType type);
/**
 *	Attribute positionZ related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_positionZ(Tango::Attribute &attr);
	virtual bool is_positionZ_allowed(Tango::AttReqType type);
/**
 *	Attribute positionP related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_positionP(Tango::Attribute &attr);
	virtual bool is_positionP_allowed(Tango::AttReqType type);
/**
 *	Attribute positionT related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_positionT(Tango::Attribute &attr);
	virtual bool is_positionT_allowed(Tango::AttReqType type);
/**
 *	Attribute deltaZX related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_deltaZX(Tango::Attribute &attr);
	virtual bool is_deltaZX_allowed(Tango::AttReqType type);
/**
 *	Attribute deltaZY related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_deltaZY(Tango::Attribute &attr);
	virtual bool is_deltaZY_allowed(Tango::AttReqType type);
/**
 *	Attribute cameraFeed related methods
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Image max = 5000 x 5000
 */
	virtual void read_cameraFeed(Tango::Attribute &attr);
	virtual bool is_cameraFeed_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : TargetTowerTangoServer::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command loadConfiguration related method
	 *	Description: 
	 *
	 *	@param argin 
	 *	@returns 
	 */
	virtual Tango::DevBoolean load_configuration(Tango::DevString argin);
	virtual bool is_loadConfiguration_allowed(const CORBA::Any &any);
	/**
	 *	Command home related method
	 *	Description: 
	 *
	 *	@returns 
	 */
	virtual Tango::DevBoolean home();
	virtual bool is_home_allowed(const CORBA::Any &any);
	/**
	 *	Command abort related method
	 *	Description: 
	 *
	 *	@returns 
	 */
	virtual Tango::DevBoolean abort();
	virtual bool is_abort_allowed(const CORBA::Any &any);
	/**
	 *	Command align related method
	 *	Description: 
	 *
	 *	@param argin align x {0,1}, align y {0,1}
	 *	@returns 
	 */
	virtual Tango::DevBoolean align(const Tango::DevVarULongArray *argin);
	virtual bool is_align_allowed(const CORBA::Any &any);
	/**
	 *	Command analyse related method
	 *	Description: 
	 *
	 *	@param argin holder no
	 *	@returns 
	 */
	virtual Tango::DevBoolean analyse(Tango::DevULong argin);
	virtual bool is_analyse_allowed(const CORBA::Any &any);
	/**
	 *	Command moveAbs related method
	 *	Description: x,y,z,t,p
	 *
	 *	@param argin axis, distance
	 *	@returns 
	 */
	virtual Tango::DevBoolean move_abs(const Tango::DevVarDoubleArray *argin);
	virtual bool is_moveAbs_allowed(const CORBA::Any &any);
	/**
	 *	Command moveRel related method
	 *	Description: x,y,z,t,p
	 *
	 *	@param argin axis, distance
	 *	@returns 
	 */
	virtual Tango::DevBoolean move_rel(const Tango::DevVarDoubleArray *argin);
	virtual bool is_moveRel_allowed(const CORBA::Any &any);
	/**
	 *	Command run related method
	 *	Description: x,y,z,t,p
	 *
	 *	@param argin axis, direction
	 *	@returns 
	 */
	virtual Tango::DevBoolean run(const Tango::DevVarULongArray *argin);
	virtual bool is_run_allowed(const CORBA::Any &any);
	/**
	 *	Command center related method
	 *	Description: 
	 *
	 *	@param argin holder, sample
	 *	@returns 
	 */
	virtual Tango::DevBoolean center(const Tango::DevVarULongArray *argin);
	virtual bool is_center_allowed(const CORBA::Any &any);
	/**
	 *	Command clearConfiguration related method
	 *	Description: 
	 *
	 *	@returns 
	 */
	virtual Tango::DevBoolean clear_configuration();
	virtual bool is_clearConfiguration_allowed(const CORBA::Any &any);
	/**
	 *	Command disconnect related method
	 *	Description: 
	 *
	 *	@returns 
	 */
	virtual Tango::DevBoolean disconnect();
	virtual bool is_disconnect_allowed(const CORBA::Any &any);
	/**
	 *	Command saveImage related method
	 *	Description: 
	 *
	 *	@param argin file path+name
	 *	@returns 
	 */
	virtual Tango::DevBoolean save_image(Tango::DevString argin);
	virtual bool is_saveImage_allowed(const CORBA::Any &any);
	/**
	 *	Command focus related method
	 *	Description: 
	 *
	 *	@returns 
	 */
	virtual Tango::DevBoolean focus();
	virtual bool is_focus_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : TargetTowerTangoServer::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(TargetTowerTangoServer::Additional Method prototypes) ENABLED START -----*/

        //the sequence monitor thread
        void homingMonitorThreadFce();
        
        //starts the monitor thread
        bool homingMonitorStart();
        
        //stops the monitor thread
        bool homingMonitorStop();

        //the sequence monitor thread
        void aligningMonitorThreadFce();
        
        //starts the monitor thread
        bool aligningMonitorStart();
        
        //stops the monitor thread
        bool aligningMonitorStop();        
        
        //the sequence monitor thread
        void moveMonitorThreadFce();
        
        //starts the monitor thread
        bool moveMonitorStart();
        
        //stops the monitor thread
        bool moveMonitorStop();          
        
        //the analyze monitor thread
        void analyzeMonitorThreadFce();
        
        //starts the monitor thread
        bool analyzeMonitorStart();
        
        //stops the monitor thread
        bool analyzeMonitorStop();                  
        
/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::Additional Method prototypes
};

/*----- PROTECTED REGION ID(TargetTowerTangoServer::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServer::Additional Classes Definitions

}	//	End of namespace

#endif   //	TargetTowerTangoServer_H
