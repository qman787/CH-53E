#include "CH53.h"

namespace CH53
{
    MainRotor::MainRotor()
    {
        for (int i = 0; i < MainRotor::numSimPoints; ++i)
        {
            if (MainRotor::numSimPoints > 0)
            {
                double azimuth = i * 360.0 / MainRotor::numSimPoints;
                double area = rotorArea/MainRotor::numSimPoints;
                simPoints[i] = SimulationPoint(0, 0, 0, 0, 0, 0, 0, 0, azimuth, area);
                xForce.push_back(Force(Vec3(), Vec3(rotorPosition.x + bladeLenght*cos(azimuth*DEG_TO_RAD)*cos(MainRotor::pitchTilt_RAD),
                                                    rotorPosition.y - bladeLenght*cos(azimuth*DEG_TO_RAD)*sin(MainRotor::pitchTilt_RAD),
                                                    rotorPosition.z + bladeLenght*sin(azimuth*DEG_TO_RAD))));
            }
        }

        for (int i = 0; i < MainRotor::RotorMoment::MAIN_ROTOR_MAX_NUM_MOMENTS; ++i)
        {
            xMoment.push_back(Vec3());
        }

    }

    MainRotor::~MainRotor()
    {
    }

    void MainRotor::vInit(bool hotStart, bool inAir)
    {
        bInitialized = true;
    }

    void MainRotor::vRelease()
    {
        bInitialized = false;

        SimModule::vRelease();
    }

    double MainRotor::getCollectivePitch() const
    {
        return collectivePitch_DEG;
    }

    // swashplate collective damping (actuator)
    double MainRotor::swashPlateCollectiveActuator(double currentSwashPlateCollective, double collectiveControl, double dt)
    {
        double swashPlateCollective = currentSwashPlateCollective;

        double swashPlateCollectiveChangeRate = limit(10.5*(collectiveControl - currentSwashPlateCollective), -1.5, 1.5);

        swashPlateCollective = limit(currentSwashPlateCollective + swashPlateCollectiveChangeRate*dt, 0, 1);

        return swashPlateCollective;
    }

    // swashplate cyclic damping (actuator)
    Vec3 MainRotor::swashPlateCyclicActuator(Vec3& currentSwashPlateCyclic, Vec3& cyclicControl, double dt)
    {
        Vec3 swashPlateCyclic = currentSwashPlateCyclic;

        Vec3 swashPlateCyclicChangeRate = Vec3(limit(10.5*(cyclicControl.x - currentSwashPlateCyclic.x), -5.5, 5.5),
                                               0,
                                               limit(10.5*(cyclicControl.z - currentSwashPlateCyclic.z), -5.5, 5.5));

        swashPlateCyclic = Vec3(limit(currentSwashPlateCyclic.x + swashPlateCyclicChangeRate.x*dt, -1, 1),
                                0, 
                                limit(currentSwashPlateCyclic.z + swashPlateCyclicChangeRate.z*dt, -1, 1));


        return swashPlateCyclic;
    }

    double MainRotor::getBladeAngleOfAttack(double bladePitch, double bladeVelocity, double sinkRate_MS) const
    {
        double aoa = 0;

        if (bladeVelocity != 0)
        {
            aoa = bladePitch - RAD_TO_DEG*atan(sinkRate_MS/bladeVelocity);
        }

        return aoa;
    }

    double MainRotor::bladeFlexureDynamics(double bladeLift) const
    {
        double bladeFlaping = 0;
        //bladeFlaping = (bladeFlapingMax_RAD/HALF_PI)*atan(0.0001*simPoint->lift/simPoint->area);
        //bladeFlaping = bladeFlapingMax_RAD*0.0384*simPoint->pitch;
   
        //aluminium alloy spar stress destribution 
        bladeFlaping = bladeFlapingMax_RAD*(0.000024*pow(bladeLift, 3) + 0.02*bladeLift)/3E+7;

        return bladeFlaping;
    }

