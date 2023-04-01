//--------------------------------------------------------------------------
// Helicopter External Flight Model for DCS World
// 
// Original F-16 Demo Author: CptSmiley (forums.eagle.ru username)
// Use Only for Non-Commercial Purposes
//
//Adapted to helicopter by Nibbylot
//
//--------------------------------------------------------------------------
// Source Data:
// 1) NASA CR-3144
// 2) Natops... page 23-1,   LCW
//--------------------------------------------------------------------------
// CH53.cpp : Defines the exported functions for the DLL application.
// Control the main portion of the discrete simulation event
//
// This project will compile a DLL.  This DLL needs to be compiled with the
// same machine type of your machine (x86 or x64).  This DLL then needs to
// be placed within the bin directory in your mod/aircraft/XXXairplane/ 
// directory within DCS World.  
//
// See associated entry.lua for how to tell the mod to use the DLL flight
// model
//--------------------------------------------------------------------------
// IMPORTANT!  COORDINATE CONVENTION:
//
// DCS WORLD Convention:
// Xbody: Out the front of the nose
// Ybody: Out the top of the aircraft
// Zbody: Out the right wing
//
// Normal Aerodynamics/Control Convention:
// Xbody: Out the front of the nose
// Ybody: Out the right wing
// Zbody: Out the bottom of the aircraft
//
// This means that if you are referincing from any aerodynamic, stabilty, or control document
// they are probably using the second set of directions.  Which means you always need to switch
// the Y and the Z and reverse the Y prior to output to DCS World
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>


// ED's API
#include "Include/Cockpit/CockpitAPI_Declare.h" // Provides param handle interfacing for use in lua
#include "include/FM/API_Declare.h"	// Provides all DCS related functions in this cpp file

#include "include/ED_FM_Utility.h"		// Provided utility functions that were in the initial EFM example
#include "include/UtilityFunctions.h"

#include "include/CH53Constants.h"		// Common constants used throughout this DLL
#include "Inputs/CH53Inputs.h"			// just list of inputs: can get potentially long list

// Model headers
#include "Engine/CH53Engine.h"					//Engine model functions
#include "Engine/CH53FuelSystem.h"				//Fuel usage and tank usage functions
#include "Airframe/CH53Airframe.h"				//Canopy, dragging chute, refuel slot, section damages..
#include "Electrics/CH53ElectricSystem.h"		//Generators, battery etc.
#include "Aerodynamics/CH53Aero.h"				//Aerodynamic model functions
#include "EquationsOfMotion/CH53EquationsOfMotion.h"
#include "Engine/CH53Gear.h"						//Gear model functions
#include "PID/pid.h"						//Gear model functions
#include <chrono>


//-------------------------------------------------------
// Start of Simulation Variables
//-------------------------------------------------------
namespace Helicopter
{
	double		alpha_DEG			= 0.0;	// Angle of attack (deg)
	double		beta_DEG			= 0.0;	// Slideslip angle (deg)
	double		rollRate_RPS		= 0.0;	// Body roll rate (rad/sec)
	double		pitchRate_RPS		= 0.0;	// Body pitch rate (rad/sec)
	double		yawRate_RPS			= 0.0;	// Body yaw rate (rad/sec)
	double		CollectiveInput		= 0.0;	// Raw collective input
	double		PedalInput			= 0.0;	// Raw pedal input	
	double		PitchInput			= 0.0;	// Raw pitch input
	double		RollInput			= 0.0;	// Raw roll input
	double		CollectiveControl	= 0.0;	// collective input adjusted for control
	double		PedalControl		= 0.0;	// pedal input adjusted for control	
	double		PitchControl		= 0.0;	// pitch input adjusted for control
	double		RollControl			= 0.0;	// roll input adjusted for control
	double		rollTrim			= 0.0;
	double		pitchTrim			= 0.0;
	double		OutsideAirTemp		= 0.0;	// deg C, for temp gauge

	double pidPValue = 0.0;
	double pidIValue = 0.0;
	double pidDValue = 0.0;
	int autopilotState = 0;
	double autopilot_target_heading = -1.0;	// set to -1 so that I know it was turned off

	double		ay_world			= 0.0;	// World referenced up/down acceleration (m/s^2)
	double		accz				= 0.0;	// Az (per normal direction convention) out the bottom of the a/c (m/s^2)
	double		accy				= 0.0;	// Ay (per normal direction convention) out the right wing (m/s^2)

	double autopilot_yaw_differential = 0.0;
	double autopilot_roll_differential = 0.0;
	double autopilot_pitch_differential = 0.0;
	double autopilot_collective_differential = 0.0;

	CH53Engine Engine;
	CH53FuelSystem Fuel;
	CH53Airframe Airframe;
	CH53Aero Aero;
	CH53Motion Motion;
	CH53ElectricSystem Electrics;
	EDPARAM cockpitAPI;
}


// This is where the simulation send the accumulated forces to the DCS Simulation after each run frame
void ed_fm_add_local_force(double &x,double &y,double &z,double &pos_x,double &pos_y,double &pos_z)
{
	Helicopter::Motion.getLocalForce(x, y, z, pos_x, pos_y, pos_z);
}

