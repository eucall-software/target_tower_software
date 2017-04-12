/*----- PROTECTED REGION ID(TargetTowerTangoServerClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        TargetTowerTangoServerClass.h
//
// description : Include for the TargetTowerTangoServer root class.
//               This class is the singleton class for
//                the TargetTowerTangoServer device class.
//               It contains all properties and methods which the 
//               TargetTowerTangoServer requires only once e.g. the commands.
//
// project :     
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef TargetTowerTangoServerClass_H
#define TargetTowerTangoServerClass_H

#include <tango.h>
#include <TargetTowerTangoServer.h>


/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServerClass.h


namespace TargetTowerTangoServer_ns
{
/*----- PROTECTED REGION ID(TargetTowerTangoServerClass::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServerClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute positionX class definition
class positionXAttrib: public Tango::Attr
{
public:
	positionXAttrib():Attr("positionX",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~positionXAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_positionX(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_positionX_allowed(ty);}
};

//	Attribute positionY class definition
class positionYAttrib: public Tango::Attr
{
public:
	positionYAttrib():Attr("positionY",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~positionYAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_positionY(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_positionY_allowed(ty);}
};

//	Attribute positionZ class definition
class positionZAttrib: public Tango::Attr
{
public:
	positionZAttrib():Attr("positionZ",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~positionZAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_positionZ(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_positionZ_allowed(ty);}
};

//	Attribute positionP class definition
class positionPAttrib: public Tango::Attr
{
public:
	positionPAttrib():Attr("positionP",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~positionPAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_positionP(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_positionP_allowed(ty);}
};

//	Attribute positionT class definition
class positionTAttrib: public Tango::Attr
{
public:
	positionTAttrib():Attr("positionT",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~positionTAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_positionT(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_positionT_allowed(ty);}
};

//	Attribute deltaZX class definition
class deltaZXAttrib: public Tango::Attr
{
public:
	deltaZXAttrib():Attr("deltaZX",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~deltaZXAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_deltaZX(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_deltaZX_allowed(ty);}
};

//	Attribute deltaZY class definition
class deltaZYAttrib: public Tango::Attr
{
public:
	deltaZYAttrib():Attr("deltaZY",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~deltaZYAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_deltaZY(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_deltaZY_allowed(ty);}
};

//	Attribute cameraFeed class definition
class cameraFeedAttrib: public Tango::ImageAttr
{
public:
	cameraFeedAttrib():ImageAttr("cameraFeed",
			Tango::DEV_USHORT, Tango::READ, 5000, 5000) {};
	~cameraFeedAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TargetTowerTangoServer *>(dev))->read_cameraFeed(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TargetTowerTangoServer *>(dev))->is_cameraFeed_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command loadConfiguration class definition
class loadConfigurationClass : public Tango::Command
{
public:
	loadConfigurationClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	loadConfigurationClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~loadConfigurationClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_loadConfiguration_allowed(any);}
};

//	Command home class definition
class homeClass : public Tango::Command
{
public:
	homeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	homeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~homeClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_home_allowed(any);}
};

//	Command abort class definition
class abortClass : public Tango::Command
{
public:
	abortClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	abortClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~abortClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_abort_allowed(any);}
};

//	Command align class definition
class alignClass : public Tango::Command
{
public:
	alignClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	alignClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~alignClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_align_allowed(any);}
};

//	Command analyse class definition
class analyseClass : public Tango::Command
{
public:
	analyseClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	analyseClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~analyseClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_analyse_allowed(any);}
};

//	Command moveAbs class definition
class moveAbsClass : public Tango::Command
{
public:
	moveAbsClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	moveAbsClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~moveAbsClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_moveAbs_allowed(any);}
};

//	Command moveRel class definition
class moveRelClass : public Tango::Command
{
public:
	moveRelClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	moveRelClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~moveRelClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_moveRel_allowed(any);}
};

//	Command run class definition
class runClass : public Tango::Command
{
public:
	runClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	runClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~runClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_run_allowed(any);}
};

//	Command center class definition
class centerClass : public Tango::Command
{
public:
	centerClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	centerClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~centerClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_center_allowed(any);}
};

//	Command clearConfiguration class definition
class clearConfigurationClass : public Tango::Command
{
public:
	clearConfigurationClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	clearConfigurationClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~clearConfigurationClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_clearConfiguration_allowed(any);}
};

//	Command disconnect class definition
class disconnectClass : public Tango::Command
{
public:
	disconnectClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	disconnectClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~disconnectClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_disconnect_allowed(any);}
};

//	Command saveImage class definition
class saveImageClass : public Tango::Command
{
public:
	saveImageClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	saveImageClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~saveImageClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_saveImage_allowed(any);}
};

//	Command focus class definition
class focusClass : public Tango::Command
{
public:
	focusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	focusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~focusClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TargetTowerTangoServer *>(dev))->is_focus_allowed(any);}
};


/**
 *	The TargetTowerTangoServerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  TargetTowerTangoServerClass : public Tango::DeviceClass
#else
class TargetTowerTangoServerClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(TargetTowerTangoServerClass::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	TargetTowerTangoServerClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static TargetTowerTangoServerClass *init(const char *);
		static TargetTowerTangoServerClass *instance();
		~TargetTowerTangoServerClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		TargetTowerTangoServerClass(string &);
		static TargetTowerTangoServerClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	TargetTowerTangoServer_H
