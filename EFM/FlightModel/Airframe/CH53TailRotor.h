#ifndef _CH53TAIL_ROTOR_H_
#define _CH53TAIL_ROTOR_H_

namespace Helicopter
{
    class CH53TailRotor
    {
    public:
        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = 0.0;
            force_y = -10000.0*sin(20.0 * degtorad);
            force_z = -10000.0*cos(20.0 * degtorad);
            pos_x   = -12.8;
            pos_y   = 4.6;
            pos_z   = -1.9;
        }

    };
}

#endif // _CH53TAIL_ROTOR_H_