#ifndef _CH53TAIL_ROTOR_H_
#define _CH53TAIL_ROTOR_H_

#include "ED_FM_Utility.h"
#include "CH53LUT.h"
#include "CH53AFCS.h"
#include <Math.h>
#include <vector>

namespace Helicopter
{
    class CH53TailRotor
    {
    public:
        static constexpr double rollTilt           = 20.0;                // Degrees
        static constexpr double bladeLength        = 3.048;               // m
        static constexpr double bladeArea          = 0.7;               // m^2
        static constexpr double numBlades          = 4;                   
        static constexpr double rotorArea          = bladeArea*numBlades; // m^2
        //Vec3                    pos           = Vec3(-12.8, 4.6, -1.9);
        Vec3                    pos                = Vec3(-12.8, 5.1, -0.6);
        //Vec3                    pos                = Vec3(-12.8, 6.1, -0.6);

    private:
        Vec3 force;
    public:
        void vSimulate(CH53Aero& rAero, CH53Motion& rMotion, CH53Engine& rEngine, CH53AFCS& rAFCS)
        {
            double rpm             = rEngine.getTurbineRPM();
            double airDencity_KgM3 = rMotion.getAirDensity();
            double airspeed_KTS    = rMotion.airspeed_KTS;
            double Cn_total        = rAero.getCnTotal();

            // fake simulation (test only)
            double blade_tip_velocity = CH53TailRotor::bladeLength*rpm*0.10472;
            double thrust = 145.0*numBlades*Cn_total*(rAFCS.getCyclicControl().y)*bladeArea*airDencity_KgM3*pow(blade_tip_velocity, 2);
            force = Vec3(0.0,
                          thrust * sin((CH53TailRotor::rollTilt)*degtorad),
                          thrust * cos((CH53TailRotor::rollTilt)*degtorad));
        }

        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = force.x;
            force_y = force.y;
            force_z = force.z;
            pos_x   = pos.x;
            pos_y   = pos.y;
            pos_z   = pos.z;
        }

    };
}

#endif // _CH53TAIL_ROTOR_H_