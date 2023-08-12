#ifndef __CH53__
#define __CH53__


#include "../include/FM/wHumanCustomPhysicsAPI.h"
#include "ED_FM_Utility.h"
#include <Math.h>

namespace Helicopter
{
	enum ForceComponents
	{
		MAIN_ROTOR      = 0,
		TAIL_ROTOR,
		TAIL_STABILIZER,
		//GEARS_C,
		//GEARS_L,
		//GEARS_R,
		MAX_FORCE_COMPONENT,
	};

	enum MomentComponents
	{
		MAIN_ROTOR_MOMENT = 0,
		MAX_MOMENT_COMPONENT,
	};
}

#endif // __CH53__
