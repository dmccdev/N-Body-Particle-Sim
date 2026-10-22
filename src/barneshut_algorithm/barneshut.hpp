#pragma once
#include "particlestate/ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"
#include "quadtree.hpp"
#include "statistics/statistics.hpp"

class BarnesHut
{
public:
    BarnesHut(int particlesCount, int integratorNumber, int particleConfigNumber, double dt);

    void InitialiseParticles();
    void calculateAccelerations(const Vec &positionX, const Vec &positionY, Vec &accelerationX, Vec &accelerationY, const Vec &mass);

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;

    ParticlesState particles;
    QuadTree quadtree;
    Statistics Stats;
    
    void Update(bool statisticsEnabled);

    void calculateAcceleration(int nodeIndex, int particleIndex, const Vec &positionX, const Vec &positionY, Vec &accelerationX, Vec &accelerationY, const Vec &mass) const;

    const int particleCount;
    const int integratorNumber;
    const int particleConfigNumber;
    int frameCounter = 0;

    const double dt;
    static constexpr double G = 1.0;

    static constexpr double epsilon = 0.5;
    static constexpr double epsilonSquared = epsilon*epsilon;

    static constexpr double theta = 0.5;
    static constexpr double thetaSquared = theta*theta;

};