// same but in component form , return value bool : function will be called until return value is true
bool ed_fm_add_local_force_component (double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{
	return false;
}

// This is where the simulation send the accumulated moments to the DCS Simulation
// after each run frame
void ed_fm_add_local_moment(double &x,double &y,double &z)
{
	Helicopter::Motion.getLocalMoment(x, y, z);
}

// same but in component form , return value bool : function will be called until return value is true
bool ed_fm_add_local_moment_component (double & x,double &y,double &z)
{
	return false;
}


//This will store a parameter that can be read in LUA. Useful for transferring data between the dll and lua code
void* TEST_PARAM = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM");
void* TEST_PARAM_2 = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_2");
void* TEST_PARAM_3 = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_3");
void* TEST_PARAM_4 = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_4");

void* TEST_PARAM_THRUST_AVAILABLE = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_THRUST_AVAILABLE");
void* TEST_PARAM_THRUST_REQUIRED = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_THRUST_REQUIRED");
void* TEST_PARAM_THRUST_PRODUCED = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_THRUST_PRODUCED");

void* TEST_PARAM_WORLD_FORCE_X = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_WORLD_FORCE_X");
void* TEST_PARAM_WORLD_FORCE_Y = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_WORLD_FORCE_Y");
void* TEST_PARAM_WORLD_FORCE_Z = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_WORLD_FORCE_Z");

void* TEST_PARAM_BODY_PITCH = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_BODY_PITCH");
void* TEST_PARAM_BODY_ROLL = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_BODY_ROLL");
void* TEST_PARAM_BODY_YAW = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_BODY_YAW");

void* TEST_PARAM_AUTOPILOT_YAW = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_AUTOPILOT_YAW");
void* TEST_PARAM_AUTOPILOT_INC = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_AUTOPILOT_INC");

void* TEST_PARAM_PID_P = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_PID_P");
void* TEST_PARAM_PID_I = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_PID_I");
void* TEST_PARAM_PID_D = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_PID_D");

void* TEST_PARAM_CX = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CX");
void* TEST_PARAM_CY = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CY");
void* TEST_PARAM_CZ = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CZ");
void* TEST_PARAM_CL = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CL");
void* TEST_PARAM_CM = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CM");
void* TEST_PARAM_CN = Helicopter::cockpitAPI.getParamHandle("TEST_PARAM_CN");

// These are used for the controls indicator
void* PITCH_INPUT = Helicopter::cockpitAPI.getParamHandle("PITCH_INPUT");
void* ROLL_INPUT = Helicopter::cockpitAPI.getParamHandle("ROLL_INPUT");
void* PEDAL_INPUT = Helicopter::cockpitAPI.getParamHandle("PEDAL_INPUT");
void* COLLECTIVE_INPUT = Helicopter::cockpitAPI.getParamHandle("COLLECTIVE_INPUT");


//-----------------------------------------------------------------------
// The most important part of the entire EFM code.  This is where your code
// gets called for each run frame.  Each run frame last for a duration of
// "dt". dt is actually slice-length DCS wants code to limit itself to,
// not actually time passed since last frame - it is constant 0.006000 seconds.
void ed_fm_simulate(double dt)
{
	
	unsigned __int64 time_now_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	unsigned __int64 time_diff = time_now_millis - Helicopter::time_now_millis_last;

	double inc_radalt = 0;

	if (true) {
		// Very important! clear out the forces and moments before you start calculated
		// a new set for this run frame
		Helicopter::Motion.clear();

		// Get the total absolute velocity acting on the aircraft with wind included
		// using english units so airspeed is in feet/second here
		Helicopter::Motion.updateFrame(dt);


		// update amount of fuel used and change in mass
		Helicopter::Fuel.update(Helicopter::Engine.getCoreRelatedRPM(), dt); // note, still uses fuel when sim is paused
		Helicopter::Engine.update(dt, Helicopter::Fuel.isFuelFlow, Helicopter::Electrics.isDC_busPowered, Helicopter::OutsideAirTemp, Helicopter::CollectiveInput);

		Helicopter::Electrics.update(dt, Helicopter::Engine.N1_RPM);
		Helicopter::Airframe.updateFrame(dt);


		// AFCS
		//
		if (Helicopter::autopilotState == 0) {
			Helicopter::autopilot_pitch_differential = 0;
			Helicopter::autopilot_roll_differential = 0;
			Helicopter::autopilot_yaw_differential = 0;
			Helicopter::autopilot_target_heading = -1;	// set to -1 so that I know it was turned off
		}
		else if (Helicopter::autopilotState == 1) {


			double current_heading = fmod(360 - (Helicopter::Motion.yaw * (180 / 3.14159)), 360);	// this is wierd AF. its like Helicopter::Motion.yaw is flipped on the 0 - 180 axis..but not then rotated 90 degrees like normal?
			if (Helicopter::autopilot_target_heading == -1) {
				Helicopter::autopilot_target_heading = current_heading;
			}
			double target_heading = Helicopter::autopilot_target_heading;
			double heading_diff = target_heading - current_heading; // this will make heading_diff +/- 180 max.

			//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_2, current_heading);
			//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_REQUIRED, heading_diff);
			//PID pid_yaw = PID(0.06, Helicopter::pidPValue * 5, Helicopter::pidPValue * -5, Helicopter::pidPValue, Helicopter::pidIValue, Helicopter::pidDValue);
			PID pid_yaw = PID(0.2, 1, -1, 0.200, 0.012, 0.036);
			double inc_yaw = pid_yaw.calculate(0, heading_diff);
			Helicopter::autopilot_yaw_differential = inc_yaw;
			if ((Helicopter::PedalInput > 0.3) || (Helicopter::PedalInput < -0.3)) {
				Helicopter::autopilot_yaw_differential = 0;
			}


			double target_roll = 0;
			double target_roll_diff = -(target_roll - (Helicopter::Motion.roll * (180 / 3.14159)));
			//PID pid_roll = PID(0.06, Helicopter::pidPValue * 5, Helicopter::pidPValue * -5, Helicopter::pidPValue, Helicopter::pidIValue, Helicopter::pidDValue);
			PID pid_roll = PID(0.2, 3, -3, 0.202, 0.002, 0.020);
			double inc_roll = pid_roll.calculate(0, target_roll_diff);
			Helicopter::autopilot_roll_differential = inc_roll;
			if ((Helicopter::RollInput > 0.3) || (Helicopter::RollInput < -0.3)) {
				Helicopter::autopilot_roll_differential = 0;
			}


			double target_pitch = -1.3;
			double target_pitch_diff = (target_pitch - (Helicopter::Motion.pitch * (180 / 3.14159)));
			//PID pid_pitch = PID(time_diff / 1000, 90, -90, Helicopter::pidPValue, Helicopter::pidIValue, Helicopter::pidDValue);
			PID pid_pitch = PID(0.2, 90, -90, 0.090, 0.025, 0.001);
			double inc_pitch = pid_pitch.calculate(0, target_pitch_diff);
			Helicopter::autopilot_pitch_differential = inc_pitch;
			//Helicopter::pitchTrim = inc_pitch;
			if ((Helicopter::PitchInput > 0.3) || (Helicopter::PitchInput < -0.3)) {
				Helicopter::autopilot_pitch_differential = 0;
			}


			double target_radalt = 50;
			double target_radalt_diff = (target_radalt - Helicopter::Motion.altitudeAGL);
			//PID pid_radalt = PID(0.2, 100, -100, Helicopter::pidPValue, Helicopter::pidIValue, Helicopter::pidDValue);
			PID pid_radalt = PID(0.2, 100, -100, 1.0, 0.03, 0.05);
			inc_radalt = pid_radalt.calculate(0, target_radalt_diff);
			Helicopter::autopilot_collective_differential = -limit((inc_radalt/ 10),-1,1);
			//Helicopter::pitchTrim = inc_pitch;
			if (Helicopter::CollectiveInput > 0.9) {
				Helicopter::autopilot_collective_differential = 0;
			}

		}

		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_PID_P, Helicopter::pidPValue);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_PID_I, Helicopter::pidIValue);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_PID_D, Helicopter::pidDValue);

		//printf("(%d) current_roll:% 7.3f inc:% 7.3f\n", i, current_roll, inc);
		//current_roll += inc;

		//PID pid = PID(0.1, 1, -1, 0.1, 0.01, 0.5);
		//double inc = pid.calculate(target_roll, current_roll);






		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_AUTOPILOT_INC, inc);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_AUTOPILOT_YAW, Helicopter::autopilot_pitch_differential);
		//-----CONTROL DYNAMICS------------------------
		// the four control constants seen here are the physical delfection amount in cm from the NASA report.
		Helicopter::RollControl = limit((Helicopter::RollInput + Helicopter::rollTrim + Helicopter::autopilot_roll_differential), -1, 1) * (22.61 / 2.0);
		Helicopter::PitchControl = (limit((Helicopter::PitchInput + Helicopter::pitchTrim + Helicopter::autopilot_pitch_differential), -1, 1) * (31.04 / 2.0));
		Helicopter::PedalControl = (Helicopter::PedalInput + Helicopter::autopilot_yaw_differential) * (12.95 / 2.0);

		Helicopter::CollectiveControl = (((Helicopter::CollectiveInput + Helicopter::autopilot_collective_differential) * (Helicopter::blade_pitch_max - Helicopter::blade_pitch_min)) + Helicopter::blade_pitch_min);

		Helicopter::cockpitAPI.setParamNumber(PITCH_INPUT, limit((Helicopter::PitchInput + Helicopter::pitchTrim + Helicopter::autopilot_pitch_differential), -1, 1));
		Helicopter::cockpitAPI.setParamNumber(ROLL_INPUT, limit((Helicopter::RollInput + Helicopter::rollTrim + Helicopter::autopilot_roll_differential), -1, 1));
		Helicopter::cockpitAPI.setParamNumber(PEDAL_INPUT, Helicopter::PedalInput + Helicopter::autopilot_yaw_differential);
		Helicopter::cockpitAPI.setParamNumber(COLLECTIVE_INPUT, Helicopter::CollectiveInput + Helicopter::autopilot_collective_differential);
		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM, Helicopter::CollectiveInput);
		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM, Helicopter::Motion.airspeed.z);


		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_REQUIRED, Helicopter::pidIValue);

		// CT, a bit like CL, depends on several geometrical factors and trade-off, 
		// but in general its maximum value is within the range 0.008 and 0.028, 
		// according to how much optimised the rotor is.

		//Thrust =  1/2 * rho * (omega*R) ^2  * (piR^2) * CT
		double rho = 1.225;
		double omega = 185 * 0.1047198 * Helicopter::Engine.getCoreRelatedRPM(); // RPM in rad/s
		double R = 12.04; // radius
		double CT = 0.02;	//Coefficient of Torque
		double blade_pitch = ((Helicopter::CollectiveInput * (Helicopter::blade_pitch_max - Helicopter::blade_pitch_min)) + Helicopter::blade_pitch_min);

		double thrust = 0.5 * rho * ((omega * R) * (omega * R)) * (3.14159 * (R * R)) * CT;
		// so max thrust is: 153,291  at 100% rpm

		Helicopter::Aero.setMassKg(Helicopter::Motion.getMassKg());
		Helicopter::Aero.setBladePitchFactor(Helicopter::CollectiveInput);  // inputs 0-1
		Helicopter::Engine.setBladePitchAngle(blade_pitch);
		Helicopter::Engine.setRotorBladeLength(Helicopter::rotor_blade_length);


		// set for ground effect simulation, rotor diameter AH6 is 8.33m --22.3 ch53
		Helicopter::Aero.setAltitudeAGL(Helicopter::Motion.altitudeAGL);
		double rotor_radius = Helicopter::main_rotor_diameter / 2;
		if (Helicopter::Motion.altitudeAGL <= rotor_radius)
		{
			// using linear dropoff (but a graph i saw showed that it should be exponential)
			//Helicopter::Aero.setGroundEffectFactor(1 - Helicopter::Motion.altitudeAGL / 22.3);
			/*
			At a rotor height of one-half rotor diameter, the thrust is increased about 7 percent.
			At rotor heights above one rotor diameter, the thrust increase is small and decreases to zero at a height of about 1 1/4 rotor diameters.
			Maximum ground effect is accomplished when hovering over smooth paved surfaces.
			While hovering over tall grass, rough terrain, revetments, or water, ground effect may be seriously reduced.
			This phenomena is due to the partial breakdown and cancellation of ground effect and the return of large vortex patterns with increased downwash angles.
			*/
			// Trying a simple curve LCW
			// https://www.transum.org/Maths/Activity/Graph/Desmos.asp
			//  y= (1/22.3) * (x-22.3) ^2

			double y = (1 / rotor_radius) * ((Helicopter::Motion.altitudeAGL - rotor_radius) * (Helicopter::Motion.altitudeAGL - rotor_radius));
			double y_normalized = (y / rotor_radius);
			Helicopter::Aero.setGroundEffectFactor(y_normalized);
		}
		else
		{
			Helicopter::Aero.setGroundEffectFactor(0);
		}

		Helicopter::Aero.computeTotals(Helicopter::Motion.airspeed.x, Helicopter::Motion.airspeed.y, Helicopter::Motion.airspeed.z,
			Helicopter::pitchRate_RPS, Helicopter::rollRate_RPS, Helicopter::yawRate_RPS,
			Helicopter::CollectiveControl, Helicopter::PitchControl, Helicopter::RollControl, Helicopter::PedalControl, Helicopter::Motion.airspeed_KTS, Helicopter::Engine.getCoreRelatedRPM(), Helicopter::Airframe.rotorIntegrityFactor, Helicopter::Airframe.tailRotorIntegrityFactor);


		Helicopter::Motion.updateAeroForces(Helicopter::Aero.getCxTotal(), Helicopter::Aero.getCxTotalNoMass(), Helicopter::Aero.getCzTotal(), Helicopter::Aero.getCmTotal(), Helicopter::Aero.getCyTotal(), Helicopter::Aero.getClTotal(), Helicopter::Aero.getCnTotal());

		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_AVAILABLE, Helicopter::Aero.weight_factor * 100);
		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_AVAILABLE, thrust);	//thrust available
		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_REQUIRED, Helicopter::Aero.getCmTotal());	//thrust available
		//Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_THRUST_PRODUCED, Helicopter::Aero.getCxTotal());




		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CX, Helicopter::Motion.altitudeAGL);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CY, inc_radalt);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CZ, 50 - Helicopter::Motion.altitudeAGL);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CL, 0);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CM, 0);
		Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_CN, Helicopter::autopilot_collective_differential);



		Helicopter::Motion.updateFuelUsageMass(Helicopter::Fuel.getUsageSinceLastFrame(), 0, 0, 0);
		Helicopter::Fuel.clearUsageSinceLastFrame();

	}
	Helicopter::time_now_millis_last = time_now_millis;
}

