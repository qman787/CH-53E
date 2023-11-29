#ifndef _CH53_AFCS_
#define _CH53_AFCS_

#include "ED_FM_Utility.h"
#include "CH53Aero.h"				//Aerodynamic model functions
#include "CH53Motion.h"
//#include "CH53MainRotor.h"				//Main Rotor dynamics impl.
//#include "CH53TailRotor.h"				//Tail Rotor dynamics impl.

namespace Helicopter
{
    class CH53AFCS
    {
    public:
        CH53AFCS()
        {
        }

        Vec3& getCyclicControl()
        {

            return augmentedCyclicControl;
        }

        double getCollectiveControl()
        {
            return augmentedCollectiveControl;
        }

        void vSimulate(CH53Motion rMotion, CH53Engine& rEngine, double pitchInput, double rollInput, double yawInput, double collectiveInput)
        {
            // ROLL AUGMENTATION
            augmentedCyclicControl.x = limit(rollInput - 1.74*rMotion.bodyAttitude_R.x + 0.7*rMotion.bodyAngularVelocity_RPS.y
                - 0.02*rMotion.bodyAngularVelocity_RPS.x, -1.0, 1.0);
            //-1.72*rMotion.bodyAngularVelocity_RPS.x - 0.115*rMotion.bodyAngularAcceleration_RPS2.x, -1.0, 1.0);

            // YAW AUGMENTAION
            augmentedCyclicControl.y = limit(yawInput + 1.3*rMotion.bodyAngularVelocity_RPS.y, -1.0, 1.0);
            //double yawControlAugmented = limit(yawControl, -1.0, 1.0);

            // PITCH AUGMENTATION
            augmentedCyclicControl.z = limit(pitchInput - 3.74*(/*CH53MainRotor::pitchTilt_RAD*/5.0*degtorad - rMotion.bodyAttitude_R.z)
                + 0.02*rMotion.bodyAngularVelocity_RPS.z, -1.0, 1.0);
            // -1.72*rMotion.bodyAngularVelocity_RPS.z - 0.115*rMotion.bodyAngularAcceleration_RPS2.z, -1.0, 1.0);

            // COLLECTIVE AUGMENTATION
            augmentedCollectiveControl = collectiveInput;

            LOG("AFCS:: att=(%06.3f, %06.3f, %06.3f), pitchInput=%06.3f, rollInput=%06.3f, collectiveControl=%05.1f, rpm=%05.1f\r", 
                rMotion.bodyAttitude_R.x*radiansToDegrees, rMotion.bodyAttitude_R.y*radiansToDegrees, rMotion.bodyAttitude_R.z*radiansToDegrees,
                augmentedCyclicControl.z, augmentedCyclicControl.x, augmentedCollectiveControl, rEngine.getTurbineRPM());

        }

    private:
        Vec3   augmentedCyclicControl;
        double augmentedCollectiveControl;
    };
}

#endif //_CH53_AFCS_
