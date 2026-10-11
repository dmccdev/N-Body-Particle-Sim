#include "pairwise.hpp"
#include <random>
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"
#include <iostream>


Pairwise::Pairwise(int particlesCount, int integratorNumber, int particleConfigNumber, double dt):
particlesCount(particlesCount),
integratorNumber(integratorNumber),
particleConfigNumber(particleConfigNumber),
particles(particlesCount),
RK4Integrator(particlesCount),
dt(dt),
Stats(dt, 1.0, 300, epsilon)
{
}

void Pairwise::InitialiseParticles()
{
    switch(particleConfigNumber) //Initalise Particle Configuration
    {
    case 1:
        particles.SingleStar();
        break;
    case 2:
        particles.BinaryStar();
        break;
    case 3:
        particles.Galaxy();
        break;
    }

    Stats.getInitialParticleState(particles);

    switch(integratorNumber) //Initialise Integrator
    {
    case 1:
        break;
    case 2:
        CalculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);
        VerletIntegrator.Initiate(particles);
        break;
    case 3:
        break;
    }
}

void Pairwise::Update(bool statisticsEnabled)
{
    if (statisticsEnabled)
    {
        frameCounter++;
    }

    switch(integratorNumber)
    {
    case 1: //Euler

        CalculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);

        EulerIntegrator.Update(particles, dt);

        particles.Draw();
        break;

    case 2: //Velocity Verlet

        particles.accelerationX = VerletIntegrator.nextAccelerationX;
        particles.accelerationY = VerletIntegrator.nextAccelerationY;

        particles.X = particles.X + particles.velocityX*dt + dt*dt *0.5*particles.accelerationX;
        particles.Y = particles.Y + particles.velocityY*dt + dt*dt *0.5*particles.accelerationY;

        CalculateAccelerations(particles.X, particles.Y, VerletIntegrator.nextAccelerationX, VerletIntegrator.nextAccelerationY, particles.mass);

        particles.velocityX = particles.velocityX + 0.5 * (particles.accelerationX + VerletIntegrator.nextAccelerationX)*dt;
        particles.velocityY = particles.velocityY + 0.5 * (particles.accelerationY + VerletIntegrator.nextAccelerationY)*dt; 

        particles.Draw();
        break;

    case 3: //RK4

        //Calculate acceleration
        CalculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);

        //K1
        RK4Integrator.Kv1X = particles.accelerationX;
        RK4Integrator.Kv1Y = particles.accelerationY;
        RK4Integrator.Kr1X = particles.velocityX;
        RK4Integrator.Kr1Y = particles.velocityY;
        RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr1X, RK4Integrator.Kr1Y, dt/2);
        

        //K2
        CalculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv2X, RK4Integrator.Kv2Y, particles.mass);
        RK4Integrator.Kr2X = particles.velocityX + dt/2 * RK4Integrator.Kv1X;
        RK4Integrator.Kr2Y = particles.velocityY + dt/2 * RK4Integrator.Kv1Y;
        RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr2X, RK4Integrator.Kr2Y, dt/2);


        //K3
        CalculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv3X, RK4Integrator.Kv3Y, particles.mass);
        RK4Integrator.Kr3X = particles.velocityX + dt/2 * RK4Integrator.Kv2X;
        RK4Integrator.Kr3Y = particles.velocityY + dt/2 * RK4Integrator.Kv2Y;
        RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr3X, RK4Integrator.Kr3Y, dt);

        //K4
        CalculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv4X, RK4Integrator.Kv4Y, particles.mass);
        RK4Integrator.Kr4X = particles.velocityX + dt * RK4Integrator.Kv3X;
        RK4Integrator.Kr4Y = particles.velocityY + dt * RK4Integrator.Kv3Y;

        RK4Integrator.Update(particles, dt);

        particles.Draw();
        break;
    }
    if (statisticsEnabled)
    {
        Stats.updateStatistics(particles, frameCounter);
    }
}


void Pairwise::CalculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass)

{
    int numberOfParticles = positionX.size();
    accelerationX.setZero();
    accelerationY.setZero();

    #pragma omp parallel for

    for(int i = 0;  i < numberOfParticles; i++)
    {
        for(int j = i + 1; j < numberOfParticles; j++)
        {   
            double displacementX = positionX[j] - positionX[i];
            double displacementY = positionY[j] - positionY[i];

            std::array<double, 2> deltaAccelerations = CalculateAcceleration(displacementX, displacementY);

            double deltaAccelerationX = deltaAccelerations[0];
            double deltaAccelerationY = deltaAccelerations[1];

            accelerationX[i] += mass[j] * deltaAccelerationX;
            accelerationY[i] += mass[j] * deltaAccelerationY;

            accelerationX[j] -= mass[i] * deltaAccelerationX;
            accelerationY[j] -= mass[i] * deltaAccelerationY;
        }
    }
}

std::array<double, 2> Pairwise::CalculateAcceleration(double displacementX, double displacementY)

{
    double distanceSquared = displacementX*displacementX + displacementY*displacementY;
    double denominator =  (distanceSquared + epsilonSquared);
    double factor = G / std::sqrt(denominator*denominator*denominator);
    double deltaAccelerationX = factor * displacementX;
    double deltaAccelerationY = factor * displacementY;

    return std::array<double, 2> {deltaAccelerationX, deltaAccelerationY};
}