/*
called before simulation to set up your environment for the next step
give parameters of surface under your aircraft 
*/
void ed_fm_set_surface (double		h,//height of surface under the center of aircraft
						double		h_obj,//height of surface with objects
						unsigned	surface_type,
						double		normal_x,//components of normal vector to surface
						double		normal_y,//components of normal vector to surface
						double		normal_z//components of normal vector to surface
						)
{	
	Helicopter::Motion.setSurface(h_obj);
}

void ed_fm_set_atmosphere(	double h,//altitude above sea level			(meters)
							double t,//current atmosphere temperature   (Kelvin)
							double a,//speed of sound					(meters/sec)
							double ro,// atmosphere density				(kg/m^3)
							double p,// atmosphere pressure				(N/m^2)
							double wind_vx,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
							double wind_vy,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
							double wind_vz //components of velocity vector, including turbulence in world coordinate system (meters/sec)
						)
{
	Helicopter::Motion.setAtmosphere(t, ro, a, h, p); 
	Helicopter::OutsideAirTemp = t - 273;
}

void ed_fm_set_current_mass_state ( double mass,
									double center_of_mass_x,
									double center_of_mass_y,
									double center_of_mass_z,
									double moment_of_inertia_x,
									double moment_of_inertia_y,
									double moment_of_inertia_z
									)
{
	Helicopter::Motion.setMassState(mass,
							center_of_mass_x, center_of_mass_y, center_of_mass_z,
							moment_of_inertia_x, moment_of_inertia_y, moment_of_inertia_z);
}

