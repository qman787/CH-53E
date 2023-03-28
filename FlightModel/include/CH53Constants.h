#pragma once

//-------------------------------------------------------
// Start of Simulation Constants 
//-------------------------------------------------------
namespace Helicopter
{
	const double		wingSpan_FT				= 72.33;		//  wing-span (ft)
	const double		wingArea_FT2			= 96.9;			//  wing area (ft^2)
	const double		diskArea				= 4098.778;		// main rotor disk area (ft^2)
	const double		pi						= acos(-1.0);	// Pi (3.14159....) - use value from math.h
	const double		degtorad				= pi/180.0;	// 
	const double		radiansToDegrees		= 180.0/pi;		// Conversion factor from radians to degrees - use value from math.h
	const double		inertia_Ix_KGM2			= 55076.0;		// Reference moment of inertia (kg/m^2) 446.0;
	const double		inertia_Iy_KGM2			= 267549.0;		// Reference moment of inertia (kg/m^2) 1219.6;
	const double		inertia_Iz_KGM2			= 284943.0;		// Reference moment of inertia (kg/m^2) 979.1;
	const double		meterToFoot				= 3.28084;		// Meter to foot conversion factor
	const double		FootToMeter				= 1/meterToFoot;	// foot to meter conversion factor
	const double		lbToNewton				= 4.44822162825;
	const double 		lbFootToNewtonMeter		= 1.35581795;

	static const double rpm_125_percent = 231;
	static const double rpm_100_percent = 185;
	static const double rotor_tilt = 5; // 5 deg fwd
	static const double torq_in_ground_effect_hover_empty = 0.40;  //  40% torq
	static const int    number_of_engines = 3;	// natops
	static const double engine_max_power[3] = {3270.0,3270.0,3270.0 };   //4,380 shp (3,270 kW)
	static const double engine_current_power[3] = {3270.0,3270.0,3270.0 }; // not really a constant ?!
	static const double blade_pitch_min = -1;	// guessed
	static const double blade_pitch_max = 24.4;	// guessed   this also matches the collective full travel  of 25.4 cm :)
	static const double mass_empty = 33226 / 2.2;  //  33226 lbs.  in Kgs
	static const double mass_aerodata = 34927 / 2.2;  //  34927 lbs.  in Kgs
	static const double mass_mtow = 73000 / 2.2;  //  73000 lbs.  in Kgs
	static const double main_rotor_diameter = 24.06;  //  meters... wikipedia
	static const double rotor_blade_length = 12.03;	// 
	static const double rotor_blade_chord = 0.66;	//  nasa cr-3144
	static const double fom = 0.6;  //  figure of merit (no units) https://www.quora.com/Will-the-Disk-Loading-tell-me-the-power-required-to-make-the-helicopter-hover  benchmark plot
	static const double solidity = 0.122;  // (total blade area/ disk area) = ((7 * (12.03 * 0.66))/ (2 * 3.1416 * (12.03 * 12.03))   https://aviation.stackexchange.com/questions/49167/what-is-the-relationship-between-speed-torque-and-pitch-in-a-helicopter

	// 12=0.001 = Cp = Cq
	// rho  =1.225
	// A = 450
	// omega = RPM in rad/s  =  185 * 0.1047198
	// R = radius = 12.03
	// Q = Cq * 1/2 * rho * A * ((omega * R) ^2)  * R  = 1251 Nm
	// P= Q * omega 


}