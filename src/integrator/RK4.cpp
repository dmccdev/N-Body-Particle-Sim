#include "RK4.hpp"
#include "math.h"
#include <raylib.h>
#include <random>
#include <iostream>

RK4::RK4(int numberOfParticles) :
virtualX(numberOfParticles),
virtualY(numberOfParticles),
Kv1X(numberOfParticles), Kv1Y(numberOfParticles), Kr1X(numberOfParticles), Kr1Y(numberOfParticles),
Kv2X(numberOfParticles), Kv2Y(numberOfParticles), Kr2X(numberOfParticles), Kr2Y(numberOfParticles),
Kv3X(numberOfParticles), Kv3Y(numberOfParticles), Kr3X(numberOfParticles), Kr3Y(numberOfParticles),
Kv4X(numberOfParticles), Kv4Y(numberOfParticles), Kr4X(numberOfParticles), Kr4Y(numberOfParticles)
{

}

void RK4::UpdateVirtualPosition(const ParticlesState &particles, const Vec &particleSlopeX, const Vec &particleSlopeY, double dt) //Private

{
    virtualX = particles.X + dt * particleSlopeX;
    virtualY = particles.Y + dt * particleSlopeY;
}

void RK4::CalculateSlopes(const ParticlesState &particles, double dt) //Private

{
    //K1
    Kv1X = particles.accelerationX;
    Kv1Y = particles.accelerationY;
    Kr1X = particles.velocityX;
    Kr1Y = particles.velocityY;
    UpdateVirtualPosition(particles, Kr1X, Kr1Y, dt/2);
    

    //K2

    Kr2X = particles.velocityX + dt/2 * Kv1X;
    Kr2Y = particles.velocityY + dt/2 * Kv1Y;
    UpdateVirtualPosition(particles, Kr2X, Kr2Y, dt/2);


    //K3

    Kr3X = particles.velocityX + dt/2 * Kv2X;
    Kr3Y = particles.velocityY + dt/2 * Kv2Y;
    UpdateVirtualPosition(particles, Kr3X, Kr3Y, dt);

    //K4
    
    Kr4X = particles.velocityX + dt * Kv3X;
    Kr4Y = particles.velocityY + dt * Kv3Y;
}


void RK4::Update(ParticlesState &particles, double dt) //Public
{
    CalculateSlopes(particles, dt);


    //Uses RK4 Formula
    particles.X += (Kr1X + 2*Kr2X + 2*Kr3X + Kr4X) * dt/6;
    particles.Y += (Kr1Y + 2*Kr2Y+ 2*Kr3Y + Kr4Y) * dt/6;

    particles.velocityX += (Kv1X + 2*Kv2X + 2*Kv3X + Kv4X) * dt/6;
    particles.velocityY += (Kv1Y + 2*Kv2Y+ 2*Kv3Y + Kv4Y) * dt/6;
}