/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_state (double ax,//linear acceleration component in world coordinate system
							double ay,//linear acceleration component in world coordinate system
							double az,//linear acceleration component in world coordinate system
							double vx,//linear velocity component in world coordinate system
							double vy,//linear velocity component in world coordinate system
							double vz,//linear velocity component in world coordinate system
							double px,//center of the body position in world coordinate system
							double py,//center of the body position in world coordinate system
							double pz,//center of the body position in world coordinate system
							double omegadotx,//angular accelearation components in world coordinate system
							double omegadoty,//angular accelearation components in world coordinate system
							double omegadotz,//angular accelearation components in world coordinate system
							double omegax,//angular velocity components in world coordinate system
							double omegay,//angular velocity components in world coordinate system
							double omegaz,//angular velocity components in world coordinate system
							double quaternion_x,//orientation quaternion components in world coordinate system
							double quaternion_y,//orientation quaternion components in world coordinate system
							double quaternion_z,//orientation quaternion components in world coordinate system
							double quaternion_w //orientation quaternion components in world coordinate system
							)
{
	Helicopter::ay_world = ay;
}

void ed_fm_set_current_state_body_axis(	double ax,//linear acceleration component in body coordinate system (meters/sec^2)
										double ay,//linear acceleration component in body coordinate system (meters/sec^2)
										double az,//linear acceleration component in body coordinate system (meters/sec^2)
										double vx,//linear velocity component in body coordinate system (meters/sec)
										double vy,//linear velocity component in body coordinate system (meters/sec)
										double vz,//linear velocity component in body coordinate system (meters/sec)
										double wind_vx,//wind linear velocity component in body coordinate system (meters/sec)
										double wind_vy,//wind linear velocity component in body coordinate system (meters/sec)
										double wind_vz,//wind linear velocity component in body coordinate system (meters/sec)
										double omegadotx,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegadoty,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegadotz,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegax,//angular velocity components in body coordinate system (rad/sec)
										double omegay,//angular velocity components in body coordinate system (rad/sec)
										double omegaz,//angular velocity components in body coordinate system (rad/sec)
										double yaw,  //radians (rad)  components in body coordinate system
										double pitch,//radians (rad/sec)  components in body coordinate system
										double roll, //radians (rad/sec)  components in body coordinate system
										double common_angle_of_attack, //AoA  (rad)
										double common_angle_of_slide   //AoS  (rad)
										)
{
	Helicopter::Motion.setAirspeed(vx, vy, vz, wind_vx, wind_vy, wind_vz);

	Helicopter::alpha_DEG		= common_angle_of_attack * Helicopter::radiansToDegrees;
	Helicopter::beta_DEG		= common_angle_of_slide * Helicopter::radiansToDegrees;
	Helicopter::rollRate_RPS	= omegax;
	Helicopter::pitchRate_RPS	= omegaz;
	Helicopter::yawRate_RPS	= -omegay;

	Helicopter::accz = ay;
	Helicopter::accy = az;

	Helicopter::Motion.pitch = pitch;
	Helicopter::Motion.yaw = yaw;
	Helicopter::Motion.roll = roll;

	Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_BODY_PITCH, pitch * (180/3.14159));
	Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_BODY_ROLL, roll * (180 / 3.14159));
	Helicopter::cockpitAPI.setParamNumber(TEST_PARAM_BODY_YAW, yaw * (180 / 3.14159));

}

