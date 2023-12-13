#include "CH53.h"

namespace CH53
{

    AFCS::AFCS()
    {
    }

    AFCS::~AFCS()
    {
    }

    void AFCS::vInit(bool hotStart, bool inAir)
    {
        augmentationMask            = AFCS::Augmentation::AFCS_ATTITUDE_HOLD |
                                      AFCS::Augmentation::AFCS_ALTITUDE_HOLD |
                                      AFCS::Augmentation::AFCS_VRATE_COMMANDER;
        cyclicControlAugmented      = Vec3(0, 0, 0);
        cyclicControlAutopilot      = Vec3(0, 0, 0);
        cyclicControl               = Vec3(0, 0, 0);
        collectiveControlAugmented  = 0;
        collectiveControlAutopilot  = 0;
        collectiveControl           = 0;
    }

    void AFCS::vRelease()
    {
    }

    Vec3& AFCS::getCyclicControl()
    {

        return cyclicControl;
    }

    double AFCS::getCollectiveControl()
    {

        return collectiveControl;
    }

    void AFCS::vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt)
    {
        // reset augmentation
        cyclicControlAugmented = Vec3();
        cyclicControlAutopilot = Vec3();
        collectiveControlAugmented = 0;
        collectiveControlAutopilot = 0;

        // CYCLIC AUGMENTATION
        if ((augmentationMask & AFCS::Augmentation::AFCS_ATTITUDE_HOLD) == AFCS::Augmentation::AFCS_ATTITUDE_HOLD)
        {
            // ROLL AUGMENTATION
            cyclicControlAugmented.x = limit(-1.740*systems.Motion.bodyAttitude_R.x                              // bank compensation 
                                             -0.420*systems.Motion.bodyAngularVelocity_RPS.x                     // roll rate compensation
                                             -0.005*systems.Motion.bodyAngularAcceleration_RPS2.x                // roll rate change factor
                                             +0.380*systems.Motion.bodyAngularVelocity_RPS.y,                    // yaw-to-roll compensation 
                                             -1.0, 1.0);

            // YAW AUGMENTAION
            cyclicControlAugmented.y = limit(1.300*systems.Motion.bodyAngularVelocity_RPS.y,                     // yaw rate compensation
                                             -1.0, 1.0);

            // PITCH AUGMENTATION
            cyclicControlAugmented.z = limit(-3.740*(MainRotor::pitchTilt_RAD - systems.Motion.bodyAttitude_R.z) // pitch compensation
                                             + 0.440*systems.Motion.bodyAngularVelocity_RPS.z                    // pitch rate compensation
                                             + 0.005*systems.Motion.bodyAngularAcceleration_RPS2.z,              // pitch rate change factor
                                             -1.0, 1.0);
        }

        // COLLECTIVE AUGMENTATION
        if ((augmentationMask & AFCS::Augmentation::AFCS_VRATE_COMMANDER) == AFCS::Augmentation::AFCS_VRATE_COMMANDER)
        {
            collectiveControlAugmented = 0;
        }

        // AUTOPILOT DIFFERENTIALS
        if ((augmentationMask & AFCS::Augmentation::AFCS_AUTOPILOT) == AFCS::Augmentation::AFCS_AUTOPILOT)
        {
            cyclicControlAutopilot.x   = limit(systems.Autopilot.autopilot_roll_differential + systems.Autopilot.autopilot_lat_differential + systems.Autopilot.autopilot_bank_differential, -1, 1);
            cyclicControlAutopilot.z   = limit(systems.Autopilot.autopilot_pitch_differential + systems.Autopilot.autopilot_long_differential + systems.Autopilot.autopilot_speed_pitch_differential, -1, 1);
            cyclicControlAutopilot.y   = limit(systems.Autopilot.autopilot_yaw_differential + systems.Autopilot.autopilot_hdg_differential, -1, 1);
            collectiveControlAutopilot = limit(systems.Autopilot.autopilot_radalt_collective_differential + systems.Autopilot.autopilot_baralt_collective_differential, 0, 1);
        }

        // FINAL CONTROL VALUES
        cyclicControl.x   = limit(systems.Input.cyclicInputTrimed.x + cyclicControlAugmented.x + cyclicControlAutopilot.x, -1, 1);
        cyclicControl.y   = limit(systems.Input.cyclicInputTrimed.y + cyclicControlAugmented.y + cyclicControlAutopilot.y, -1, 1);
        cyclicControl.z   = limit(systems.Input.cyclicInputTrimed.z + cyclicControlAugmented.z + cyclicControlAutopilot.z, -1, 1);
        collectiveControl = limit(systems.Input.collectiveInput + collectiveControlAugmented + collectiveControlAutopilot,  0, 1);


        //LOG(1, "AFCS:: att=(%06.3f, %06.3f, %06.3f), pitchInput=%06.3f, systems.Input.cyclicInputTrimed.x=%06.3f, collectiveControl=%05.1f, rpm=%05.1f\r", 
        //    systems.Motion.bodyAttitude_R.x*RAD_TO_DEG, systems.Motion.bodyAttitude_R.y*RAD_TO_DEG, systems.Motion.bodyAttitude_R.z*RAD_TO_DEG,
        //    cyclicControlAugmented.z, cyclicControlAugmented.x, collectiveControlAugmented, rEngine.getTurbineRPM());
    }
}
