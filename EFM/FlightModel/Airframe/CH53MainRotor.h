#ifndef _CH53MAIN_ROTOR_H_
#define _CH53MAIN_ROTOR_H_

namespace Helicopter
{
    class CH53MainRotor
    {
    private:
        Vec3 torque; 
    public:
        void updateAeroForce(const double mass_kg, const double PitchControl, const double RollControl, const double Cx_total, const double Cx_total_no_mass, const double Cz_total, const double Cm_total, const double Cy_total, const double Cl_total, const double Cn_total)
        {
            double total_torque = -Cz_total * mass_kg;
            torque = Vec3(total_torque * sin((8.0 + PitchControl*10.0) * degtorad), 
                          total_torque * cos((8.0 + PitchControl*10.0) * degtorad),
                          total_torque * sin(       (RollControl*10.0) * degtorad));
        }

        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = torque.x;
            force_y = torque.y;
            force_z = torque.z;
            pos_x   = 0.0;
            pos_y   = 4.6;
            pos_z   = 0.0;
        }


    };
}

#endif // _CH53MAIN_ROTOR_H_