// list of input enums kept in separate header for easier documenting
//
// Command = Command Index (See Export.lua), Value = Signal Value (-1 to 1 for Joystick Axis)
void ed_fm_set_command(int command, float value)
{
	if (value > 1) // if the command comes from clickabledata.lua, it adds the device id to the value and changes the value
	{
		float device_id;
		float normalized = modf(value, &device_id);
		value = normalized * 8.f - 2.f;
	}

	switch (command)
	{
	case JoystickRoll:
		Helicopter::RollInput = limit(value, -1.0, 1.0);
		break;

	case JoystickPitch:
		Helicopter::PitchInput = limit(-value, -1.0, 1.0);
		break;

	case JoystickYaw:
		Helicopter::PedalInput = limit(-value, -1.0, 1.0);
		break;

	case JoystickThrottle:
		Helicopter::CollectiveInput = limit(((-value + 1.0) / 2.0), 0.0, 1.0);
		break;

	case trimUp:
		Helicopter::pitchTrim += 0.0015;
		break;
	case trimDown:
		Helicopter::pitchTrim -= 0.0015;
		break;
	case trimLeft:
		Helicopter::rollTrim -= 0.0015;
		break;
	case trimRight:
		Helicopter::rollTrim += 0.0015;
		break;
	case pidPUp:
		//Helicopter::td += 0.001;
		Helicopter::pidPValue += 0.001;
		//Helicopter::center_of_gravity_x += 0.001;
		break;
	case pidPDown:
		//Helicopter::td -= 0.001;
		Helicopter::pidPValue -= 0.001;
		//Helicopter::center_of_gravity_x -= 0.001;
		break;
	case pidIUp:
		Helicopter::pidIValue += 0.0001;
		//Helicopter::center_of_gravity_y += 0.001;
		break;
	case pidIDown:
		Helicopter::pidIValue -= 0.0001;
		//Helicopter::center_of_gravity_y -= 0.001;
		break;
	case pidDUp:
		Helicopter::pidDValue += 0.0001;
		break;
	case pidDDown:
		Helicopter::pidDValue -= 0.0001;
		break;
	case autopilotOff:
		Helicopter::autopilotState = 0;
		break;
	case autopilotLevel1:
		Helicopter::autopilotState = 1;
		break;
	case autopilotLevel2:
		Helicopter::autopilotState = 2;
		break;
	case autopilotLevelChangeUp:
		Helicopter::autopilotState -= 1;
		if (Helicopter::autopilotState <0) {
			Helicopter::autopilotState = 0;
		}
		break;
	case autopilotLevelChangeDown:
		Helicopter::autopilotState += 1;
		if (Helicopter::autopilotState > 2) {
			Helicopter::autopilotState = 2;
		}
		break;
	case starterButton:
		Helicopter::Engine.setStarterButton(value);
		break;
	
	case throttleIdleCutoff:
		Helicopter::Fuel.setIdleCutOff(value);
		break;
	case throttle:
		Helicopter::Engine.setThrottleInput(value);
		break;
	case throttleAxis:
		Helicopter::Engine.setThrottleInput((-value + 1.0) / 2.0);
		break;
		
	case batterySwitch:
		Helicopter::Electrics.setBatteryOn(value);
			break;

	case generatorSwitch:
		Helicopter::Electrics.setGeneratorOn(value);
		break;

	case inverterSwitch:
		Helicopter::Electrics.setInverterOn(value);
		break;

	default:
		{
			// do nothing
		}
		break;
	}
}

