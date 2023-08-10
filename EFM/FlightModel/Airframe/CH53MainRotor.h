#ifndef _CH53MAIN_ROTOR_H_
#define _CH53MAIN_ROTOR_H_

namespace Helicopter
{
    class CH53MainRotor
    {
    public:
        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = 10000.0 * sin(8.0 * degtorad);
            force_y = 10000.0 * cos(8.0 * degtorad);
            force_z = 0.0;
            pos_x   = 0.0;
            pos_y   = 5.0;
            pos_z   = 0.0;
        }

    };
}

#endif // _CH53MAIN_ROTOR_H_