#pragma once

// Used in ed_fm_set_command() 
enum CH53InputCommands
{
	//Yaw
    rudderleft          = 201,
    rudderleftend       = 202,
    rudderright         = 203,
    rudderrightend      = 204,

	// commands from command_defs.lua
	starterButton 		= 3000,
	throttleIdleCutoff	= 3001,
	throttle			= 3002,
	batterySwitch 		= 3003,
	generatorSwitch 	= 3004,
	inverterSwitch 		= 3005,
	throttleAxis		= 3006,
	trimUp				= 3007,
	trimDown			= 3008,
	trimLeft			= 3009,
	trimRight			= 3010,
	trimReset			= 3011,
	trimSave			= 3012,
	
	LeftThrottleAxis		= 3013,
	RightThrottleAxis		= 3014,

	pidPUp = 3015,
	pidPDown = 3016,
	pidIUp = 3017,
	pidIDown = 3018,
	pidDUp = 3019,
	pidDDown = 3020,

	autopilotOff = 3021,
	autopilotAFCS = 3022,
	autopilotRADALT = 3023,
	autopilotBARALT = 3024,
	autopilotHDGHOLD = 3025,
	autopilotRADALT_INC = 3026,
	autopilotRADALT_DEC = 3027,
	autopilotBARALT_INC = 3028,
	autopilotBARALT_DEC = 3029,
	autopilotHDGHOLD_INC = 3030,
	autopilotHDGHOLD_DEC = 3031,
	
	autopilotBANKHOLD = 3032,
	autopilotSPEEDHOLD = 3033,

	pidTimeUp = 3034,
	pidTimeDown = 3035,

	collectiveIncrease = 3036,
	collectiveDecrease = 3037,
	pedalsLeft          = 3038,
	pedalsRight         = 3039,  
	
	// joystick axis commands
	JoystickPitch		= 2001,	
	JoystickRoll		= 2002,
	JoystickYaw			= 2003,
	JoystickThrottle	= 2004,


	// debug  testing stuff
	debugCommand_1 = 9000,

	Reserved // placeholder
};