/*
	Mass handling 

	will be called  after ed_fm_simulate :
	you should collect mass changes in ed_fm_simulate 

	double delta_mass = 0;
	double x = 0;
	double y = 0; 
	double z = 0;
	double piece_of_mass_MOI_x = 0;
	double piece_of_mass_MOI_y = 0; 
	double piece_of_mass_MOI_z = 0;
 
	//
	while (ed_fm_change_mass(delta_mass,x,y,z,piece_of_mass_MOI_x,piece_of_mass_MOI_y,piece_of_mass_MOI_z))
	{
	//internal DCS calculations for changing mass, center of gravity,  and moments of inertia
	}
*/
/*
if (fuel_consumption_since_last_time > 0)
{
	delta_mass		 = fuel_consumption_since_last_time;
	delta_mass_pos_x = -1.0;
	delta_mass_pos_y =  1.0;
	delta_mass_pos_z =  0;

	delta_mass_moment_of_inertia_x	= 0;
	delta_mass_moment_of_inertia_y	= 0;
	delta_mass_moment_of_inertia_z	= 0;

	fuel_consumption_since_last_time = 0; // set it 0 to avoid infinite loop, because it called in cycle 
	// better to use stack like structure for mass changing 
	return true;
}
else 
{
	return false;
}
*/
bool ed_fm_change_mass(double & delta_mass,
						double & delta_mass_pos_x,
						double & delta_mass_pos_y,
						double & delta_mass_pos_z,
						double & delta_mass_moment_of_inertia_x,
						double & delta_mass_moment_of_inertia_y,
						double & delta_mass_moment_of_inertia_z
						)
{
	// TODO: better integration of fuel mass position and actual fuel usage calculation
	if (Helicopter::Motion.isMassChanged() == true)
	{
		Helicopter::Motion.getMassMomentInertiaChange(delta_mass,
											delta_mass_pos_x, 
											delta_mass_pos_y, 
											delta_mass_pos_z,
											delta_mass_moment_of_inertia_x, 
											delta_mass_moment_of_inertia_y, 
											delta_mass_moment_of_inertia_z);
		// Can't set to true...crashing right now :(
		return false;
	}

	return false;
}

/*
	set internal fuel volume, called on initialization, 
	you should distribute it inside at different fuel tanks
*/
void ed_fm_set_internal_fuel(double fuel)
{
	Helicopter::Fuel.setInternalFuel(fuel);
}

// get internal fuel volume 
double ed_fm_get_internal_fuel()
{
	return Helicopter::Fuel.getInternalFuel();
}

//set external fuel volume for each payload station , called for weapon init and on reload
void ed_fm_set_external_fuel(int station,double fuel,double x,double y,double z)
{
}
//get external fuel volume 
double ed_fm_get_external_fuel()
{
	return 0;
}

// called on ground refuel
void ed_fm_refueling_add_fuel(double fuel)
{
	return Helicopter::Fuel.refuelAdd(fuel);
}

// external model draw arguments.  size: count of elements in array
void ed_fm_set_draw_args(EdDrawArgument * drawargs, size_t size)
{
	drawargs[500].f = (float)-Helicopter::PedalInput;
	drawargs[9].f = (float)Helicopter::CollectiveInput;
	drawargs[11].f = (float)limit((Helicopter::RollInput + Helicopter::rollTrim), -1, 1);
	drawargs[15].f = (float)-limit((Helicopter::PitchInput + Helicopter::pitchTrim), -1, 1);
	drawargs[37].f = (float)Helicopter::Engine.rotorPosition;
	
}

