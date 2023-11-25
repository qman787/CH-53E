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
        static constexpr double bladeArea          = 9.15;                // m^2
        static constexpr double bladeMass          = 200.0;               // Kg
        static constexpr double numBlades          = 7;                   
        static constexpr double rotorArea          = bladeArea*numBlades; // 64.05 // m^2
        static constexpr double rotorMass          = bladeMass*numBlades;  // Kg;
        static constexpr double CnFactor           = 0.7;
        static constexpr double pitchFactor        = 1.1;
        static constexpr double rollFactor         = 0.5;
        static constexpr double modelScaleFactor   = 0.85;
        static constexpr double bladeLenght        = Helicopter::rotor_blade_length*modelScaleFactor;
        static constexpr UINT32 numSimPoints       = 4;
        Vec3                    location[CH53MainRotor::numSimPoints] = 
            {Vec3( bladeLenght *cos(CH53MainRotor::pitchTilt*degtorad), 4.6 - bladeLenght * sin(CH53MainRotor::pitchTilt * degtorad),  0.0),  //MAIN_ROTOR_0_OCLOCK
             Vec3( 0.0,                                                 4.6,  bladeLenght),                                                   //MAIN_ROTOR_3_OCLOCK
             Vec3(-bladeLenght *cos(CH53MainRotor::pitchTilt*degtorad), 4.6 + bladeLenght * sin(CH53MainRotor::pitchTilt * degtorad),  0.0),  //MAIN_ROTOR_6_OCLOCK
             Vec3( 0.0,                                                 4.6, -bladeLenght)};                                                  //MAIN_ROTOR_9_OCLOCK
    private:
        double point_thrust[CH53MainRotor::numSimPoints];
        Vec3 thrust[CH53MainRotor::numSimPoints];
        Vec3 torque;
    public:
        void vSimulate(CH53Motion& rMotion, double pitchControl, double rollControl, double collectiveControl, double airspeed_KTS, double Cz_total, double rpm, double airDencity_KgM3,
                       double pitchRate_RPS, double rollRate_RPS)
        {
           point_thrust[MAIN_ROTOR_0_OCLOCK] = -Cz_total*(1 - pitchFactor*pitchControl)*(rotorArea/numSimPoints)*airDencity_KgM3*pow(rpm*Helicopter::rotor_blade_length, 2);
           thrust[MAIN_ROTOR_0_OCLOCK] = Vec3(point_thrust[MAIN_ROTOR_0_OCLOCK] * sin(CH53MainRotor::pitchTilt*degtorad),
                                              point_thrust[MAIN_ROTOR_0_OCLOCK] * cos(CH53MainRotor::pitchTilt*degtorad),
                                              0.0);
           point_thrust[MAIN_ROTOR_3_OCLOCK] = -Cz_total*(1 -  rollFactor*rollControl)*(rotorArea/numSimPoints)*airDencity_KgM3*pow(rpm*Helicopter::rotor_blade_length, 2);
           thrust[MAIN_ROTOR_3_OCLOCK] = Vec3(point_thrust[MAIN_ROTOR_3_OCLOCK] * sin(CH53MainRotor::pitchTilt*degtorad),
                                              point_thrust[MAIN_ROTOR_3_OCLOCK] * cos(CH53MainRotor::pitchTilt*degtorad),
                                              0.0);
           point_thrust[MAIN_ROTOR_6_OCLOCK] = -Cz_total*(1 +  pitchFactor*pitchControl)*(rotorArea/numSimPoints)*airDencity_KgM3*pow(rpm*Helicopter::rotor_blade_length, 2);
           thrust[MAIN_ROTOR_6_OCLOCK] = Vec3(point_thrust[MAIN_ROTOR_6_OCLOCK] * sin(CH53MainRotor::pitchTilt*degtorad),
                                              point_thrust[MAIN_ROTOR_6_OCLOCK] * cos(CH53MainRotor::pitchTilt*degtorad),
                                              0.0);
           point_thrust[MAIN_ROTOR_9_OCLOCK] = -Cz_total*(1 +  rollFactor*rollControl)*(rotorArea/numSimPoints)*airDencity_KgM3*pow(rpm*Helicopter::rotor_blade_length, 2);
           thrust[MAIN_ROTOR_9_OCLOCK] = Vec3(point_thrust[MAIN_ROTOR_9_OCLOCK] * sin(CH53MainRotor::pitchTilt*degtorad),
                                              point_thrust[MAIN_ROTOR_9_OCLOCK] * cos(CH53MainRotor::pitchTilt*degtorad),
                                              0.0);

           torque.x = thrust[MAIN_ROTOR_0_OCLOCK].x + thrust[MAIN_ROTOR_3_OCLOCK].x + thrust[MAIN_ROTOR_6_OCLOCK].x + thrust[MAIN_ROTOR_9_OCLOCK].x;
           torque.y = thrust[MAIN_ROTOR_0_OCLOCK].y + thrust[MAIN_ROTOR_3_OCLOCK].y + thrust[MAIN_ROTOR_6_OCLOCK].y + thrust[MAIN_ROTOR_9_OCLOCK].y;
           torque.z = thrust[MAIN_ROTOR_0_OCLOCK].z + thrust[MAIN_ROTOR_3_OCLOCK].z + thrust[MAIN_ROTOR_6_OCLOCK].z + thrust[MAIN_ROTOR_9_OCLOCK].z;


           // Pitch compenstation moment N*m
           Vec3 pitch_compensation_moment(0.0, 0.0, (-1.3*rMotion.getPitch() - 0.7*pitchRate_RPS)*rMotion.getInertia().z);
           rMotion.add_local_moment(pitch_compensation_moment);

           // Roll compenstation moment N*m
           Vec3 roll_compensation_moment((-1.4*rMotion.getRoll() - 0.8*rollRate_RPS)*rMotion.getInertia().x, 0.0, 0.0);
           rMotion.add_local_moment(roll_compensation_moment);

        }

        void getLocalForceComponent(UINT32 point, double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            if (point < numSimPoints)
            {
                force_x = thrust[point].x;
                force_y = thrust[point].y;
                force_z = thrust[point].z;
                pos_x = location[point].x;
                pos_y = location[point].y;
                pos_z = location[point].z;
            }
        }

        void getLocalMomentComponent(double& x, double& y, double& z)
        {
            x = -CnFactor * torque.x;
            y = -CnFactor * torque.y;
            z = -CnFactor * torque.z;
        }

    };
}

#endif // _CH53MAIN_ROTOR_H_