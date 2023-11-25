#ifndef _CH53TAIL_ROTOR_H_
#define _CH53TAIL_ROTOR_H_

namespace Helicopter
{
    class CH53TailRotor
    {
    public:
        static constexpr double rollTilt           = 20.0;                // Degrees
        static constexpr double bladeLength        = 3.048;               // m
        static constexpr double bladeArea          = 14.15;               // m^2
        static constexpr double numBlades          = 4;                   
        static constexpr double rotorArea          = bladeArea*numBlades; // m^2
        //Vec3                    location           = Vec3(-12.8, 4.6, -1.9);
        Vec3                    location           = Vec3(-12.8, 5.1, -0.6);

    private:
        Vec3 thrust;
    public:
        void vSimulate(CH53Motion& rMotion, double yawControl, double collectiveControl, double airspeed_KTS, double Cn_total, double rpm, double airDencity_KgM3)
        {
            double total_thrust = -45.0*Cn_total*(2 + yawControl)*rotorArea * airDencity_KgM3 * pow(rpm * CH53TailRotor::bladeLength, 2);
            thrust = Vec3(0.0,
                          total_thrust * sin((CH53TailRotor::rollTilt) * degtorad),
                          total_thrust * cos((CH53TailRotor::rollTilt) * degtorad));
        }

        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = thrust.x;
            force_y = thrust.y;
            force_z = thrust.z;
            //force_x = 0;
            //force_y = 0;
            //force_z = 0;
            pos_x   = location.x;
            pos_y   = location.y;
            pos_z   = location.z;
        }

    };
}

#endif // _CH53TAIL_ROTOR_H_