// cockpit draw arguments
void ed_fm_set_fc3_cockpit_draw_args (EdDrawArgument * drawargs,size_t size)
{
	
	drawargs[1].f = (float)-limit((Helicopter::PitchInput + Helicopter::pitchTrim), -1, 1);
	drawargs[2].f = (float)limit((Helicopter::RollInput + Helicopter::rollTrim), -1, 1);
	drawargs[3].f = (float)Helicopter::CollectiveInput;
	drawargs[118].f = (float)Helicopter::Engine.getCoreRelatedRPM();
	double placeHolder;
	drawargs[119].f = (float)modf(Helicopter::Engine.getCoreRelatedRPM()*10, &placeHolder);
	drawargs[124].f = (float) limit(Helicopter::OutsideAirTemp/60 , 0, 1.0);

	if (Helicopter::Fuel.isIdleCutoff == true) // this is for the throttle handle bc it has one animation based on two clickables
	{
		drawargs[4].f = -1;
	}
	else
	{
		drawargs[4].f = (float)Helicopter::Engine.throttleInput;
	}
}

// shake level amplitude for head simulation  
double ed_fm_get_shake_amplitude ()
{
	// LCW
	double shake = 0.0;
	double shake_threshold_kts = 150.0;	// cruise is 150
	double max_kts = 170.0;	
	if (Helicopter::Motion.airspeed_KTS > shake_threshold_kts) {
		shake = ((Helicopter::Motion.airspeed_KTS - shake_threshold_kts) / (max_kts - shake_threshold_kts));
	}
	if (shake > 1)shake = 1; // safety
	return shake; 

	// return 0;
}

/*
will be called for your internal configuration
*/
void ed_fm_configure(const char * cfg_path)
{
}

// called when fm not needed anymore: aircraft death, mission quit etc.; use to reset flight model
void ed_fm_release ()
{
	Helicopter::Motion.clear();
	Helicopter::CollectiveInput = 0;
	Helicopter::PedalInput = 0;
	Helicopter::PitchInput = 0;
	Helicopter::RollInput = 0;
	Helicopter::rollTrim = 0;
	Helicopter::pitchTrim = 0;
	Helicopter::Aero.setGroundEffectFactor(0);
	DeInitLogFile();
}

// see enum ed_fm_param_enum in wHumanCustomPhysicsAPI.h
double ed_fm_get_param(unsigned param_enum)
{
	switch (param_enum)
	{
	case ED_FM_PROPELLER_0_RPM:	// this is neccesary for rotor sound, 470 rpm seems to be from AH-6.lua rotor_RPM definition
		return (Helicopter::Engine.N2_RPM/100) * Helicopter::rpm_100_percent;
	case ED_FM_PROPELLER_0_PITCH:  // propeller blade pitch
	case ED_FM_PROPELLER_0_TILT:   // for helicopter
	case ED_FM_PROPELLER_0_INTEGRITY_FACTOR:   // for 0 to 1 , 0 is fully broken 
		return 1;

	case ED_FM_ENGINE_1_RPM:
	case ED_FM_ENGINE_1_RELATED_RPM:
	case ED_FM_ENGINE_1_THRUST:
	case ED_FM_ENGINE_1_RELATED_THRUST:
	case ED_FM_ENGINE_1_CORE_RPM:
		return 0;
	case ED_FM_ENGINE_1_CORE_RELATED_RPM:		// This is important to use for engine sounds, heatblur, and for other internal functions like a functioning RWR
		return Helicopter::Engine.getCoreRelatedRPM();
		//return Helicopter::cockpitAPI.getParamNumber(Core_RPM);	// using rpm from lua script
	case ED_FM_ENGINE_1_CORE_THRUST:
	case ED_FM_ENGINE_1_CORE_RELATED_THRUST:
		return 0;


	case ED_FM_ENGINE_1_TEMPERATURE:
		return 100;//Helicopter::Engine.getEngineTemperature();
	case ED_FM_ENGINE_1_OIL_PRESSURE:
		//return Helicopter::Engine.getOilPressure();
	case ED_FM_ENGINE_1_FUEL_FLOW:
		//return Helicopter::Engine.getFuelFlow();

	case ED_FM_SUSPENSION_0_GEAR_POST_STATE: // from 0 to 1 : from fully retracted to full released
	case ED_FM_SUSPENSION_1_GEAR_POST_STATE: 
	case ED_FM_SUSPENSION_2_GEAR_POST_STATE: 
		return 1;

	case ED_FM_FLOW_VELOCITY:
		return 0;

	// Groups for fuel indicator
	case ED_FM_FUEL_FUEL_TANK_GROUP_0_LEFT:			// Values from different group of tanks
	case ED_FM_FUEL_FUEL_TANK_GROUP_0_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_1_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_1_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_2_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_2_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_3_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_3_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_4_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_4_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_5_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_5_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_6_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_6_RIGHT:
		return 0;

	case ED_FM_FUEL_INTERNAL_FUEL:
	case ED_FM_FUEL_TOTAL_FUEL:
		return Helicopter::Fuel.getInternalFuel();
	case ED_FM_FUEL_LOW_SIGNAL:
		return Helicopter::Fuel.isLowFuel();

	case ED_FM_ANTI_SKID_ENABLE:
	case ED_FM_COCKPIT_PRESSURIZATION_OVER_EXTERNAL: 
		return 0;

	case ED_FM_STICK_FORCE_CENTRAL_PITCH:  // i.e. trimmered position where force feeled by pilot is zero
		return Helicopter::pitchTrim;//Trim values you programmed to trim aircraft out (0 to 1)
	case ED_FM_STICK_FORCE_FACTOR_PITCH:
		return 1.0;//Force factor range from 0 to 1. Make it 1 and rather change the force factor in your aircraft setup controls menu (0 - 100 percent).
	case ED_FM_STICK_FORCE_SHAKE_AMPLITUDE_PITCH:
	case ED_FM_STICK_FORCE_SHAKE_FREQUENCY_PITCH:
		return 0;

	case ED_FM_STICK_FORCE_CENTRAL_ROLL:   // i.e. trimmered position where force feeled by pilot is zero
		return Helicopter::rollTrim;//Trim values you programmed to trim aircraft out (0 to 1)
	case ED_FM_STICK_FORCE_FACTOR_ROLL:
		return 1.0;//Force factor range from 0 to 1. Make it 1 and rather change the force factor in your aircraft setup controls menu (0 - 100 percent).
	case ED_FM_STICK_FORCE_SHAKE_AMPLITUDE_ROLL:
	case ED_FM_STICK_FORCE_SHAKE_FREQUENCY_ROLL:
		return 0;

	default:
		// silence compiler warning(s)
		break;
	}
	return 0;	
}

