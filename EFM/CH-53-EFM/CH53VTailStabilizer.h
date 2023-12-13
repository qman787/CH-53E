#ifndef _CH53_VTAIL_STABILIZER_H_
#define _CH53_VTAIL_STABILIZER_H_

#include "CH53SimModule.h"

namespace CH53
{
    class VTailStabilizer : public SimModule
    {
    public:
        VTailStabilizer();
        virtual ~VTailStabilizer();
        virtual void vInit(bool hotStart, bool inAir);
        virtual void vRelease();
        virtual void vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt);

    };
}

#endif // _CH53_VTAIL_STABILIZER_H_