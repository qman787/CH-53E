#ifndef _CH53MAIN_ROTOR_H_
#define _CH53MAIN_ROTOR_H_

#include "ED_FM_Utility.h"
#include <Math.h>

namespace Helicopter
{
    class CH53MainRotor
    {
    public:
        static constexpr double pitchTilt          = 5.0;                 // Degrees 
        static constexpr double pitchDeflectionMax = 8.0;                 // Degrees
        static constexpr double rollDeflectionMax  = 8.0;                 // Degrees
        static constexpr double bladeArea          = 9.15;                // m^2
        static constexpr double bladeMass          = 200.0;               // Kg
        static constexpr double numBlades          = 7;                   
        static constexpr double rotorArea          = bladeArea*numBlades; // 64.05 // m^2
        static constexpr double rotorMass          = bladeMass*numBlades;  // Kg;
        static constexpr double CnFactor           = 0.7;
        Vec3                    location           = Vec3(0.0, 4.6, 0.0);

    private:
        Vec3 thrust; 
    public:
        void vSimulate(double pitchControl, double rollControl, double collectiveControl, double airspeed_KTS, double Cz_total, double rpm, double airDencity_KgM3)
        {
           double total_thrust = -Cz_total*rotorArea*airDencity_KgM3*pow(rpm*Helicopter::rotor_blade_length, 2);
            thrust = Vec3(total_thrust * sin((CH53MainRotor::pitchTilt + pitchControl* CH53MainRotor::pitchDeflectionMax) * degtorad),
                          total_thrust * cos((CH53MainRotor::pitchTilt + pitchControl* CH53MainRotor::pitchDeflectionMax) * degtorad),
                          total_thrust * sin(                            (rollControl* CH53MainRotor::rollDeflectionMax)  * degtorad));
        }

        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = thrust.x;
            force_y = thrust.y;
            force_z = thrust.z;
            pos_x   = location.x;
            pos_y   = location.y;
            pos_z   = location.z;
        }

        void getLocalMomentComponent(double& x, double& y, double& z)
        {
            x = CnFactor *thrust.x;
            y = CnFactor *thrust.y;
            z = CnFactor *thrust.z;
        }

    };
}

#endif // _CH53MAIN_ROTOR_H_