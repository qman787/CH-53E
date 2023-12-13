#include "CH53.h"

namespace CH53
{
    FuelSystem::FuelSystem()
        : is_unlimited_fuel(false)
        , previous_usage(0)
        , MainTank(400 / 2.2046) // lb to kg
        , AuxTank(400 / 2.2046)
        , isIdleCutoff(false)
        , isFuelFlow(false)
    {}
    FuelSystem::~FuelSystem() {}

    void FuelSystem::vInit(bool hotStart, bool inAir)
    {
        if (hotStart)
        {
            isIdleCutoff = false;
            isFuelFlow = true;
        }
        else
        {
            isIdleCutoff = true;
            isFuelFlow = false;
        }
    }

    void FuelSystem::vRelease()
    {
    }

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
    bool FuelSystem::isLowFuel() const
    {
        // check remining fuel
        if (getInternalFuel() <= 45)
        {
            return true;
        }
        return false;
    }

    // called on initialization
    void FuelSystem::setInternalFuel(const double fuel) // <- in kg
    {
        MainTank.fuel = 0;
        AuxTank.fuel = 0;
        refuelAdd(fuel);
    }
    double FuelSystem::getInternalFuel() const
    {
        return (MainTank.fuel + AuxTank.fuel);
    }

    void FuelSystem::refuelAdd(const double fuel) // <- in kg
    {		// distribute fuel to each tank for weight balance
        double addition = fuel;
        addition = MainTank.addFuel(addition);
        addition = AuxTank.addFuel(addition);
    }

    void FuelSystem::setIdleCutOff(float value)
    {
        if (value == 1)
        {
            isIdleCutoff = true;
            return;
        }
        isIdleCutoff = false;
    }

    void FuelSystem::setUnlimitedFuel(bool status)
    {
        is_unlimited_fuel = status;
    }

    double FuelSystem::getUsageSinceLastFrame() const
    {
        return previous_usage;
    }
    void FuelSystem::clearUsageSinceLastFrame()
    {
        previous_usage = 0;
    }

    void FuelSystem::vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt)
    {
        if (is_unlimited_fuel == true)
        {
            isFuelFlow = true;
            return;
        }

        double fueltmp = systems.Engine.getTurbineRPM()*0.0364*dt;
        previous_usage += fueltmp; // add to usage since last time updated
        // TODO: transfer of fuel between tanks

        fueltmp = AuxTank.decFuel(fueltmp);
        fueltmp = MainTank.decFuel(fueltmp);

        if (getInternalFuel() > 0 && isIdleCutoff == false)
        {
            isFuelFlow = true;
        }
        else
        {
            isFuelFlow = false;
        }
    }
}
