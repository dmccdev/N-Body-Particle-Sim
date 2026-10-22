#include "barneshut.hpp"
#include <omp.h>
#include <iostream>

BarnesHut::BarnesHut(int particleCount, int integratorNumber, int particleConfigNumber, double dt) :
particles(particleCount),
particleConfigNumber(particleConfigNumber),
integratorNumber(integratorNumber),
RK4Integrator(particleCount),
particleCount(particleCount),
dt(dt),
Stats(dt, 1.0, 300, 0.5)

{
}

void BarnesHut::InitialiseParticles()
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
            calculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);
            VerletIntegrator.Initiate(particles);
            break;
        case 3:
            break;
    }
}

void BarnesHut::calculateAccelerations(const Vec &positionX, const Vec &positionY, Vec &accelerationX, Vec &accelerationY, const Vec &mass)
{
    accelerationX.setZero();
    accelerationY.setZero();

    quadtree.buildTree(particles, 450, 450);
    #pragma omp parallel for
    for(int particleIndex = 0; particleIndex < particleCount; particleIndex++)
    {
        calculateAcceleration(0, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
    }
}

void BarnesHut::Update(bool statisticsEnabled)
{
    if (statisticsEnabled)
    {
        frameCounter++;
    }
    else
    {
        frameCounter = 0;
    }

    switch(integratorNumber)
    {
        case 1:
            calculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);
            EulerIntegrator.Update(particles, dt);
            break;

        case 2:
            particles.accelerationX = VerletIntegrator.nextAccelerationX;
            particles.accelerationY = VerletIntegrator.nextAccelerationY;
            particles.X = particles.X + particles.velocityX*dt + dt*dt *0.5*particles.accelerationX;
            particles.Y = particles.Y + particles.velocityY*dt + dt*dt *0.5*particles.accelerationY;
            calculateAccelerations(particles.X, particles.Y, VerletIntegrator.nextAccelerationX, VerletIntegrator.nextAccelerationY, particles.mass);
            particles.velocityX = particles.velocityX + 0.5 * (particles.accelerationX + VerletIntegrator.nextAccelerationX)*dt;
            particles.velocityY = particles.velocityY + 0.5 * (particles.accelerationY + VerletIntegrator.nextAccelerationY)*dt;  
            break;

        case 3:

            calculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);

            //K1
            RK4Integrator.Kv1X = particles.accelerationX;
            RK4Integrator.Kv1Y = particles.accelerationY;
            RK4Integrator.Kr1X = particles.velocityX;
            RK4Integrator.Kr1Y = particles.velocityY;
            RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr1X, RK4Integrator.Kr1Y, dt/2);
            

            //K2
            calculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv2X, RK4Integrator.Kv2Y, particles.mass);
            RK4Integrator.Kr2X = particles.velocityX + dt/2 * RK4Integrator.Kv1X;
            RK4Integrator.Kr2Y = particles.velocityY + dt/2 * RK4Integrator.Kv1Y;
            RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr2X, RK4Integrator.Kr2Y, dt/2);


            //K3
            calculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv3X, RK4Integrator.Kv3Y, particles.mass);
            RK4Integrator.Kr3X = particles.velocityX + dt/2 * RK4Integrator.Kv2X;
            RK4Integrator.Kr3Y = particles.velocityY + dt/2 * RK4Integrator.Kv2Y;
            RK4Integrator.UpdateVirtualPosition(particles, RK4Integrator.Kr3X, RK4Integrator.Kr3Y, dt);

            //K4
            calculateAccelerations(RK4Integrator.virtualX, RK4Integrator.virtualY, RK4Integrator.Kv4X, RK4Integrator.Kv4Y, particles.mass);
            RK4Integrator.Kr4X = particles.velocityX + dt * RK4Integrator.Kv3X;
            RK4Integrator.Kr4Y = particles.velocityY + dt * RK4Integrator.Kv3Y;

            RK4Integrator.Update(particles, dt);
            break;
    }
    if (statisticsEnabled)
    {
        Stats.updateStatistics(particles, frameCounter);
    }
    particles.Draw();
}



void BarnesHut::calculateAcceleration(int nodeIndex, int particleIndex, const Vec &positionX, const Vec &positionY, Vec &accelerationX, Vec &accelerationY, const Vec &mass) const
{
    //Node External
    if(quadtree.external(nodeIndex))
    {
        //Node empty
        if(quadtree.empty(nodeIndex))
        {
            return;
        }
        //Node full
        else
        {
            const int existingParticleIndex = quadtree.nodes[nodeIndex].particleIndex;
            if(existingParticleIndex == particleIndex)
            {
                return;
            }

            const double displacementX = positionX[existingParticleIndex] - positionX[particleIndex];
            const double displacementY = positionY[existingParticleIndex] - positionY[particleIndex];

            const double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            const double denominator =  (distanceSquared + epsilonSquared);
            const double factor = G / std::sqrt(denominator*denominator*denominator) * mass[existingParticleIndex];

            const double deltaAccelerationX = factor * displacementX;
            const double deltaAccelerationY = factor * displacementY;

            accelerationX[particleIndex] += deltaAccelerationX;
            accelerationY[particleIndex] += deltaAccelerationY;

            return;
        }
    }
    //Node internal
    else
    {            
        const double displacementX = quadtree.nodes[nodeIndex].centreMassX - positionX[particleIndex];
        const double displacementY = quadtree.nodes[nodeIndex].centreMassY - positionY[particleIndex];

        const double distanceSquared = displacementX*displacementX + displacementY*displacementY;
 
        if(distanceSquared == 0.0)
        {
            const int childIndex = quadtree.nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+1, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+2, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+3, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            return;  
        }

        const double ratio = (4*quadtree.nodes[nodeIndex].halfWidth*quadtree.nodes[nodeIndex].halfWidth)/distanceSquared;


        if(ratio < thetaSquared)
        {
            const double denominator =  (distanceSquared + epsilonSquared);
            const double factor = G / sqrt(denominator*denominator*denominator) * quadtree.nodes[nodeIndex].mass;

            const double deltaAccelerationX = factor * displacementX;
            const double deltaAccelerationY = factor * displacementY;

            accelerationX[particleIndex] += deltaAccelerationX;
            accelerationY[particleIndex] += deltaAccelerationY;

        }
        else 
        {
            const int childIndex = quadtree.nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+1, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+2, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            calculateAcceleration(childIndex+3, particleIndex, positionX, positionY, accelerationX, accelerationY, mass);
            return;
        }
    }
}
