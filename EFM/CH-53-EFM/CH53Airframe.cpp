#include "CH53.h"

namespace CH53
{
    Airframe::Airframe()
    {
    }

    Airframe::~Airframe()
    {
    }

    void Airframe::vInit(bool hotStart, bool inAir)
    {
        isImmortal               = false;
        rotorIntegrityFactor     = 1;
        tailRotorIntegrityFactor = 1;

        for (int i = 0; i < IntegrityElement::COUNT; i++)
        {
            elementIntegrity[i] = 1.0;
        }
    }

    void Airframe::vRelease()
    {
    }

    // Scripts\Aircrafts\_Common\Damage.lua	 for element numbers
    void Airframe::onAirframeDamage(int IntegrityElement, double element_integrity_factor)
    {
        if (IntegrityElement >= 0 && IntegrityElement < IntegrityElement::COUNT)
        {
            elementIntegrity[IntegrityElement] = element_integrity_factor;
        }

        if (isImmortal == false)
        {
            rotorIntegrityFactor = (elementIntegrity[BLADE_1_CENTER] + elementIntegrity[BLADE_2_CENTER] + elementIntegrity[BLADE_3_CENTER] + 
                                    elementIntegrity[BLADE_4_CENTER] + elementIntegrity[BLADE_5_CENTER] + elementIntegrity[BLADE_6_CENTER]) / 6;
            tailRotorIntegrityFactor = elementIntegrity[TAIL];
        }
    }

    void Airframe::onRepair()
    {
        for (int i = 0; i < IntegrityElement::COUNT; i++)
        {
            elementIntegrity[i] = 1.0;
        }
    }

    bool Airframe::isRepairNeeded() const
    {
        // zero is fully broken
        for (int i = 0; i < IntegrityElement::COUNT; i++)
        {
            if (elementIntegrity[i] < 1)
            {
                return true;
            }
        }
        return false;
    }

    void Airframe::setImmortal(bool value)
    {
        isImmortal = value;
    }

    void Airframe::vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt)
    {
    }
}
