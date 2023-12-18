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
                simPoints[i] = SimulationPoint(0, 0, 0, 0, 0, 0, 0, azimuth, area);
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


    void MainRotor::vSimulate(struct Systems& systems, EDPARAM& cockpitAPI, double dt)
    {
        //torque.x = force[MAIN_ROTOR_0_OCLOCK].x + force[MAIN_ROTOR_3_OCLOCK].x + force[MAIN_ROTOR_6_OCLOCK].x + force[MAIN_ROTOR_9_OCLOCK].x;
        //torque.y = force[MAIN_ROTOR_0_OCLOCK].y + force[MAIN_ROTOR_3_OCLOCK].y + force[MAIN_ROTOR_6_OCLOCK].y + force[MAIN_ROTOR_9_OCLOCK].y;
        //torque.z = force[MAIN_ROTOR_0_OCLOCK].z + force[MAIN_ROTOR_3_OCLOCK].z + force[MAIN_ROTOR_6_OCLOCK].z + force[MAIN_ROTOR_9_OCLOCK].z;


        //// Pitch compenstation moment N*m
        //Vec3 pitch_compensation_moment(0.0, 0.0, (-1.3*systems.Motion.getPitch() - 0.7*Motion.bodyAngularVelocity_RPS.z)*systems.Motion.getInertia().z);
        //systems.Motion.add_local_moment(pitch_compensation_moment);

        //// Roll compenstation moment N*m
        //Vec3 roll_compensation_moment((-1.4*systems.Motion.getRoll() - 0.8*Motion.bodyAngularVelocity_RPS.x)*systems.Motion.getInertia().x, 0.0, 0.0);
        //systems.Motion.add_local_moment(roll_compensation_moment);

        thrust                        = 0;
        double rpm                    = systems.Engine.getTurbineRPM();
        double airDencity_KgM3        = systems.Motion.getAirDensity();
        double airspeed_KTS           = systems.Motion.airspeed_KTS;
        //double Cz_total               = rAero.getCzTotal();
        double bodyAirflowFactor      = 0.51;
        double tailRotorAirflowFactor = 0.012;
        double cyclicBladePitchFactor = 0.7;
        double bladeTipVelocity       = rpm*0.10472*rotor_blade_length;

        collectivePitch_DEG           = systems.AFCS.getCollectiveControl()*(blade_pitch_max - blade_pitch_min) + blade_pitch_min;

        // Induced rotor tilt
        Vec3 inducedRotorTilt_RAD     = Vec3(2.5*pitchTilt_RAD*systems.AFCS.getCyclicControl().x, 0, 2.5*pitchTilt_RAD*systems.AFCS.getCyclicControl().z);
        Vec3 totalRotorTilt_RAD       = Vec3(inducedRotorTilt_RAD.x, 0, pitchTilt_RAD + inducedRotorTilt_RAD.z);

        // CCW rotor sim point
        for (int i = 0; i < MainRotor::numSimPoints; ++i)
        {
            double azimuth_RAD = simPoints[i].azimuth*DEG_TO_RAD;
            simPoints[i].velocity = bladeTipVelocity +                                                          // blade tip velocity
                                    bodyAirflowFactor*systems.Motion.bodyLinearVelocity_MS.x*sin(azimuth_RAD) -       // body airflow
                                    bodyAirflowFactor*systems.Motion.bodyLinearVelocity_MS.z*cos(azimuth_RAD) +       // body airflow
                                    bladeTipVelocity*tailRotorAirflowFactor*exp(-5.0*pow(azimuth_RAD - PI, 2)); // tail induced airflow

            simPoints[i].pitch = limit(collectivePitch_DEG -
                                      (systems.AFCS.getCyclicControl().z*(blade_pitch_max*cyclicBladePitchFactor))*cos(azimuth_RAD) -
                                      (systems.AFCS.getCyclicControl().x*(blade_pitch_max*cyclicBladePitchFactor))*sin(azimuth_RAD),
                                       blade_pitch_min, blade_pitch_max);

            simPoints[i].aoa = simPoints[i].pitch; // fake
            simPoints[i].Cl = Cl(simPoints[i].aoa);
            simPoints[i].Cd = Cd(simPoints[i].aoa);
            simPoints[i].lift = 0.5*simPoints[i].Cl*airDencity_KgM3*simPoints[i].area*pow(simPoints[i].velocity, 2);
            simPoints[i].drag = 0.01*simPoints[i].Cd*airDencity_KgM3*simPoints[i].area*pow(simPoints[i].velocity, 2);
            
            if (i < xForce.size())
            {
                xForce[i].vForce = Vec3(simPoints[i].lift*sin(totalRotorTilt_RAD.z),
                                        simPoints[i].lift*cos(totalRotorTilt_RAD.z)*cos(totalRotorTilt_RAD.x),
                                        simPoints[i].lift*sin(totalRotorTilt_RAD.x));

                xForce[i].vPos = Vec3(rotorPosition.x + bladeLenght*cos(simPoints[i].azimuth*DEG_TO_RAD)*cos(totalRotorTilt_RAD.z),
                                      rotorPosition.y - bladeLenght*(cos(simPoints[i].azimuth*DEG_TO_RAD)*sin(totalRotorTilt_RAD.z)*cos(totalRotorTilt_RAD.x) + sin(simPoints[i].azimuth*DEG_TO_RAD)*sin(totalRotorTilt_RAD.x)*cos(totalRotorTilt_RAD.z)),
                                      rotorPosition.z + bladeLenght*sin(simPoints[i].azimuth*DEG_TO_RAD)*cos(totalRotorTilt_RAD.x));

            }

            thrust += simPoints[i].lift;
        }

        LOG(5, "                    sp[0].pitch=%09.1f\r", simPoints[0].pitch);
        LOG(6, "sp[3].pitch=%09.1f                       sp[1].pitch=%09.1f \r", simPoints[3].pitch, simPoints[1].pitch);
        LOG(7, "                    sp[2].pitch=%09.1f\r", simPoints[2].pitch);

        //LOG(5, "                    sp[0].lift=%09.1f\r", simPoints[0].lift);
        //LOG(6, "sp[3].lift=%09.1f                       sp[1].lift=%09.1f \r", simPoints[3].lift, simPoints[1].lift);
        //LOG(7, "                    sp[2].lift=%09.1f\r", simPoints[2].lift);

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