// These 3 functions called at beginning of every mission, use to initialize systems
void ed_fm_cold_start()
{
	Helicopter::Engine.initCold();
	Helicopter::Fuel.initCold();
	Helicopter::Electrics.initCold();
	Helicopter::Airframe.init();
	double x= limit(60, 0, 1.0);
	InitLogFile();
}

void ed_fm_hot_start()
{
	Helicopter::Engine.initHot();
	Helicopter::Fuel.initHot();
	Helicopter::Electrics.initHot();
	Helicopter::Airframe.init();
}

void ed_fm_hot_start_in_air()
{
	Helicopter::Engine.initHot();
	Helicopter::Fuel.initHot();
	Helicopter::Electrics.initHot();
	Helicopter::Airframe.init();
}

/* 
for experts only : called  after ed_fm_hot_start_in_air for balance FM at actual speed and height,
it is directly force aircraft dynamic data in case of success 
*/
bool ed_fm_make_balance (double & ax,//linear acceleration component in world coordinate system);
									double & ay,//linear acceleration component in world coordinate system
									double & az,//linear acceleration component in world coordinate system
									double & vx,//linear velocity component in world coordinate system
									double & vy,//linear velocity component in world coordinate system
									double & vz,//linear velocity component in world coordinate system
									double & omegadotx,//angular accelearation components in world coordinate system
									double & omegadoty,//angular accelearation components in world coordinate system
									double & omegadotz,//angular accelearation components in world coordinate system
									double & omegax,//angular velocity components in world coordinate system
									double & omegay,//angular velocity components in world coordinate system
									double & omegaz,//angular velocity components in world coordinate system
									double & yaw,  //radians
									double & pitch,//radians
									double & roll)//radians
{
	return false;
}

// enable debug information like force vector visualization and velocity vector visualization
bool ed_fm_enable_debug_info()
{
	return true;
}

// path to your plugin installed
void ed_fm_set_plugin_data_install_path(const char * path)
{
}

// damages and failures
// callback when preplaneed failure triggered from mission 
void ed_fm_on_planned_failure(const char * data)
{
}

// callback when damage occurs for airframe element 
void ed_fm_on_damage(int Element, double element_integrity_factor)
{
	// keep integrity information in airframe
	Helicopter::Airframe.onAirframeDamage(Element, element_integrity_factor);
}

// called in case of repair routine 
void ed_fm_repair()
{
	Helicopter::Airframe.onRepair();
}

// in case of some internal damages or system failures this function return true, to switch on repair process
bool ed_fm_need_to_be_repaired()
{
	return Helicopter::Airframe.isRepairNeeded();
}

// inform about invulnerability settings
void ed_fm_set_immortal(bool value)
{
	Helicopter::Airframe.setImmortal(value);
}

// inform about unlimited fuel
void ed_fm_unlimited_fuel(bool value)
{
	Helicopter::Fuel.setUnlimitedFuel(value);
}

// inform about simplified flight model request 
void ed_fm_set_easy_flight(bool value)
{
}

// custom properties sheet 
void ed_fm_set_property_numeric(const char * property_name,float value)
{
	// TODO: do we set these somewhere in lua?
}

// custom properties sheet 
void ed_fm_set_property_string(const char * property_name,const char * value)
{
	// TODO: do we set these somewhere in lua?
}


//inform DCS about internal simulation event, like structure damage , failure , or effect
bool ed_fm_pop_simulation_event(ed_fm_simulation_event & out)
{
	if (Helicopter::Engine.engineFire == true)
	{
		// cant get this to work
		Helicopter::Engine.engineFire = false;
		out.event_type = ED_FM_EVENT_FIRE;
		out.event_params[0] = 1; 
		out.event_params[1] = (float)-1.8; // x pos
		out.event_params[2] = (float)-0.55; // y pos
		out.event_params[3] = 0; // z pos
		out.event_params[4] = 1; // x component direction
		out.event_params[5] = 0;
		out.event_params[6] = 0;
		out.event_params[7] = 1; // particle speed
		out.event_params[8] = 1; // scale
		return true;
	}
	else
	{
		return false;
	}
}

// feedback to your fm about suspension state
void ed_fm_suspension_feedback(int idx,const ed_fm_suspension_info * info)
{
	// not used for helicopter
}