    void MainRotor::vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt)
    {
        thrust                            = 0;
        double rpm                        = systems.Engine.getTurbineRPM();
        double airDencity_KgM3            = systems.Motion.getAirDensity();
        double airspeed_KTS               = systems.Motion.airspeed_KTS;
        //double Cz_total                   = rAero.getCzTotal();
        double bodyAirflowFactor          = 0.51;
        double tailRotorAirflowFactor     = 0.012;
        double cyclicBladePitchFactor     = 1.0; //0.7;
        double collectiveBladePitchFactor = 1.0; //0.7;
        double bladeTipVelocity           = rpm*0.10472*rotor_blade_length;

        swashPlateCollective              = swashPlateCollectiveActuator(swashPlateCollective, systems.AFCS.getCollectiveControl(), dt);
        collectivePitch_DEG               = swashPlateCollective*(blade_pitch_max - blade_pitch_min) + blade_pitch_min;

        swashPlateCyclic                  = swashPlateCyclicActuator(swashPlateCyclic, systems.AFCS.getCyclicControl(), dt);

        // CCW rotor sim point
        for (int i = 0; i < MainRotor::numSimPoints; ++i)
        {
            SimulationPoint* simPoint = &simPoints[i];
            double azimuth_RAD        = simPoint->azimuth*DEG_TO_RAD;
            simPoint->velocity        = bladeTipVelocity +                                                          // blade tip velocity
                                        bodyAirflowFactor*systems.Motion.bodyLinearVelocity_MS.x*sin(azimuth_RAD) - // body airflow
                                        bodyAirflowFactor*systems.Motion.bodyLinearVelocity_MS.z*cos(azimuth_RAD) + // body airflow
                                        bladeTipVelocity*tailRotorAirflowFactor*exp(-5.0*pow(azimuth_RAD - PI, 2)); // tail rotor induced airflow

            simPoint->pitch           = limit(collectiveBladePitchFactor*collectivePitch_DEG -
                                              cyclicBladePitchFactor*blade_pitch_max*swashPlateCyclic.z*cos(azimuth_RAD) -
                                              cyclicBladePitchFactor*blade_pitch_max*swashPlateCyclic.x*sin(azimuth_RAD),
                                              blade_pitch_min, blade_pitch_max);

            simPoint->aoa             = getBladeAngleOfAttack(simPoint->pitch, simPoint->velocity, systems.Motion.bodyLinearVelocity_MS.y);
            simPoint->Cl              = Cl(simPoint->aoa);
            simPoint->Cd              = 0.02*Cd(simPoint->aoa);
            simPoint->lift            = 0.5*simPoint->Cl*airDencity_KgM3*simPoint->area*pow(simPoint->velocity, 2);
            simPoint->drag            = 0.5*simPoint->Cd*airDencity_KgM3*simPoint->area*pow(simPoint->velocity, 2);
            simPoint->flaping         = bladeFlexureDynamics(simPoint->lift/simPoint->area);

            
            if (i < xForce.size())
            {
                //xForce[i].vForce = Vec3(simPoint->lift*sin(totalRotorTilt_RAD.z),
                //                        simPoint->lift*cos(totalRotorTilt_RAD.z)*cos(totalRotorTilt_RAD.x),
                //                        simPoint->lift*sin(totalRotorTilt_RAD.x));
                //xForce[i].vPos = Vec3(rotorPosition.x + bladeLenght*cos(simPoint->azimuth*DEG_TO_RAD)*cos(totalRotorTilt_RAD.z),
                //                      rotorPosition.y - bladeLenght*(cos(simPoint->azimuth*DEG_TO_RAD)*sin(totalRotorTilt_RAD.z)*cos(totalRotorTilt_RAD.x) + sin(simPoint->azimuth*DEG_TO_RAD)*sin(totalRotorTilt_RAD.x)*cos(totalRotorTilt_RAD.z)),
                //                      rotorPosition.z + bladeLenght*sin(simPoint->azimuth*DEG_TO_RAD)*cos(totalRotorTilt_RAD.x));

                xForce[i].vPos = Vec3(rotorPosition.x + bladeLenght*cos(simPoint->flaping)* cos(azimuth_RAD)*cos(pitchTilt_RAD),
                                      //rotorPosition.y - bladeLenght*(cos(azimuth_RAD)*sin(pitchTilt_RAD) + sin(azimuth_RAD)*cos(pitchTilt_RAD)),
                                      rotorPosition.y - bladeLenght*cos(azimuth_RAD)*sin(pitchTilt_RAD) + bladeLenght*sin(simPoint->flaping),
                                      rotorPosition.z + bladeLenght*cos(simPoint->flaping)* sin(azimuth_RAD));

                xForce[i].vForce = Vec3(simPoint->lift*sin(pitchTilt_RAD),
                                        simPoint->lift*cos(pitchTilt_RAD)*cos(simPoint->flaping),
                                        simPoint->lift*sin(-simPoint->flaping));
                //xForce[i].vForce = Vec3();

            }

            thrust += simPoint->lift;
        }

        UINT32 uiSimpointsIndex[4] = {0, (UINT32)(MainRotor::numSimPoints*0.25),(UINT32)(MainRotor::numSimPoints*0.50),(UINT32)(MainRotor::numSimPoints*0.75)};
        double simPointArea = simPoints[0].area;
        LOG( 5, "                           [%d]         aoa=%05.1f\r",                                        uiSimpointsIndex[0], simPoints[uiSimpointsIndex[0]].aoa);
        LOG( 6, "                           [%d].[lift=%09.1f flaping=%09.1f]\r",                              uiSimpointsIndex[0], simPoints[uiSimpointsIndex[0]].lift/simPointArea, simPoints[uiSimpointsIndex[0]].flaping*RAD_TO_DEG);
        LOG( 7, "[%d]         aoa=%05.1f                                 [%d]         aoa=%05.1f\r",           uiSimpointsIndex[3], simPoints[uiSimpointsIndex[3]].aoa,               
                                                                                                               uiSimpointsIndex[1], simPoints[uiSimpointsIndex[1]].aoa);
        LOG( 8, "[%d].[lift=%09.1f flaping=%09.1f]                       [%d].[lift=%09.1f flaping=%09.1f]\r", uiSimpointsIndex[3], simPoints[uiSimpointsIndex[3]].lift/simPointArea, simPoints[uiSimpointsIndex[3]].flaping*RAD_TO_DEG,
                                                                                                               uiSimpointsIndex[1], simPoints[uiSimpointsIndex[1]].lift/simPointArea, simPoints[uiSimpointsIndex[1]].flaping*RAD_TO_DEG);
        LOG( 9, "                           [%d]         aoa=%05.1f\r",                                        uiSimpointsIndex[2], simPoints[uiSimpointsIndex[2]].aoa);
        LOG(10, "                           [%d].[lift=%09.1f flaping=%09.1f]\r",                              uiSimpointsIndex[2], simPoints[uiSimpointsIndex[2]].lift/simPointArea, simPoints[uiSimpointsIndex[2]].flaping*RAD_TO_DEG);

        //    systems.Motion.bodyAttitude_R.x*RAD_TO_DEG, systems.Motion.bodyAttitude_R.y*RAD_TO_DEG, systems.Motion.bodyAttitude_R.z*RAD_TO_DEG,
        //    pitchControlAugmented, rollControlAugmented, collectiveControl, rpm, simPoints[0].velocity, simPoints[0].pitch, simPoints[0].Cl, simPoints[0].lift);


        // CCW rotor moment
        if (xMoment.size() == MainRotor::RotorMoment::MAIN_ROTOR_MAX_NUM_MOMENTS)
        {
            xMoment[MainRotor::RotorMoment::MAIN_ROTOR_TORQUE] = Vec3(0, 0, 0);
            for (int i = 0; i < MainRotor::numSimPoints; ++i)
            {
                xMoment[MainRotor::RotorMoment::MAIN_ROTOR_TORQUE].y += -simPoints[i].drag*rotor_blade_length;
            }

            // FLAP BACK DYNAMICS
            xMoment[MainRotor::RotorMoment::MAIN_ROTOR_FLAP_BACK] = Vec3(0, 0, 0);
        }

    }
}
