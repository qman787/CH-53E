#ifndef _CH53TAIL_STABILIZER_H_
#define _CH53TAIL_STABILIZER_H_

namespace Helicopter
{
    class CH53TailStabilizer
    {
    public:
        void getLocalForceComponent(double& force_x, double& force_y, double& force_z, double& pos_x, double& pos_y, double& pos_z)
        {
            force_x = 0.0;
            force_y = 0.0 * cos(20.0 * degtorad);
            force_z = 0.0 * sin(20.0 * degtorad);
            pos_x   = -12.3;
            pos_y   = 4.9;
            pos_z   = 0.2;
        }

    };
}

#endif // _CH53TAIL_STABILIZER_H_