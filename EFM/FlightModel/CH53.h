#ifndef __CH53__
#define __CH53__


#include "../include/FM/wHumanCustomPhysicsAPI.h"
#include "ED_FM_Utility.h"
#include <Math.h>

namespace Helicopter
{
	enum ForceComponents
	{
		MAIN_ROTOR_0_OCLOCK = 0,
		MAIN_ROTOR_3_OCLOCK,
		MAIN_ROTOR_6_OCLOCK,
		MAIN_ROTOR_9_OCLOCK,
		TAIL_ROTOR,
		TAIL_STABILIZER,
		//GEARS_C,
		//GEARS_L,
		//GEARS_R,
		MAX_FORCE_COMPONENT,
	};

	enum MomentComponents
	{
		MAIN_ROTOR_TORQUE = 0,
		MAX_MOMENT_COMPONENT,
	};
}

#endif // __CH53__
