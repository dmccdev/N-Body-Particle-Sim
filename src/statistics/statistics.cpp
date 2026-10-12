#include "statistics.hpp"
#include <iostream>
#include <cmath>

Statistics::Statistics(double dt, double G, double frameCalculationInterval, double epsilon)
{
    this->dt = dt;
    this->G = G;
    this->frameCalculationInterval = frameCalculationInterval;
    this->epsilon = epsilon;
    this->epsilonSquared = epsilon*epsilon;
}

void Statistics::calculateStatistics(ParticlesState &particles)
{
    //Reset Statistics
    linearMomentumX = 0.0;
    linearMomentumY = 0.0;
    totalEnergy = 0.0;
    totalKineticEnergy = 0.0;
    totalPotentialEnergy = 0.0;
    energyError = 0;

    //Calculate Linear Momentum & Total Kinetic Energy

    #pragma omp parallel for reduction(+:linearMomentumX,linearMomentumY,angularMomentum,totalKineticEnergy)

    for(int i = 0; i < particles.particlesCount; i++)
    {
        linearMomentumX += particles.mass[i] * particles.velocityX[i];
        linearMomentumY += particles.mass[i] * particles.velocityY[i];

        angularMomentum += particles.mass[i] * (particles.X[i] * particles.velocityY[i] - particles.Y[i] * particles.velocityX[i]);

        totalKineticEnergy += 0.5 * particles.mass[i] * (particles.velocityX[i] * particles.velocityX[i] + particles.velocityY[i] * particles.velocityY[i]);
    }


    //Calculating Potential Energy
    for(int i = 0;  i < particles.particlesCount; i++)
    {
        for(int j = i + 1; j < particles.particlesCount; j++)
        {   
            double displacementX = particles.X[j] - particles.X[i];
            double displacementY = particles.Y[j] - particles.Y[i];

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            if(distanceSquared > 0.0)
            {
                totalPotentialEnergy -= G * particles.mass[i] * particles.mass[j] / std::sqrt(distanceSquared + epsilonSquared);
            }
        }
    }   

    //Calculating Total Energy
    totalEnergy = totalKineticEnergy + totalPotentialEnergy;

    //Calculating Errors
    energyError = std::abs((totalEnergy - initialTotalEnergy) / initialTotalEnergy) * 100;
    linearMomentumErrorX = std::abs((linearMomentumX - initialLinearMomentumX) / initialLinearMomentumX) * 100;
    linearMomentumErrorY = std::abs((linearMomentumY - initialLinearMomentumY) / initialLinearMomentumY) * 100;
}

void Statistics::updateStatistics(ParticlesState &particles, int &frameCounter)
{
    if (!initialStatisticsCalculated && initialStateCaptured)
    {
        calculateInitialStatistics();
    }

    if(frameCounter >= frameCalculationInterval)
    {
        calculateStatistics(particles);
        frameCounter = 0;
    }
}

void Statistics::getInitialParticleState(ParticlesState &particles)
{
    initialPositionX = particles.X;
    initialPositionY = particles.Y;
    initialVelocityX = particles.velocityX;
    initialVelocityY = particles.velocityY;
    initialMass = particles.mass;
    initialStateCaptured = true;
    initialStatisticsCalculated = false;
}

void Statistics::calculateInitialStatistics()
{
    initialLinearMomentumX = 0.0;
    initialLinearMomentumY = 0.0;
    initialAngularMomentum = 0.0;
    initialTotalKineticEnergy = 0.0;
    initialTotalPotentialEnergy = 0.0;
    initialTotalEnergy = 0.0;

    energyError = 0.0;
    linearMomentumErrorX = 0.0;
    linearMomentumErrorY = 0.0;   

    int numberOfParticles = initialMass.size();

    //Calculating Inital Linear Momentum and Initial Total Kinetic Energy
    #pragma omp parallel for reduction(+:initialLinearMomentumX,initialLinearMomentumY,initialAngularMomentum,initialTotalKineticEnergy)

    for(int i = 0; i < numberOfParticles; i++)
    {
        initialLinearMomentumX += initialMass[i] * initialVelocityX[i];
        initialLinearMomentumY += initialMass[i] * initialVelocityY[i];

        initialTotalKineticEnergy += 0.5 * initialMass[i] * (initialVelocityX[i] * initialVelocityX[i] + initialVelocityY[i] * initialVelocityY[i]);
    }


    //Calculating Initial Potential Energy
    for(int i = 0;  i < numberOfParticles; i++)
    {
        for(int j = i + 1; j < numberOfParticles; j++)
        {   
            double displacementX = initialPositionX[j] - initialPositionX[i];
            double displacementY = initialPositionY[j] - initialPositionY[i];

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            if(distanceSquared > 0)
            {
                initialTotalPotentialEnergy -= G * initialMass[i] * initialMass[j] / std::sqrt(distanceSquared + epsilonSquared); 
            }
            else
            {
                std::cout << "zero";
            }
        }
    }
    //Calculating Initial Total Energy
    initialTotalEnergy = initialTotalPotentialEnergy + initialTotalKineticEnergy;
    initialStatisticsCalculated = true;

    //Remove Stored Initial particle data
    initialPositionX.resize(0);
    initialPositionY.resize(0);
    initialVelocityX.resize(0);
    initialVelocityY.resize(0);
    initialMass.resize(0);
    initialStateCaptured = false;
}
