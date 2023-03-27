#pragma once

// Used in ed_fm_set_command() 
enum CH53InputCommands
{
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


	// joystick axis commands
	JoystickPitch		= 2001,	
	JoystickRoll		= 2002,
	JoystickYaw			= 2003,
	JoystickThrottle	= 2004,


	// debug  testing stuff
	debugCommand_1 = 9000,

	Reserved // placeholder
};