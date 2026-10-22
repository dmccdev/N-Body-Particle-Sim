#include "math.h"
#include <raylib.h>
#include <random>
#include "Verlet.hpp"

void Verlet::Initiate(const ParticlesState &particles)

{
    nextAccelerationX = particles.accelerationX;
    nextAccelerationY = particles.accelerationY;
}

void Verlet::Update(ParticlesState &particles, double dt)

{
    particles.accelerationX = nextAccelerationX;
    particles.accelerationY = nextAccelerationY;

    particles.X = particles.X + particles.velocityX*dt + dt*dt *0.5*particles.accelerationX;
    particles.Y = particles.Y + particles.velocityY*dt + dt*dt *0.5*particles.accelerationY;

    particles.velocityX = particles.velocityX + 0.5 * (particles.accelerationX + nextAccelerationX)*dt;
    particles.velocityY = particles.velocityY + 0.5 * (particles.accelerationY + nextAccelerationY)*dt;   

}
