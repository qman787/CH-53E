#ifndef _CH53_FUEL_SYSTEM_
#define _CH53_FUEL_SYSTEM_

#include "CH53SimModule.h"

// also calculation for the mass of the fuel and position of it

// JP8 weight 6.65lbs/gal ?

namespace CH53
{

    class FuelTank
    {
    public:

        double volume; // capacity of tank
        double fuel; // amount of fuel in tank

        double x;
        double y;
        double z;

        FuelTank(double _volume = 0, double _fuel = 0)
            : volume(_volume)
            , fuel(_fuel)
            , x(0)
            , y(0)
            , z(0)
        {}
        ~FuelTank() {}

        // add what is possible, return remaining if full
        double addFuel(const double addition)
        {
            double space = volume - fuel;
            if (space < addition)
            {
                fuel = volume; // set to max
                return (addition - space); // overflow
            }
            fuel += addition;
            return 0;
        }

        double decFuel(const double decrement)
        {
            if (fuel < decrement)
            {
                double tmp = decrement - fuel;
                fuel = 0; // set to min
                return tmp; // remaining
            }
            fuel -= decrement;
            return 0;
        }

        double getSpace() const
        {
            return (volume - fuel);
        }
    };

    class FuelSystem : public SimModule
    {
    protected:
        bool is_unlimited_fuel;
        double previous_usage;

        FuelTank MainTank;
        FuelTank AuxTank;

    public:

        bool isIdleCutoff; // true means no fuel flow

        bool isFuelFlow;

        FuelSystem();
        virtual ~FuelSystem();
        virtual void vInit(bool hotStart, bool inAir);
        virtual void vRelease();

        /*
        // for weight-balance calculation,
        // we need amount of fuel in each tank and position
        double getFuelMass()
    {
            // JP8 weight 6.65lbs/gal ?
            //return getInternalFuel() * weightconstant;
            return 0;
        }
        */

        // is low fuel indication
        virtual bool isLowFuel() const;

        // called on initialization
        virtual void setInternalFuel(const double fuel);
        virtual double getInternalFuel() const;
        virtual void refuelAdd(const double fuel);

        virtual void setIdleCutOff(float value);


        virtual void setUnlimitedFuel(bool status);

        virtual double getUsageSinceLastFrame() const;
        virtual void clearUsageSinceLastFrame();

        virtual void vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt);
    };
}
#endif //_CH53_FUEL_SYSTEM_